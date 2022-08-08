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

#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "tm_port.h"

//sum = SUM(Ai*Bi)
TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{
    sumtype_t sum=0;
    for(int i=0; i <size; i++){
        sum += sptr[i]*kptr[i];
    }
    *result = sum;
    return;
}

//TODO: arm gcc inline slower than macro?
/*
#define tm_dot_prod_gap(sptr,kptr,ksize,csize,k_oft,sbuf,result) \
    {\
        for (int _cc = 0; _cc < (csize); _cc++) {\
            for (int _k = 0; _k < (ksize); _k++) {\
                sumtype_t val = (sumtype_t)(sptr)[(k_oft)[_k]];\
                sumtype_t wt  = (sumtype_t)(kptr)[_k];\
                (*(result)) += val * wt;\
            }\
            (kptr) += (ksize);\
            (sptr) += 1;\
        }\
    }
*/ 


