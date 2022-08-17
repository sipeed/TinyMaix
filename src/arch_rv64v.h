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

#ifndef __riscv_vector
#error "Vector intrinsics require the vector extension."
#endif
#include <riscv_vector.h>

//RV64 V-extend acceleration
//https://github.com/T-head-Semi/openc906/tree/main/doc
//https://github.com/riscv-non-isa/rvv-intrinsic-doc
//https://occ.t-head.cn/community/download?id=4073475960903634944
//https://github.com/riscv/riscv-v-spec
//Use this toolchain support rvv inntrinsic: Xuantie-900-gcc-elf-newlib-x86_64-V2.2.4-20211227.tar.gz
//-march=rv64gcv0p7_zfh_xtheadc 
//more information refer to: 
//https://occ-oss-prod.oss-cn-hangzhou.aliyuncs.com/resource//1659318003104/Xuantie+900+Series+RVV-0.7.1+Intrinsic+Manual.pdf
//https://riscv.org/wp-content/uploads/2019/06/17.40-Vector_RISCV-20190611-Vectors.pdf
//Total 32 vector registers
//ELEN: Size of largest element in bits
//VLEN: Number of bits in each vector register; VLEN>=ELEN
//SLEN: Striping distance in bits; VLEN >= SLEN >= ELEN
//SEW:  Standard element width, =8,16,32,...,1024
//LMUL: Vector length multiplier, =1,2,4,8  (LMUL registers at the same time)
//EDIV: Vector element divider, =1,2,4,8
//vtype: Vtype sets width of element in each vector register (e.g., 16-bit, 32-bit, …)
//vl: Vector length CSR sets number of elements active in each instruction
//vstart: Resumption element after trap
//fcsr(vxrm/vxsat): Fixed-point rounding mode and saturation flag fields in FP CSR


#define RVV_VLEN 128  //TODO: read csrr_vlenb

/********************************** TM_MDL_FP32 ***********************************************/
#if TM_MDL_TYPE==TM_MDL_FP32
#define PACK_N (RVV_VLEN/32)    //fp32
TM_INLINE  void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{ 
    float sumbuf[PACK_N];
    float sum = 0.f;
    int cnt=size/PACK_N;
    if(cnt>0){
        size_t vl = vsetvl_e32m1(PACK_N); 
        vfloat32m1_t sumv = vfmv_v_f_f32m1(0.f, vl);   //set sum=0
        vfloat32m1_t v_zero = vfmv_v_f_f32m1(0.f, vl);
        for(int i=0; i<cnt; i++){
            vfloat32m1_t s = vle32_v_f32m1(sptr, vl);
            vfloat32m1_t k = vle32_v_f32m1(kptr, vl);
            sumv = vfmacc_vv_f32m1(sumv, s, k, vl);
            sptr += PACK_N;
            kptr += PACK_N;
        }
        v_zero = vfredosum_vs_f32m1_f32m1(v_zero, sumv, v_zero, vl);
        sum = vfmv_f_s_f32m1_f32(v_zero);
    }
    for(int i=0; i<size%PACK_N; i++){
        sum += *sptr * *kptr;
        sptr++;kptr++;
    }

    *result = sum;
    return;
}

/********************************** TM_MDL_FP16 ***********************************************/
#elif TM_MDL_TYPE==TM_MDL_FP16
#define PACK_N (RVV_VLEN/16)    //fp16, packn=8
TM_INLINE  void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{ 
    float16_t sumbuf[PACK_N];
    float16_t sum = 0;
    int cnt=size/PACK_N;

    if(cnt>0){
        size_t vl = vsetvl_e16m1(PACK_N); 
        vfloat16m1_t sumv = vfmv_v_f_f16m1(0.f, vl);   //set sum=0
        vfloat16m1_t v_zero = vfmv_v_f_f16m1(0.0f, vl);        
        for(int i=0; i<cnt; i++){
            vfloat16m1_t s = vle16_v_f16m1(sptr, vl);
            vfloat16m1_t k = vle16_v_f16m1(kptr, vl);
            sumv = vfmacc_vv_f16m1(sumv, s, k, vl);
            sptr += PACK_N;
            kptr += PACK_N;
        }
        v_zero = vfredosum_vs_f16m1_f16m1(v_zero, sumv, v_zero, vl);
        sum = vfmv_f_s_f16m1_f16(v_zero);
    }
    for(int i=0; i<size%PACK_N; i++){
        sum += *sptr * *kptr;
        sptr++;kptr++;
    }

    *result = sum;
    return;
}

/********************************** TM_MDL_INT8 ***********************************************/
#elif TM_MDL_TYPE==TM_MDL_INT8
#define PACK_N (RVV_VLEN/8)    //int8  16
//i8 to i16 (Widening Floating-Point/Integer Type-Convert Operations)
//  vint16m2_t vwcvt_x_x_v_i16m2 (vint8m1_t src, size_t vl);
//Vector Widening Integer Multiply-Add Operations
//  vint32m4_t vwmacc_vv_i32m4 (vint32m4_t vd, vint16m2_t vs1, vint16m2_t vs2, size_t vl);
TM_INLINE  void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{ 
    int32_t sumbuf[PACK_N];
    int32_t sum = 0;
    int cnt=size/PACK_N;
    if(cnt>0){
        size_t vl = vsetvl_e8m1(PACK_N); 
        vint32m4_t sumv = vmv_v_x_i32m4(0, vl);         //set sum=0
        vint32m1_t v_zero = vmv_v_x_i32m1(0, vl);  
        for(int i=0; i<cnt; i++){
            vint8m1_t  s8  = vle8_v_i8m1(sptr, vl);     //load i8
            vint8m1_t  k8  = vle8_v_i8m1(kptr, vl);
            vint16m2_t s16 = vwcvt_x_x_v_i16m2(s8,vl);  //cvt i8 to i16
            vint16m2_t k16 = vwcvt_x_x_v_i16m2(k8,vl);
            sumv = vwmacc_vv_i32m4(sumv, s16, k16, vl); //mac
            sptr += PACK_N;
            kptr += PACK_N;
        }
        v_zero = vredsum_vs_i32m4_i32m1(v_zero, sumv, v_zero, vl);
        sum = vmv_x_s_i32m1_i32(v_zero);
    }
    for(int i=0; i<size%PACK_N; i++){
        sum += *sptr * *kptr;
        sptr++;kptr++;
    }

    *result = sum;
    return;
}

#else
#error "ERR MDL TYPE"
#endif