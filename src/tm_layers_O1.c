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
// It is O1 implement
#include "tinymaix.h"
#include "float.h"
#include "math.h"

#if TM_OPT_LEVEL == TM_OPT1

#if TM_ARCH==TM_ARCH_CPU
    #include "arch_cpu.h"
#elif TM_ARCH==TM_ARCH_ARM_SIMD
    #include "arch_arm_simd.h"
#elif TM_ARCH==TM_ARCH_ARM_NEON
    #include "arch_arm_neon.h"
#elif TM_ARCH==TM_ARCH_ARM_MVEI
    #include "arch_arm_mvei.h"
#elif TM_ARCH==TM_ARCH_RV32P
    #include "arch_rv32p.h"
#elif TM_ARCH==TM_ARCH_RV64V
    #include "arch_rv64v.h"
#else
    #error "UNSUPPORT ARCH!"
#endif


#if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16)
    #error "O1 not support int8/16 now!"
#endif


#error "TODO... we will speed up 30% compare to O0"


#endif 

