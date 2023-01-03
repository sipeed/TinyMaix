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


#include "fomo.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "tm_port.h"

static inline float min(float a, float b)
{
    return a < b ? a : b;
}

static inline float max(float a, float b)
{
    return a > b ? a : b;
}

/**
 * @result_data can be NULL, if NULL, malloc memory for result
 *              or you can provide memory for result
*/
fomo_result_t* fomo_init(int max_box_num, fomo_result_t* result_data)
{
    fomo_result_t* result = result_data;
    if(!result)
    {
        result = (fomo_result_t*)tm_malloc(sizeof(fomo_result_t));
        if (!result)
        {
            return NULL;
        }
        result->box = (fomo_box_t*)tm_malloc(sizeof(fomo_box_t) * max_box_num);
        if (!result->box)
        {
            tm_free(result);
            return NULL;
        }
        result->malloc = true;
    }
    else if(!result->box)
    {
        TM_PRINTF("result->box is NULL\n");
        return NULL;
    }
    else
    {
        result->malloc = false;
    }
    result->num = 0;
    result->max_num = max_box_num;
    return result;
}

/**
 * @brief  merge overlapped boxes to one box
 * @param result the curr_idx box of result is new coming box
 * @return true if curr_idx box is overlapped with previous boxes
*/
bool overlap(fomo_result_t *result, int curr_idx)
{
    fomo_box_t* box = result->box + curr_idx;
    for(int i=0; i<curr_idx; ++i)
    {
        fomo_box_t* box0 = result->box + i;
        if(box->class_id != box0->class_id)
        {
            continue;
        }
        if(box->x > box0->x + box0->w || box->x + box->w < box0->x || box->y > box0->y + box0->h || box->y + box->h < box0->y)
        {
            continue;
        }
        box0->x = min(box->x, box0->x);
        box0->y = min(box->y, box0->y);
        box0->w = max(box->x + box->w, box0->x + box0->w) - box0->x;
        box0->h = max(box->y + box->h, box0->y + box0->h) - box0->y;
        box0->score = max(box->score, box0->score);
        return true;
    }
    return false;
}

/**
 * merge overlapped boxes to one box
*/
void overlap2(fomo_result_t *result, int out_w, int out_h, int input_w, int input_h)
{
    fomo_box_t* box;
    fomo_box_t* box0;
    int num = 0;
    float scale_x = input_w / out_w;
    float scale_y = input_h / out_h;

    for(int i=0; i<result->num; ++i)
    {
        for(int j=i+1; j<result->num; ++j)
        {
            box0 = result->box + i;
            box = result->box + j;
            if(box->class_id != box0->class_id)
            {
                continue;
            }
            if(box->x > box0->x + box0->w || box->x + box->w < box0->x || box->y > box0->y + box0->h || box->y + box->h < box0->y)
            {
                continue;
            }
            box0->x = min(box->x, box0->x);
            box0->y = min(box->y, box0->y);
            box0->w = max(box->x + box->w, box0->x + box0->w) - box0->x;
            box0->h = max(box->y + box->h, box0->y + box0->h) - box0->y;
            box0->score = max(box->score, box0->score);
            box->class_id = -1;
        }
    }
    if(input_w ==0 || input_h == 0)
    {
        for(int i=0; i<result->num; ++i)
        {
            if(result->box[i].class_id != -1)
            {

                result->box[num++] = result->box[i]; // auto value copy
            }
        }
    }
    else
    {
        for(int i=0; i<result->num; ++i)
        {
            box0 = result->box + num;
            if(result->box[i].class_id != -1)
            {
                box = result->box + i;
                box0->x = box->x * scale_x;
                box0->y = box->y * scale_y;
                box0->w = box->w * scale_x;
                box0->h = box->h * scale_y;
                box0->score = box->score;
                box0->class_id = box->class_id;
                num++;
            }
            else
            {
                box0->x = box0->x * scale_x;
                box0->y = box0->y * scale_y;
                box0->w = box0->w * scale_x;
                box0->h = box0->h * scale_y;
            }
        }
    }
    result->num = num;
}

/**
 * @param result result boxes, should be initialized by fomo_init,
 *              box contains left-top coordinate and width/height
 * @param output output of network, should be dequantized float value with hwc layout
 * @param input_w input image width, can be 0 if result box is relative value(0.0~1.0)
 * @param input_h input image height, can be 0 if result box is relative value(0.0~1.0)
*/
int fomo_decode(fomo_result_t *result, float *output, int out_w, int out_h, int class_num, float score_threshold, int input_w, int input_h)
{
    float value;
    bool end = false;

    result->num = 0;
    for(int c = 0; c < class_num; ++c)
    {
        for(int i=0; i < out_h; ++i)
        {
            for(int j = 0; j < out_w; ++j)
            {
                value = output[(i * out_w + j) * class_num + c];
                if(value > score_threshold)
                {
                    result->box[result->num].x = j;
                    result->box[result->num].y = i;
                    result->box[result->num].w = 1;
                    result->box[result->num].h = 1;
                    result->box[result->num].score = value;
                    result->box[result->num].class_id = c;
                    if(overlap(result, result->num))
                    {
                        continue;
                    }
                    result->num++;
                    if (result->num >= result->max_num)
                    {
                        end = true;
                        break;
                    }
                }
            }
            if(end)
            {
                break;
            }
        }
        if(end)
        {
            break;
        }
    }
    overlap2(result, out_w, out_h, input_w, input_h);
}


void fomo_destroy(fomo_result_t **result)
{
    if(*result != NULL)
    {
        if(!(*result)->malloc)
        {
            return;
        }
        tm_free(*result);
        *result = NULL;
    }
}



