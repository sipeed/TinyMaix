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

#ifndef __TM_PORT_H
#define __TM_PORT_H

#define TM_ARCH_O0          (0) //default, optimize level 0
#define TM_ARCH_O1          (1)
#define TM_ARCH_O2          (2)
#define TM_ARCH_ARM_SIMD    (3) //seems 32bit simd not faster enough
#define TM_ARCH_ARM_NEON    (4) //
#define TM_ARCH_ARM_MVEI    (5) //ARMv8.1: M55, etc.
#define TM_ARCH_RV32P       (6) //
#define TM_ARCH_RV64V       (7)

/******************************* PORT CONFIG  ************************************/
#define TM_ARCH         TM_ARCH_O0
#define TM_MDL_TYPE     TM_MDL_INT8
#define TM_FASTSCALE    (0)         //enable if your chip don't have FPU, may speed up 1/3, but decrease accuracy
#define TM_ENABLE_STAT  (1)         //enable mdl stat functions
#define TM_MAX_CSIZE    (1000)      //max channel num //used if INT8 mdl  //cost TM_MAX_CSIZE*4 Byte
#define TM_MAX_KSIZE    (5*5)       //max kernel_size   //cost TM_MAX_KSIZE*4 Byte
#define TM_MAX_KCSIZE   (3*3*256)   //max kernel_size*channels //cost TM_MAX_KSIZE*sizeof(mtype_t) Byte

#define TM_INLINE       __attribute__((always_inline)) static inline
#define tm_malloc(x)    malloc(x)
#define tm_free(x)      free(x)


#define TM_PRINTF(...) printf(__VA_ARGS__)
#define TM_DBG(...)    //TM_PRINTF("###L%d: ",__LINE__);TM_PRINTF(__VA_ARGS__);
#define TM_DBGL()      TM_PRINTF("###L%d\n",__LINE__);


#include <sys/time.h>
#include <time.h>
#define  TM_GET_US()       ((uint32_t)(clock()*1000000/CLOCKS_PER_SEC))

#define TM_DBGT_INIT()     uint32_t _start,_finish;float _time;_start=TM_GET_US();
#define TM_DBGT_START()    _start=TM_GET_US();
#define TM_DBGT(x)         {_finish=TM_GET_US();\
                            _time = (float)(_finish-_start)/1000.0;\
                            TM_PRINTF("===%s use %.3f ms\n", (x), _time);\
                            _start=TM_GET_US();}

/******************************* OPS CONFIG  ************************************/




#endif
