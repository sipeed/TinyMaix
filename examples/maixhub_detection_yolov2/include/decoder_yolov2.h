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


#ifndef __LIBMAIX_NN_DECODER_YOLO2_H
#define __LIBMAIX_NN_DECODER_YOLO2_H



#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LIBMAIX_ERR_NONE   = 0,
    LIBMAIX_ERR_PARAM  = 1,
    LIBMAIX_ERR_NO_MEM = 2,
    LIBMAIX_ERR_NOT_IMPLEMENT = 3,
    LIBMAIX_ERR_NOT_READY     = 4,
    LIBMAIX_ERR_NOT_INIT      = 5,
    LIBMAIX_ERR_NOT_PERMIT    = 6,
    LIBMAIX_ERR_NOT_EXEC      = 7,
    LIBMAIX_ERR_UNKNOWN,
}libmaix_err_t;

typedef struct
{
    float x;
    float y;
    float w;
    float h;
}libmaix_nn_decoder_yolo2_box_t;

typedef struct
{
    uint32_t classes_num;
    float    threshold;
    float    nms_value;
    uint32_t anchors_num;
    float*   anchors;
    uint32_t net_in_width;     // net input width, e.g. 224
    uint32_t net_in_height;
    uint32_t net_out_width;    // net output width, e.g. 7
    uint32_t net_out_height;
    uint32_t input_width;      // input image width, e.g. 320, if it not the same as net input, the output box max width is the same as image width
    uint32_t input_height;
}libmaix_nn_decoder_yolo2_config_t;


typedef struct
{
    libmaix_nn_decoder_yolo2_config_t* config;
    uint32_t coords;          // coord length, 4: xmin, ymin, xmax, ymax
    uint32_t net_out_wh;
    uint32_t boxes_number;
    uint32_t output_number;
    uint32_t one_box_output_number;
    uint32_t one_ch_output_number;
    float scale;
    float bias;
    libmaix_nn_decoder_yolo2_box_t* boxes;
    //uint8_t *input;
    float *output;
    float *probs_buf;
    float **probs;
    float *activate;
    float *softmax;
} region_layer_t;

typedef struct
{
    int index;
    int class_id;
    float **probs;
}sortable_box_t;


typedef enum
{
    LIBMAIX_NN_DTYPE_UINT8  = 0,
    LIBMAIX_NN_DTYPE_INT8   = 1,
    LIBMAIX_NN_DTYPE_FLOAT  = 2,
}libmaix_nn_dtype_t;               // data type

typedef enum
{
    LIBMAIX_NN_LAYOUT_HWC    = 0,
    LIBMAIX_NN_LAYOUT_CHW    = 1,
}libmaix_nn_layout_t;

typedef struct libmaix_nn_decoder
{
    libmaix_err_t (*init)(struct libmaix_nn_decoder *obj, void* config);
    libmaix_err_t (*deinit)(struct libmaix_nn_decoder *obj);
    libmaix_err_t (*decode)(struct libmaix_nn_decoder *obj, float* feature_map_data, void* result);

    void* data;
} libmaix_nn_decoder_t;

typedef struct
{
    uint32_t                        boxes_num;
    libmaix_nn_decoder_yolo2_box_t* boxes;
    float**                         probs;      // shape: (boxes_num, classes_num, dtype='float')
}libmaix_nn_decoder_yolo2_result_t;


typedef void (*callback_draw_result_func_t)(uint32_t id, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t class_id, float prob, const char* label, void* arg);

libmaix_err_t libmaix_nn_decoder_yolo2_init(struct libmaix_nn_decoder* obj, void* config);
libmaix_err_t libmaix_nn_decoder_yolo2_deinit(struct libmaix_nn_decoder* obj);
libmaix_err_t libmaix_nn_decoder_yolo2_decode(struct libmaix_nn_decoder* obj, float* feature_map_data, void* result);
void libmaix_nn_decoder_yolo2_draw_result(struct libmaix_nn_decoder* obj, libmaix_nn_decoder_yolo2_result_t* result, uint32_t id, const char** labels, callback_draw_result_func_t callback, void* arg);

libmaix_nn_decoder_t* libmaix_nn_decoder_yolo2_create();
void libmaix_nn_decoder_yolo2_destroy(libmaix_nn_decoder_t** obj);


#ifdef __cplusplus
}
#endif

#endif
