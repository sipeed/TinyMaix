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

#include "emmintrin.h"

/********************************** TM_MDL_INT8 ***********************************************/
#if TM_MDL_TYPE==TM_MDL_INT8
TM_INLINE int _mm_reduce_add_epi32(__m128i x)
{
    __m128i hi64 = _mm_unpackhi_epi64(x, x);
    __m128i sum64 = _mm_add_epi32(hi64, x);
    __m128i hi32 = _mm_shuffle_epi32(sum64, _MM_SHUFFLE(2, 3, 0, 1));
    __m128i sum32 = _mm_add_epi32(sum64, hi32);
    return _mm_cvtsi128_si32(sum32);
}

TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{
    uint32_t i = 0;
    sumtype_t sum = 0;

    __m128i _sum0 = _mm_setzero_si128();
    __m128i _sum1 = _mm_setzero_si128();
    for(; i + 15 < size; i += 16) {
        __m128i s8 = _mm_loadu_si128((const __m128i*)sptr);
        __m128i k8 = _mm_loadu_si128((const __m128i*)kptr);
        __m128i _exts8 = _mm_cmpgt_epi8(_mm_setzero_si128(), s8);
        __m128i _extk8 = _mm_cmpgt_epi8(_mm_setzero_si128(), k8);
        __m128i s16l = _mm_unpacklo_epi8(s8, _exts8);
        __m128i s16h = _mm_unpackhi_epi8(s8, _exts8);
        __m128i k16l = _mm_unpacklo_epi8(k8, _extk8);
        __m128i k16h = _mm_unpackhi_epi8(k8, _extk8);
        _sum0 = _mm_add_epi32(_sum0, _mm_madd_epi16(s16l, k16l));
        _sum1 = _mm_add_epi32(_sum1, _mm_madd_epi16(s16h, k16h));
        sptr += 16;
        kptr += 16;
    }
    for(; i + 7 < size; i += 8) {
        __m128i s8 = _mm_loadl_epi64((const __m128i*)sptr);
        __m128i k8 = _mm_loadl_epi64((const __m128i*)kptr);
        __m128i _exts8 = _mm_cmpgt_epi8(_mm_setzero_si128(), s8);
        __m128i _extk8 = _mm_cmpgt_epi8(_mm_setzero_si128(), k8);
        __m128i s16 = _mm_unpacklo_epi8(s8, _exts8);
        __m128i k16 = _mm_unpacklo_epi8(k8, _extk8);
        _sum0 = _mm_add_epi32(_sum0, _mm_madd_epi16(s16, k16));
        sptr += 8;
        kptr += 8;
    }
    sum += _mm_reduce_add_epi32(_mm_add_epi32(_sum0, _sum1));

    for(; i < size; i++) {
        sum += (int32_t) ((int16_t) *sptr++ * *kptr++);
    }

    *result = sum;
    return;
}

TM_INLINE  void tm_dot_prod_pack2(mtype_t* sptr, mtype_t* kptr, uint32_t size, sumtype_t* result)
{
    uint32_t i = 0;
    sumtype_t sum0 = 0;
    sumtype_t sum1 = 0;
    mtype_t* kptr0 = kptr;
    mtype_t* kptr1 = kptr+size;

    __m128i _sum0 = _mm_setzero_si128();
    __m128i _sum1 = _mm_setzero_si128();
    for(; i + 15 < size; i += 16) {
        __m128i s8 = _mm_loadu_si128((const __m128i*)sptr);
        __m128i k80 = _mm_loadu_si128((const __m128i*)kptr0);
        __m128i k81 = _mm_loadu_si128((const __m128i*)kptr1);
        __m128i _exts8 = _mm_cmpgt_epi8(_mm_setzero_si128(), s8);
        __m128i _extk80 = _mm_cmpgt_epi8(_mm_setzero_si128(), k80);
        __m128i _extk81 = _mm_cmpgt_epi8(_mm_setzero_si128(), k81);
        __m128i s16l = _mm_unpacklo_epi8(s8, _exts8);
        __m128i s16h = _mm_unpackhi_epi8(s8, _exts8);
        __m128i k160l = _mm_unpacklo_epi8(k80, _extk80);
        __m128i k160h = _mm_unpackhi_epi8(k80, _extk80);
        __m128i k161l = _mm_unpacklo_epi8(k81, _extk81);
        __m128i k161h = _mm_unpackhi_epi8(k81, _extk81);
        _sum0 = _mm_add_epi32(_sum0, _mm_madd_epi16(s16l, k160l));
        _sum1 = _mm_add_epi32(_sum1, _mm_madd_epi16(s16l, k161l));
        _sum0 = _mm_add_epi32(_sum0, _mm_madd_epi16(s16h, k160h));
        _sum1 = _mm_add_epi32(_sum1, _mm_madd_epi16(s16h, k161h));
        sptr += 16;
        kptr0 += 16;
        kptr1 += 16;
    }
    for(; i + 7 < size; i += 8) {
        __m128i s8 = _mm_loadl_epi64((const __m128i*)sptr);
        __m128i k80 = _mm_loadl_epi64((const __m128i*)kptr0);
        __m128i k81 = _mm_loadl_epi64((const __m128i*)kptr1);
        __m128i _exts8 = _mm_cmpgt_epi8(_mm_setzero_si128(), s8);
        __m128i _extk80 = _mm_cmpgt_epi8(_mm_setzero_si128(), k80);
        __m128i _extk81 = _mm_cmpgt_epi8(_mm_setzero_si128(), k81);
        __m128i s16 = _mm_unpacklo_epi8(s8, _exts8);
        __m128i k160 = _mm_unpacklo_epi8(k80, _extk80);
        __m128i k161 = _mm_unpacklo_epi8(k81, _extk81);
        _sum0 = _mm_add_epi32(_sum0, _mm_madd_epi16(s16, k160));
        _sum1 = _mm_add_epi32(_sum1, _mm_madd_epi16(s16, k161));
        sptr += 8;
        kptr0 += 8;
        kptr1 += 8;
    }
    sum0 += _mm_reduce_add_epi32(_sum0);
    sum1 += _mm_reduce_add_epi32(_sum1);

    for(; i < size; i++) {
        sum0 += (int32_t) ((int16_t) *sptr * *kptr0++);
        sum1 += (int32_t) ((int16_t) *sptr++ * *kptr1++);
    }

    result[0] = sum0;
    result[1] = sum1;
    return;
}

TM_INLINE void tm_dot_prod_gap_3x3x1(mtype_t* sptr, mtype_t* kptr, uint32_t* k_oft, sumtype_t* result)
{
    *result = sptr[k_oft[0]]*kptr[0] + sptr[k_oft[1]]*kptr[1] + sptr[k_oft[2]]*kptr[2] + \
        sptr[k_oft[3]]*kptr[3] + sptr[k_oft[4]]*kptr[4] + sptr[k_oft[5]]*kptr[5] + \
        sptr[k_oft[6]]*kptr[6] + sptr[k_oft[7]]*kptr[7] + sptr[k_oft[8]]*kptr[8] ;
    return;
}

TM_INLINE void tm_dot_prod_3x3x1(mtype_t* sptr, mtype_t* kptr, sumtype_t* result)
{
    *result = sptr[0]*kptr[0] + sptr[1]*kptr[1] + sptr[2]*kptr[2] + \
        sptr[3]*kptr[3] + sptr[4]*kptr[4] + sptr[5]*kptr[5] + \
        sptr[6]*kptr[6] + sptr[7]*kptr[7] + sptr[8]*kptr[8] ;
    return;
}

/********************************** TM_MDL_FP32 ***********************************************/
#elif TM_MDL_TYPE==TM_MDL_FP32
TM_INLINE float _mm_reduce_add_ps(__m128 x128)
{
    const __m128 x64 = _mm_add_ps(x128, _mm_movehl_ps(x128, x128));
    const __m128 x32 = _mm_add_ss(x64, _mm_shuffle_ps(x64, x64, 0x55));
    return _mm_cvtss_f32(x32);
}

TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{
    uint32_t i = 0;
    sumtype_t sum = 0;

    __m128 _sum = _mm_setzero_ps();
    for(; i + 3 < size; i += 4) {
        __m128 s = _mm_loadu_ps(sptr);
        __m128 k = _mm_loadu_ps(kptr);
        _sum = _mm_add_ps(_mm_mul_ps(s, k), _sum);
        sptr += 4;
        kptr += 4;
    }
    sum += _mm_reduce_add_ps(_sum);

    for(; i < size; i++) {
        sum += (int32_t) ((int16_t) *sptr++ * *kptr++);
    }

    *result = sum;
    return;
}

TM_INLINE  void tm_dot_prod_pack2(mtype_t* sptr, mtype_t* kptr, uint32_t size, sumtype_t* result)
{
    uint32_t i = 0;
    sumtype_t sum0 = 0;
    sumtype_t sum1 = 0;
    mtype_t* kptr0 = kptr;
    mtype_t* kptr1 = kptr+size;

    __m128 _sum0 = _mm_setzero_ps();
    __m128 _sum1 = _mm_setzero_ps();
    for(; i + 3 < size; i += 4) {
        __m128 s = _mm_loadu_ps(sptr);
        __m128 k0 = _mm_loadu_ps(kptr0);
        __m128 k1 = _mm_loadu_ps(kptr1);
        _sum0 = _mm_add_ps(_mm_mul_ps(s, k0), _sum0);
        _sum1 = _mm_add_ps(_mm_mul_ps(s, k1), _sum1);
        sptr += 4;
        kptr0 += 4;
        kptr1 += 4;
    }
    sum0 += _mm_reduce_add_ps(_sum0);
    sum1 += _mm_reduce_add_ps(_sum1);

    for(; i < size; i++) {
        sum0 +=  (*sptr * *kptr0++);
        sum1 +=  (*sptr++ * *kptr1++);
    }

    result[0] = sum0;
    result[1] = sum1;
    return;
}

TM_INLINE void tm_dot_prod_gap_3x3x1(mtype_t* sptr, mtype_t* kptr, uint32_t* k_oft, sumtype_t* result)
{
    *result = sptr[k_oft[0]]*kptr[0] + sptr[k_oft[1]]*kptr[1] + sptr[k_oft[2]]*kptr[2] + \
        sptr[k_oft[3]]*kptr[3] + sptr[k_oft[4]]*kptr[4] + sptr[k_oft[5]]*kptr[5] + \
        sptr[k_oft[6]]*kptr[6] + sptr[k_oft[7]]*kptr[7] + sptr[k_oft[8]]*kptr[8] ;
    return;
}

TM_INLINE void tm_dot_prod_3x3x1(mtype_t* sptr, mtype_t* kptr, sumtype_t* result)
{
    *result = sptr[0]*kptr[0] + sptr[1]*kptr[1] + sptr[2]*kptr[2] + \
        sptr[3]*kptr[3] + sptr[4]*kptr[4] + sptr[5]*kptr[5] + \
        sptr[6]*kptr[6] + sptr[7]*kptr[7] + sptr[8]*kptr[8] ;
    return;
}

#else
    #error "TODO"
#endif

#if (TM_MDL_TYPE==TM_MDL_FP32) || (TM_MDL_TYPE==TM_MDL_FP16) 

TM_INLINE void tm_postprocess_sum(int n, sumtype_t* sums, btype_t* bs, int act, mtype_t* outp, \
    sctype_t* scales, sctype_t out_s, zptype_t out_zp)
{
    for(int i = 0; i < n; i++) {
        sumtype_t sum = sums[i];
        sum += bs[i];
        switch(act){    //activation func
        case TM_ACT_RELU:
        case TM_ACT_RELU6: //treat relu6 as relu in float mode //speed up
            sum = sum>0?sum:0;
            break;
        //    sum = sum>0?sum:0;
        //    sum = sum>6?6:sum;
        //    break;
        default:
            break;
        }
        outp[i] = (mtype_t)sum;
    }
    return;
}

#elif (TM_MDL_TYPE==TM_MDL_INT8) || (TM_MDL_TYPE==TM_MDL_INT16) 

#if !TM_FASTSCALE
TM_INLINE void tm_postprocess_sum(int n, sumtype_t* sums, btype_t* bs, int act, mtype_t* outp, sctype_t* scales, sctype_t out_s_inv, zptype_t out_zp)
#else
TM_INLINE void tm_postprocess_sum(int n, sumtype_t* sums, btype_t* bs, int act, mtype_t* outp, int32_t* scales, int32_t out_s, zptype_t out_zp)
#endif
{
    for(int i = 0; i < n; i++) {
        sumtype_t sum = sums[i];
        sum += bs[i];
        #if !TM_FASTSCALE
            float sumf = sum*scales[i];
        #else 
            sumtype_t sumf = (sum<<TM_FASTSCALE_SHIFT)/scales[i];
        #endif
        switch(act){    //activation func
        case TM_ACT_RELU:
            sumf = sumf>0?sumf:0;
            break;
        case TM_ACT_RELU6:
            sumf = sumf>0?sumf:0;
        #if (!TM_FASTSCALE)
            sumf = sumf>6?6:sumf;
        #else
            sumf = sumf>(6<<TM_FASTSCALE_SHIFT)?(6<<TM_FASTSCALE_SHIFT):sumf;
        #endif
            break;
        default:
            break;
        }
        #if !TM_FASTSCALE
            outp[i] = (mtype_t)(sumf*out_s_inv + out_zp);  //(mtype_t)((int)(sumf/out_s) + out_zp) //(mtype_t)((int)(sumf/out_s +0.5) + out_zp)
        #else 
            outp[i] = (mtype_t)(((sumf*out_s)>>(TM_FASTSCALE_SHIFT+TM_FASTSCALE_SHIFT))+out_zp);
        #endif
    }
    return;
}
#endif
