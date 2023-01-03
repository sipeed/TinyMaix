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

#include "stdio.h"
#include "tinymaix.h"
#include "fomo.h"

#include "model_info.h"   // model header file generted by maixhub
#include "model_final.h"  // model header file generted by maixhub

/////////////////// config ////////////////////////
static int class_num = PARAM_CLASS_NUM;                              // class number
static const char* labels[] = PARAM_LABELS;                          // class names
static int input_w = PARAM_INPUT_W, input_h = PARAM_INPUT_H;         // input resolution
extern unsigned char pic[PARAM_INPUT_W * PARAM_INPUT_H * PARAM_INPUT_C];         // test input image, hwc, RGBRGB...RGBRGB
static float mean = PARAM_MEAN, std = PARAM_STD;             // preprocess params, input = (data - mean) / std
                                                             // must the same as the used when training
                                                             // can find in report.json
#define PREPROCESS_USE_NORMAL_PROCEDURE 0                    // 0: use fast preprocess, 1: use normal preprocess
///////////////////////////////////////////////////

#if TM_MDL_TYPE != TM_MDL_INT8 && TM_MDL_TYPE != TM_MDL_FP32
    #error "MaixHub only generate int8 or fp32 model"
#endif


static tm_err_t layer_cb(tm_mdl_t* mdl, tml_head_t* lh)
{   //dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch= lh->out_dims[3];
    mtype_t* output = TML_GET_OUTPUT(mdl, lh);
    return TM_OK;
    TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
    #if 1
    for(int y=0; y<h; y++){
        TM_PRINTF("[");
        for(int x=0; x<w; x++){
            TM_PRINTF("[");
            for(int c=0; c<ch; c++){
            #if TM_MDL_TYPE == TM_MDL_FP32
                TM_PRINTF("%.3f,", output[(y*w+x)*ch+c]);
            #else
                TM_PRINTF("%.3f,", TML_DEQUANT(lh,output[(y*w+x)*ch+c]));
            #endif
            }
            TM_PRINTF("],");
        }
        TM_PRINTF("],\n");
    }
    TM_PRINTF("\n");
    #endif
    return TM_OK;
}

static void on_draw_box(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t class_id, float prob, const char* label, void* arg)
{
    TM_PRINTF("## box: x:%d, y:%d, w:%d, h:%d, class:%d, score:%.3f, label:%s\n\n", x, y, w, h, class_id, prob, label);
}

static void parse_output(fomo_result_t* decoder, tm_mat_t* outs, int class_num, float threshold, int input_w, int input_h)
{
    tm_mat_t out = outs[0];
    float* data  = out.dataf;
    int res = fomo_decode(decoder, data, out.w, out.h, class_num, threshold, input_w, input_h);
    for(int i = 0; i < decoder->num; ++i)
    {
        fomo_box_t* box = decoder->box + i;
        on_draw_box(box->x, box->y, box->w, box->h, box->class_id, box->score, labels[box->class_id], NULL);
    }
    return;
}

void maixhub_image_preprocess_quantize(tm_mdl_t* mdl, uint8_t *img_data, int w, int h, float mean, float std, mtype_t *quantized_data)
{
    uint8_t* p = img_data;
    mtype_t* quant_p = quantized_data;
    tml_head_t* l0h = (tml_head_t*)mdl->b->layers_body;
    sctype_t in_s = l0h->in_s;
    zptype_t in_zp= l0h->in_zp;

    for(int i = 0; i < h * w; ++i)
    {
        #if TM_MDL_TYPE == TM_MDL_INT8
            #if PREPROCESS_USE_NORMAL_PROCEDURE
                *quant_p++ = (mtype_t)(((float)*p++ - mean) / std / in_s + in_zp);
                *quant_p++ = (mtype_t)(((float)*p++ - mean) / std / in_s + in_zp);
                *quant_p++ = (mtype_t)(((float)*p++ - mean) / std / in_s + in_zp);
            #else
                *quant_p++ = (mtype_t)((int16_t)*p++ - 128);
                *quant_p++ = (mtype_t)((int16_t)*p++ - 128);
                *quant_p++ = (mtype_t)((int16_t)*p++ - 128);
            #endif
        #else
            *quant_p++ = ((float)*p++ - mean) / std;
            *quant_p++ = ((float)*p++ - mean) / std;
            *quant_p++ = ((float)*p++ - mean) / std;
        #endif
    }
}


int main(int argc, char** argv)
{
    TM_DBGT_INIT();
    TM_PRINTF("MaixHub detection yolov2\n");

    tm_mdl_t mdl;

    float threshold = 0.5;

    tm_mat_t in = {3, input_h, input_w, 3, NULL};
    tm_mat_t outs[1];
    tm_err_t res;

    // init decoder
    fomo_result_t* decoder = fomo_init(5, NULL);
    if(!decoder)
    {
        TM_PRINTF("no mem for decoder\n");
        return -1;
    }

    tm_stat((tm_mdlbin_t*)mdl_data);

    res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
    if(res != TM_OK) {
        TM_PRINTF("tm model load err %d\n", res);
        return -1;
    }

    // preprocess and quantize
    maixhub_image_preprocess_quantize(&mdl, pic, input_w, input_h, mean, std, in.data);

    TM_DBGT_START();
    res = tm_run(&mdl, &in, outs);
    TM_DBGT("tm_run");

    if(res==TM_OK)
    {
        parse_output(decoder, outs, class_num, threshold, input_w, input_h);
    }
    else
    {
        TM_PRINTF("tm run error: %d\n", res);
    }
    fomo_destroy(&decoder);
    tm_unload(&mdl);
    return 0;
}
