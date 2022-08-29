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

#include "arm_mve.h"
//ARM MVEI instruction acceleration
//https://developer.arm.com/architectures/instruction-sets/intrinsics/
//Not Tested!!!

#if TM_MDL_TYPE==TM_MDL_INT8
#define PARALLEL_CNT 16
TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{
    uint32_t  cnt;           
    int8x16_t vecA;
    int8x16_t vecB;
    sumtype_t sum = 0;
    cnt = size/PARALLEL_CNT;
    while (cnt > 0) {
        vecA = vld1q_s8(sptr);
        vecB = vld1q_s8(kptr);
        sum = vmladavaq(sum, vecA, vecB);
        cnt--; sptr += PARALLEL_CNT; kptr += PARALLEL_CNT;
    }
    cnt = size%PARALLEL_CNT;
    if (cnt > 0U) {
        mve_pred16_t p0 = vctp8q(cnt);
        vecA = vld1q_s8(sptr);
        vecB = vld1q_s8(kptr);
        sum = vmladavaq_p(sum, vecA, vecB, p0);
    }
    *result = sum;
    return;
}

TM_INLINE void tm_dot_prod_gap_3x3x1(mtype_t* sptr, mtype_t* kptr, uint32_t* k_oft, sumtype_t* result)
{
    *result = sptr[k_oft[0]]*kptr[0] + sptr[k_oft[1]]*kptr[1] + sptr[k_oft[2]]*kptr[2] + \
        sptr[k_oft[3]]*kptr[3] + sptr[k_oft[4]]*kptr[4] + sptr[k_oft[5]]*kptr[5] + \
        sptr[k_oft[6]]*kptr[6] + sptr[k_oft[7]]*kptr[7] + sptr[k_oft[8]]*kptr[8] ;
    return;                  
}

#else
    #error "TODO"
#endif