/* Copyright 2022 Sipeed Technology Co., Ltd. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/


#ifdef __cplusplus
extern "C" {
#endif

#include "decoder_yolov2.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


static libmaix_err_t region_layer_init(region_layer_t *rl)
{
    libmaix_err_t flag = LIBMAIX_ERR_NONE;

    rl->coords = 4;
    rl->net_out_wh = rl->config->net_out_width * rl->config->net_out_height;
    rl->boxes_number = (rl->net_out_wh * rl->config->anchors_num);
    rl->one_box_output_number = (rl->config->classes_num + rl->coords + 1);
    rl->one_ch_output_number = rl->one_box_output_number * rl->config->anchors_num;
    rl->output_number = rl->boxes_number * rl->one_box_output_number;
    rl->output = NULL;

    //rl->scale = output_scale;
    //rl->bias = output_bias;

    // Initialize for out-of-memory situations
    rl->probs_buf = NULL;
    rl->probs = NULL;

    /*rl->output = malloc(rl->output_number * sizeof(float));
    if (rl->output == NULL)
    {
        flag = -1;
        goto malloc_error;
    }*/
    rl->boxes = (libmaix_nn_decoder_yolo2_box_t*)malloc(rl->boxes_number * sizeof(libmaix_nn_decoder_yolo2_box_t));
    if (rl->boxes == NULL)
    {
        flag = LIBMAIX_ERR_NO_MEM;
        goto malloc_error;
    }
    rl->probs_buf = (float*)malloc(rl->boxes_number * (rl->config->classes_num + 1) * sizeof(float));
    if (rl->probs_buf == NULL)
    {
        flag = LIBMAIX_ERR_NO_MEM;
        goto malloc_error;
    }
    rl->probs = (float**)malloc(rl->boxes_number * sizeof(float *));
    if (rl->probs == NULL)
    {
        flag = LIBMAIX_ERR_NO_MEM;
        goto malloc_error;
    }
    /*rl->activate = malloc(256 * sizeof(float));
    if (rl->activate == NULL)
    {
        flag = -5;
        goto malloc_error;
    }
    rl->softmax = malloc(256 * sizeof(float));
    if (rl->softmax == NULL)
    {
        flag = -5;
        goto malloc_error;
    }
    for (int i = 0; i < 256; i++)
    {
        rl->activate[i] = 1.0 / (1.0 + expf(-(i * rl->scale + rl->bias)));
        rl->softmax[i] = expf(rl->scale * (i - 255));
    }*/
    for (uint32_t i = 0; i < rl->boxes_number; i++)
    {
	    rl->probs[i] = &(rl->probs_buf[i * (rl->config->classes_num + 1)]);
    }


    return LIBMAIX_ERR_NONE;
malloc_error:
    //free(rl->output);
    free(rl->boxes);
    free(rl->probs_buf);
    free(rl->probs);
    //free(rl->activate);
    //free(rl->softmax);
    return flag;
}

static void region_layer_deinit(region_layer_t *rl)
{
    //free(rl->output);
    free(rl->boxes);
    free(rl->probs_buf);
    free(rl->probs);
    //free(rl->activate);
    //free(rl->softmax);
}

static inline float sigmod(float input)
{
    return 1.0 / (1.0 + expf(-input));
}

static void activate_array(region_layer_t *rl, int index, int n)
{
    float *output = &rl->output[index];
    //uint8_t *input = &rl->input[index];

    for (int i = 0; i < n; ++i)
        output[i] = 1.0 / (1.0 + expf(-output[i]));//rl->activate[input[i]];
}

static int entry_index(region_layer_t *rl, int location, int entry)
{
    int wh = rl->config->net_out_width * rl->config->net_out_height;
    int n = location / wh;
    int loc = location % wh;

    return n * wh * (rl->coords + rl->config->classes_num + 1) + entry * wh + loc;
}

static void softmax(float *data, int n, int stride)
{
    int i;
    // int diff;
    // float e;
    float sum = 0;
    float largest_i = data[0];

    for (i = 0; i < n; ++i)
    {
        if (data[i * stride] > largest_i)
            largest_i = data[i * stride];
    }
    for (i = 0; i < n; ++i)
    {
        float value = expf(data[i * stride] - largest_i);
        sum += value;
        data[i * stride] = value;
    }
    for (i = 0; i < n; ++i)
	{
        data[i * stride] /= sum;
	}
}


static void softmax_cpu(float *data, int n, int batch, int batch_offset, int groups, int groups_dis)
{
    int g, b;

    for (b = 0; b < batch; ++b)
    {
        for (g = 0; g < groups; ++g)
            softmax(data + b * batch_offset + g * groups_dis, n, 1);
    }
}

/**
 * x,y,p_detect sigmod, and p_classes softmax
 *
 */
static void forward_region_layer(region_layer_t *rl)
{
    int ch_offset_x, ch_offset_y;
    int ch_offset_pd; // probability of detect
    int stride;

    //for (index = 0; index < rl->output_number; index++)
    //    rl->output[index] = rl->input[index] * rl->scale + rl->bias;
    for (int n = 0; n < rl->config->anchors_num; ++n)
    {
        ch_offset_x = n * rl->one_box_output_number;
        ch_offset_y = ch_offset_x + 1;
        ch_offset_pd = ch_offset_x + rl->coords;
        for(int i = 0; i < rl->net_out_wh; ++i)
        {
            stride = i * rl->one_ch_output_number;
            rl->output[ch_offset_x + stride] = sigmod(rl->output[ch_offset_x + stride]);
            rl->output[ch_offset_y + stride] = sigmod(rl->output[ch_offset_y + stride]);
            rl->output[ch_offset_pd + stride] = sigmod(rl->output[ch_offset_pd + stride]);
        }
    }
	softmax_cpu(rl->output + rl->coords + 1, rl->config->classes_num, rl->config->anchors_num,
		rl->one_box_output_number, rl->net_out_wh, rl->one_ch_output_number);
}

static void correct_region_boxes(region_layer_t *rl, libmaix_nn_decoder_yolo2_box_t *boxes)
{
    uint32_t net_width = rl->config->net_in_width;
    uint32_t net_height = rl->config->net_in_height;
    uint32_t image_width = rl->config->input_width;
    uint32_t image_height = rl->config->input_height;
    uint32_t boxes_number = rl->boxes_number;
    int new_w = 0;
    int new_h = 0;

    if(net_width == image_width && net_height == image_height)
        return;

    if (((float)net_width / image_width) <
        ((float)net_height / image_height))
    {
        new_w = net_width;
        new_h = (image_height * net_width) / image_width;
    }
    else
    {
        new_h = net_height;
        new_w = (image_width * net_height) / image_height;
    }
    for (int i = 0; i < boxes_number; ++i)
    {
        libmaix_nn_decoder_yolo2_box_t b = boxes[i];

        b.x = (b.x - (net_width - new_w) / 2. / net_width) /
              ((float)new_w / net_width);
        b.y = (b.y - (net_height - new_h) / 2. / net_height) /
              ((float)new_h / net_height);
        b.w *= (float)net_width / new_w;
        b.h *= (float)net_height / new_h;
        boxes[i] = b;
    }
}

static libmaix_nn_decoder_yolo2_box_t get_region_box(float *x, float *biases, int n, int index, int i, int j, int w, int h, int stride)
{
    // volatile libmaix_nn_decoder_yolo2_box_t b;
    libmaix_nn_decoder_yolo2_box_t b;

    b.x = (i + x[index + 0 * stride]) / w;
    b.y = (j + x[index + 1 * stride]) / h;
    b.w = expf(x[index + 2 * stride]) * biases[2 * n] / w;
    b.h = expf(x[index + 3 * stride]) * biases[2 * n + 1] / h;
    return b;
}

static void get_region_boxes(region_layer_t *rl, float *predictions, float **probs, libmaix_nn_decoder_yolo2_box_t *boxes)
{
    uint32_t layer_width = rl->config->net_out_width;
    uint32_t layer_height = rl->config->net_out_height;
    uint32_t anchor_number = rl->config->anchors_num;
    uint32_t classes = rl->config->classes_num;
    uint32_t coords = rl->coords;
    float threshold = rl->config->threshold;

    for (int i = 0; i < rl->net_out_wh; ++i)
    {
        int row = i / layer_width;
        int col = i % layer_width;

        for (int n = 0; n < anchor_number; ++n)
        {
            // int index = i * anchor_number + n;
            int index = n * rl->net_out_wh + i;

            for (int j = 0; j < classes; ++j)
                probs[index][j] = 0;
            int box_ch_offset = rl->one_ch_output_number * i + rl->one_box_output_number * n;
            int obj_ch_offset = box_ch_offset + coords;



            float max = 0;
            float p_classes_sum = 0;

            for (int j = 0; j < classes; ++j)
            {
                int class_index = obj_ch_offset + j + 1;
                float prob = predictions[obj_ch_offset] * predictions[class_index];
                p_classes_sum += prob;

                probs[index][j] = (prob > threshold) ? prob : 0;
                if (prob > max)
                {
                    max = prob;
                }
            }
            probs[index][classes] = max;
            if(p_classes_sum > 0)
            {
                boxes[index] = get_region_box(predictions, rl->config->anchors, n, box_ch_offset, col, row,
                                          layer_width, layer_height, 1);
            }
        }
    }
    int index = 874;
    correct_region_boxes(rl, boxes);
}

static int nms_comparator(const void *pa, const void *pb)
{
    sortable_box_t a = *(sortable_box_t *)pa;
    sortable_box_t b = *(sortable_box_t *)pb;
    float diff = a.probs[a.index][b.class_id] - b.probs[b.index][b.class_id];

    if (diff < 0)
        return 1;
    else if (diff > 0)
        return -1;
    return 0;
}

static float overlap(float x1, float w1, float x2, float w2)
{
    float l1 = x1 - w1 / 2;
    float l2 = x2 - w2 / 2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1 / 2;
    float r2 = x2 + w2 / 2;
    float right = r1 < r2 ? r1 : r2;

    return right - left;
}

static float box_intersection(libmaix_nn_decoder_yolo2_box_t a, libmaix_nn_decoder_yolo2_box_t b)
{
    float w = overlap(a.x, a.w, b.x, b.w);
    float h = overlap(a.y, a.h, b.y, b.h);

    if (w < 0 || h < 0)
        return 0;
    return w * h;
}

static float box_union(libmaix_nn_decoder_yolo2_box_t a, libmaix_nn_decoder_yolo2_box_t b)
{
    float i = box_intersection(a, b);
    float u = a.w * a.h + b.w * b.h - i;

    return u;
}

static float box_iou(libmaix_nn_decoder_yolo2_box_t a, libmaix_nn_decoder_yolo2_box_t b)
{
    return box_intersection(a, b) / box_union(a, b);
}

static void do_nms_sort(region_layer_t *rl, libmaix_nn_decoder_yolo2_box_t *boxes, float **probs)
{
    uint32_t boxes_number = rl->boxes_number;
    uint32_t classes = rl->config->classes_num;
    float nms_value = rl->config->nms_value;
    int i, j, k;
    sortable_box_t s[boxes_number];

    for (i = 0; i < boxes_number; ++i)
    {
        s[i].index = i;
        s[i].class_id = 0;
        s[i].probs = probs;
    }

    for (k = 0; k < classes; ++k)
    {
        for (i = 0; i < boxes_number; ++i)
            s[i].class_id = k;
        qsort(s, boxes_number, sizeof(sortable_box_t), nms_comparator);
        for (i = 0; i < boxes_number; ++i)
        {
            if (probs[s[i].index][k] == 0)
                continue;
            libmaix_nn_decoder_yolo2_box_t a = boxes[s[i].index];

            for (j = i + 1; j < boxes_number; ++j)
            {
                libmaix_nn_decoder_yolo2_box_t b = boxes[s[j].index];

                if (box_iou(a, b) > nms_value)
                    probs[s[j].index][k] = 0;
            }
        }
    }
}

static int max_index(float *a, int n)
{
    int i, max_i = 0;
    float max = a[0];

    for (i = 1; i < n; ++i)
    {
        if (a[i] > max)
        {
            max = a[i];
            max_i = i;
        }
    }
    return max_i;
}

static void region_layer_run(region_layer_t *rl)
{
	forward_region_layer(rl);
    get_region_boxes(rl, rl->output, rl->probs, rl->boxes);
    do_nms_sort(rl, rl->boxes, rl->probs);
}



libmaix_err_t libmaix_nn_decoder_yolo2_init(struct libmaix_nn_decoder *obj, void* config)
{
    libmaix_nn_decoder_yolo2_deinit(obj);
    region_layer_t* rl = (region_layer_t*)malloc(sizeof(region_layer_t));
    if(!rl)
    {
        return LIBMAIX_ERR_NO_MEM;
    }
    memset(rl, 0, sizeof(region_layer_t));
    obj->data = (void*)rl;
    rl->config = (libmaix_nn_decoder_yolo2_config_t*)config;
    libmaix_err_t err = region_layer_init(rl);
    if(err != LIBMAIX_ERR_NONE)
    {
        return err;
    }
    return LIBMAIX_ERR_NONE;
}

libmaix_err_t libmaix_nn_decoder_yolo2_deinit(struct libmaix_nn_decoder *obj)
{
    region_layer_t* rl = (region_layer_t*)obj->data;
    if(rl)
    {
        region_layer_deinit(rl);
        free(rl);
        obj->data = NULL;
    }
    return LIBMAIX_ERR_NONE;
}

libmaix_err_t libmaix_nn_decoder_yolo2_decode(struct libmaix_nn_decoder *obj, float* feature_map_data, void* result_in)
{
    region_layer_t* rl = (region_layer_t*)obj->data;
    libmaix_nn_decoder_yolo2_result_t* result = (libmaix_nn_decoder_yolo2_result_t*)result_in;
    rl->output = feature_map_data;
    region_layer_run(rl);
    result->boxes_num = rl->boxes_number;
    result->boxes     = rl->boxes;
    result->probs     = rl->probs;
    return LIBMAIX_ERR_NONE;
}


void libmaix_nn_decoder_yolo2_draw_result(struct libmaix_nn_decoder* obj, libmaix_nn_decoder_yolo2_result_t* result, uint32_t id, const char** labels, callback_draw_result_func_t callback, void* arg)
{
    // assert(callback);
    region_layer_t* rl = (region_layer_t*)obj->data;
    const char* label = NULL;
    uint32_t image_width = rl->config->input_width;
    uint32_t image_height = rl->config->input_height;
    float threshold = rl->config->threshold;
    libmaix_nn_decoder_yolo2_box_t *boxes = result->boxes;

    for (int i = 0; i < result->boxes_num; ++i)
    {
        int class_id = max_index(rl->probs[i], rl->config->classes_num);
        float prob = result->probs[i][class_id];
        if (prob > threshold)
        {
            libmaix_nn_decoder_yolo2_box_t *b = boxes + i;
            uint32_t x1 = b->x * image_width - (b->w * image_width / 2);
            uint32_t y1 = b->y * image_height - (b->h * image_height / 2);
            uint32_t w = b->w * image_width;
            uint32_t h = b->h * image_height;
            if(labels)
            {
                label = labels[class_id];
            }
            callback(id, x1, y1, w, h, class_id, prob, label, arg);
        }
    }
}

libmaix_nn_decoder_t* libmaix_nn_decoder_yolo2_create()
{
    libmaix_nn_decoder_t* obj = (libmaix_nn_decoder_t*)malloc(sizeof(libmaix_nn_decoder_t));
    if(!obj)
    {
        return NULL;
    }
    memset(obj, 0, sizeof(libmaix_nn_decoder_t)); //init space
    obj->init = libmaix_nn_decoder_yolo2_init;
    obj->deinit = libmaix_nn_decoder_yolo2_deinit;
    obj->decode = libmaix_nn_decoder_yolo2_decode;

    return obj;
}

void libmaix_nn_decoder_yolo2_destroy(libmaix_nn_decoder_t** obj)
{
    if(*obj)
    {
        (*obj)->deinit(*obj);
        free(*obj);
        *obj = NULL;
    }
}

#ifdef __cplusplus
}
#endif
