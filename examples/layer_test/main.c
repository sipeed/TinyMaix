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

#if TM_MDL_TYPE == TM_MDL_FP32
void dump_mat(tm_mat_t* mat)
{
    for(int y=0; y<mat->h; y++){
        for(int x=0; x<mat->w; x++){
            for(int c=0; c<mat->c; c++){
                TM_PRINTF("%.1f,", mat->data[(y*mat->w+x)*mat->c+c]);
            }
        }
        TM_PRINTF("\n");
    }
    TM_PRINTF("\n");
}

tm_err_t check_result(tm_mat_t* mat, float* good)
{
    tm_err_t res = TM_OK;
    int idx = 0;
    for(int y=0; y<mat->h; y++){
        for(int x=0; x<mat->w; x++){
            for(int c=0; c<mat->c; c++){
                idx = (y*mat->w+x)*mat->c+c;
                if(mat->data[idx] != good[idx]){
                    res = TM_ERR;
                    break;
                }
            }
        }
    }
    return res;
}

#define DEAL_RESULT(res, str) if(res!=TM_OK){TM_PRINTF("!!! Test %s failed!\n", str);return res;}else{TM_PRINTF("=== Test %s OK!\n", str);};

int main(int argc, char** argv)
{
    tm_err_t res;
    TM_PRINTF("layer test\n");
    float buf_in[9]={\
        1,2,3,
        4,5,6,
        7,8,9
    };
    float buf_out[9];
    tm_mat_t in  = {3,3,3,1, buf_in};
    tm_mat_t out = {3,3,3,1, buf_out};

    float w[9] = {1,1,1, 1,1,1, 1,1,1};
    float b[9] = {0,0,0, 0,0,0, 0,0,0};

    //conv valid 
    float res_valid[1] = {45};
    out.w=1; out.h=1;
    tml_conv2d_dwconv2d(&in, &out, w, b, 3, 3, 1, 1, 1, 1, 0, \
    0, 0, 0, 0, 0,\
    NULL, 1, 0, 1, 0);
    dump_mat(&out);
    res = check_result(&out, res_valid);
    DEAL_RESULT(res, "conv2d_valid");

    //conv same
    float res_same[9] = {12,21,16,  27,45,33, 24,39,28};
    out.w=3; out.h=3;
    tml_conv2d_dwconv2d(&in, &out, w, b, 3, 3, 1, 1, 1, 1, 0, \
    1, 1, 1, 1, 0,\
    NULL, 1, 0, 1, 0);
    dump_mat(&out);
    res = check_result(&out, res_same);
    DEAL_RESULT(res, "conv2d_same");

    //conv same stride
    float res_same_s[9] = {12,16, 24,28};
    out.w=2; out.h=2;
    tml_conv2d_dwconv2d(&in, &out, w, b, 3, 3, 2, 2, 1, 1, 0, \
    1, 1, 1, 1, 0,\
    NULL, 1, 0, 1, 0);
    dump_mat(&out);
    res = check_result(&out, res_same_s);
    DEAL_RESULT(res, "conv2d_same_stride");
                
    return 0;
}

#else 
#error "TM_MDL_TYPE error!"
#endif


