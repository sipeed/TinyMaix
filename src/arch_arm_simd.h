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

//ARM SIMD instruction acceleration
//https://www.keil.com/pack/doc/CMSIS/Core/html/group__intrinsic__SIMD__gr.html

//sum = SUM(Ai*Bi)
TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{
    uint32_t cnt;                  
    int32_t sum = 0;
    cnt = size/4;

#ifdef __UVISION_VERSION //MDK,KEIL
		__asm(
#else
   asm  volatile(
#endif
			"DOT_LOOP%=: \n"
			"subs   %[cnt], %[cnt], #1   \n"
			"ldr    r2, [%[sptr]], #4    \n" 
			"ldr    r3, [%[kptr]], #4    \n" 
			"ror    r4, r2, #8      \n" 
			"ror    r5, r3, #8      \n" 
			"sxtb16 r2, r2          \n" 
			"sxtb16 r3, r3          \n" 
			"sxtb16 r4, r4          \n" 
			"sxtb16 r5, r5          \n" 
			"smlad  %[sum], r2, r3, %[sum]  \n" 
			"smlad  %[sum], r4, r5, %[sum]  \n" 
			"cmp    %[cnt], #0      \n"
			"bgt DOT_LOOP%=         \n"
			:[sptr]"+r"(sptr),[kptr]"+r"(kptr),[sum]"+r"(sum),[cnt]"+r"(cnt)
			:
			:"r2", "r3", "r4", "r5");
    cnt = size % 4;
    while (cnt){
        sum += (int32_t) ((int16_t) *sptr++ * *kptr++);
        cnt--;
    }
		*result = sum;
    return;
}
