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
#include "decoder_yolov2.h"

#include "model_info.h"   // model header file generted by maixhub
#include "model_final.h"  // model header file generted by maixhub

/////////////////// config ////////////////////////
static int class_num = PARAM_CLASS_NUM;                              // class number
static const char* labels[] = PARAM_LABELS;                          // class names
static int input_w = PARAM_INPUT_W, input_h = PARAM_INPUT_H;         // input resolution
static float anchors[PARAM_ANCHOR_NUM * 2] = PARAM_ANCHORS;          // anchors, the same as train's
extern unsigned char pic[PARAM_INPUT_W * PARAM_INPUT_H * PARAM_INPUT_C];         // test input image, hwc, RGBRGB...RGBRGB
static float mean = PARAM_MEAN, std = PARAM_STD;             // preprocess params, input = (data - mean) / std
                                                             // must the same as the used when training
                                                             // can find in report.json
#define PREPROCESS_USE_NORMAL_PROCEDURE 0                    // 0: use fast preprocess, 1: use normal preprocess
///////////////////////////////////////////////////
static int recognize_count = 0;

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

static void on_draw_box(uint32_t id, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t class_id, float prob, const char* label, void* arg)
{
    static uint32_t last_id = 0xffffffff;
    if(id != last_id)
    {
        TM_PRINTF("----image:%d----\n", id);
    }
    TM_PRINTF("## img:%d, box: x:%d, y:%d, w:%d, h:%d, class:%d, prob:%.3f, label:%s\n\n", id, x, y, w, h, class_id, prob, label);
    last_id = id;
}

static void parse_output(libmaix_nn_decoder_t* decoder, tm_mat_t* outs, int class_num)
{
    tm_mat_t out = outs[0];
    float* data  = out.dataf;
    libmaix_nn_decoder_yolo2_result_t result;
    libmaix_err_t err = decoder->decode(decoder, data, (void*)&result);
    if(err != LIBMAIX_ERR_NONE)
    {
        TM_PRINTF("decode error: %d\n", err);
        return;
    }
    TM_PRINTF("decode result boxes_num: %d\n", result.boxes_num);
    libmaix_nn_decoder_yolo2_draw_result(decoder, &result, recognize_count++, labels, on_draw_box, NULL);
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

    tm_mat_t in = {3, input_h, input_w, 3, NULL};
    tm_mat_t outs[1];
    tm_err_t res;

    // init decoder
    libmaix_err_t err;
    libmaix_nn_decoder_yolo2_config_t yolo2_config = {
        .classes_num     = class_num,
        .threshold       = 0.5,   //Confidence level
        .nms_value       = 0.3,
        .anchors_num     = PARAM_ANCHOR_NUM,
        .anchors         = anchors,
        .net_in_width    = PARAM_INPUT_W,
        .net_in_height   = PARAM_INPUT_H,
        .net_out_width   = PARAM_OUTPUT_W,
        .net_out_height  = PARAM_OUTPUT_H,
        .input_width     = PARAM_INPUT_W,
        .input_height    = PARAM_INPUT_W
    };
    libmaix_nn_decoder_t *decoder = libmaix_nn_decoder_yolo2_create();
    if(!decoder)
    {
        TM_PRINTF("tm init decoder faild, maybe no memory\n");
        return -1;
    }
    err = decoder->init(decoder, &yolo2_config);
    if(err != LIBMAIX_ERR_NONE)
    {
        TM_PRINTF("tm init decoder faild, maybe no memory\n");
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
        parse_output(decoder, outs, class_num);
    }
    else
    {
        TM_PRINTF("tm run error: %d\n", res);
    }
    decoder->deinit(decoder);
    libmaix_nn_decoder_yolo2_destroy(&decoder);
    tm_unload(&mdl);
    return 0;
}
