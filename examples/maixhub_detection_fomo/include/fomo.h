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

#ifndef __FOMO_H__
#define __FOMO_H__

#include "stdbool.h"

typedef struct {
    float x;
    float y;
    float w;
    float h;
    float score;
    int   class_id;
} fomo_box_t;

typedef struct {
    int num;
    int max_num;
    fomo_box_t *box;

    bool malloc;
} fomo_result_t;

fomo_result_t* fomo_init(int max_box_num, fomo_result_t* result_data);

/**
 * @param result result boxes, should be initialized by fomo_init,
 *              box contains left-top coordinate and width/height
 * @param input_w input image width, can be 0 if result box is relative value(0.0~1.0)
 * @param input_h input image height, can be 0 if result box is relative value(0.0~1.0)
*/
int fomo_decode(fomo_result_t *result, float *output, int out_w, int out_h, int class_num, float score_threshold, int input_w, int input_h);

void fomo_destroy(fomo_result_t **result);


#endif
