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

#if TM_MDL_TYPE == TM_MDL_INT8
#include "../../tools/tmdl/vww96_q.h"
#elif TM_MDL_TYPE == TM_MDL_FP32
#include "../../tools/tmdl/vww96_f.h"
#else
#error "fill your modl path here"
#endif

#define IMG_L 96
#define IMG_CH 3
#define CLASS_NUM 2


//#include "pic/pic_noperson1.h"
//#include "pic/pic_noperson2.h"
//#include "pic/pic_noperson3.h"

#include "pic/pic_person1.h"
//#include "pic/pic_person2.h"
//#include "pic/pic_person3.h"

char* labels[CLASS_NUM] = {"no-person", "person"};

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

static void parse_output(tm_mat_t* outs)
{
    tm_mat_t out = outs[0];
    float* data  = out.dataf;
    float maxp = 0;
    int maxi = -1;
    for(int i=0; i<CLASS_NUM; i++){
        printf("%d: %.3f\n", i, data[i]);
        if(data[i] > maxp) {
            maxi = i;
            maxp = data[i];
        }
    }
    TM_PRINTF("### Predict output is: Class %d, %s, prob %.3f\n", maxi, labels[maxi], maxp);
    return;
}

int main(int argc, char** argv)
{   TM_DBGT_INIT();
    TM_PRINTF("mnist demo\n");
    tm_mdl_t mdl;

    tm_mat_t in_uint8 = {3,IMG_L,IMG_L,IMG_CH, {(mtype_t*)pic}};
    tm_mat_t in = {3,IMG_L,IMG_L,IMG_CH, {NULL}};
    tm_mat_t outs[1];
    tm_err_t res;
    tm_stat((tm_mdlbin_t*)mdl_data); 

    res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
    if(res != TM_OK) {
        TM_PRINTF("tm model load err %d\n", res);
        return -1;
    }

#if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16) 
    res = tm_preprocess(&mdl, TMPP_UINT2INT, &in_uint8, &in); 
#else
    res = tm_preprocess(&mdl, TMPP_UINT2FP01, &in_uint8, &in); 
#endif
    TM_DBGT_START();
    res = tm_run(&mdl, &in, outs);
    TM_DBGT("tm_run");
    if(res==TM_OK) parse_output(outs);  
    else TM_PRINTF("tm run error: %d\n", res);
    tm_unload(&mdl);                 
    return 0;
}
