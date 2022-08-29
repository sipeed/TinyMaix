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
// warning: O1 code bloat much to get better performance, experimental now
/*
pwconv
dwconv
    3x3：tm_dot_prod_gap_3x3x1
    none 3x3: 
mdwconv
conv
*/

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


/*************************** TML_CONV2D **********************************/
static uint32_t k_oft[TM_MAX_KSIZE]; 
static mtype_t sbuf[TM_MAX_KCSIZE]; 
#if (TM_MDL_TYPE==TM_MDL_FP32) || (TM_MDL_TYPE==TM_MDL_FP16) 
#define SUMSCALE (1.0)
static sctype_t outscale;
TM_INLINE void l_postprocess_sum(sumtype_t sum, btype_t b, int act, mtype_t* outp, \
    sctype_t scale, sctype_t out_s, zptype_t out_zp)
{   sum += b;
    switch(act){    //activation func
    case TM_ACT_RELU:
        sum = sum>0?sum:0;
        break;
    case TM_ACT_RELU6:
        sum = sum>0?sum:0;
        sum = sum>6?6:sum;
        break;
    default:
        break;
    }
    *outp = (mtype_t)sum;
    return;
}
#elif (TM_MDL_TYPE==TM_MDL_INT8) || (TM_MDL_TYPE==TM_MDL_INT16) 

#if TM_FASTSCALE
    static int32_t sumscale[TM_MAX_CSIZE];
    static int32_t outscale;
#else
    static float sumscale[TM_MAX_CSIZE];
    static sctype_t outscale;
#endif
#define SUMSCALE (sumscale[c])

#if !TM_FASTSCALE
TM_INLINE void l_postprocess_sum(sumtype_t sum, btype_t b, int act, mtype_t* outp, sctype_t scale, sctype_t out_s, zptype_t out_zp)
#else
TM_INLINE void l_postprocess_sum(sumtype_t sum, btype_t b, int act, mtype_t* outp, int32_t scale, int32_t out_s, zptype_t out_zp)
#endif
{   sum += b;
    #if !TM_FASTSCALE
        float sumf = sum*scale;
    #else 
        sumtype_t sumf = (sum<<TM_FASTSCALE_SHIFT)/scale;
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
        *outp = (mtype_t)(sumf/out_s + out_zp);  //(mtype_t)((int)(sumf/out_s) + out_zp) //(mtype_t)((int)(sumf/out_s +0.5) + out_zp)
    #else 
        *outp = (mtype_t)(((sumf*out_s)>>(TM_FASTSCALE_SHIFT+TM_FASTSCALE_SHIFT))+out_zp);
    #endif
    return;
}
#endif
 
//1x1 pw conv
TM_INLINE tm_err_t l_tml_pwconv2d(tm_mat_t* in, tm_mat_t* out, wtype_t* w, btype_t* b, \
    int kw, int kh, int sx, int sy, int dx, int dy, int act, \
    int pad_top, int pad_bottom, int pad_left, int pad_right, int dmul, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp) 
{   TM_PERF_INIT();TM_PERF_REG(t_sbuf);TM_PERF_REG(t_dotp);TM_PERF_REG(t_post);
    int pad_flag = (pad_top != 0 ||pad_bottom != 0 ||pad_left != 0 ||pad_right != 0);
    int maxk = kw*kh;
    int chi  = in->c; 
    int cho  = out->c;
    sumtype_t sum; 
    mtype_t* outp = out->data;

    for (int y = 0; y < out->h; y++) {
        for (int x = 0; x < out->w; x++) {
            mtype_t* sptr = (mtype_t*)TM_MATP(in, sy*y, sx*x, 0); 
            wtype_t* kptr = (wtype_t*)w;
            for(int c=0; c<out->c; c++){
                sum = 0 ; TM_PERF_START();
                tm_dot_prod(sptr, kptr, chi, &sum); TM_PERF_ADD(t_dotp);//size=maxk*chi //pw maxk==1
                l_postprocess_sum(sum, b[c], act, outp, SUMSCALE, outscale, out_zp); outp++; TM_PERF_ADD(t_post);
                kptr += chi;
            }
        }
    }
    TM_PERF_PRINT(t_sbuf);TM_PERF_PRINT(t_dotp);TM_PERF_PRINT(t_post);
    return TM_OK;
}

//normal conv
TM_INLINE tm_err_t l_tml_conv2d(tm_mat_t* in, tm_mat_t* out, wtype_t* w, btype_t* b, \
    int kw, int kh, int sx, int sy, int dx, int dy, int act, \
    int pad_top, int pad_bottom, int pad_left, int pad_right, int dmul, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp) 
{   TM_PERF_INIT();TM_PERF_REG(t_sbuf);TM_PERF_REG(t_dotp);TM_PERF_REG(t_post);
    int pad_flag = (pad_top != 0 ||pad_bottom != 0 ||pad_left != 0 ||pad_right != 0);
    int maxk = kw*kh;
    int chi  = in->c; 
    int cho  = out->c;
    sumtype_t sum; 
    mtype_t* outp = out->data;

    int oft = 0;
    int idx = 0;
    for(int y=0; y<kh; y++){    //gen k_oft table
        for(int x=0; x<kw; x++){
            k_oft[idx] = oft;
            idx += 1;
            oft += chi;
        }
        oft += (in->w - kw)*chi; 
    }
    int slow_flag = 0; //same pad part is slow

    for (int y = 0; y < out->h; y++) {
        int src_y0 = sy*y - pad_top;
        for (int x = 0; x < out->w; x++) {
            int src_x0 = sx*x - pad_left;
            sumtype_t sum; 
            slow_flag = ((src_y0<0)+(src_x0<0)+(src_y0+kh>in->h)+(src_x0+kw>in->w)); 
            TM_PERF_START();
            if(!slow_flag) {//valid or same valid part
                mtype_t* sptr_base = (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); //?c/dmul:0
                mtype_t* sptr = sptr_base; //= (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); //sbuf 不变
                uint32_t sidx=0;    //sbuf:cho,chi,maxk //dw:chi==1;
                for (int cc = 0; cc < chi; cc++) {
                    for (int k = 0; k < maxk; k++) {
                        sbuf[sidx+k] = sptr[k_oft[k]];
                    }
                    sidx += maxk;
                    sptr += 1;
                }
            } else {        //same pad part
                int _ky0 = src_y0<0 ? -src_y0 : 0;
                int _kx0 = src_x0<0 ? -src_x0 : 0;
                int _ky1 = in->h-src_y0>kh ? kh : in->h-src_y0;
                int _kx1 = in->w-src_x0>kw ? kw : in->w-src_x0;
                uint32_t sidx=0;    //sbuf:cho,chi,maxk //dw:chi==1;
                uint32_t s_step = (_ky1-_ky0)*(_kx1-_kx0);
                mtype_t* sptr_base = (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); 
                mtype_t* sptr = sptr_base;
            #if TM_MDL_TYPE == TM_MDL_INT8
                memset(sbuf, in_zp, chi*maxk);    //do padding
            #elif (TM_MDL_TYPE == TM_MDL_FP32)||(TM_MDL_TYPE == TM_MDL_FP16)
                memset(sbuf, 0, chi*maxk*sizeof(mtype_t));
            #else
            #error "unsupport mdl type"
            #endif
                for (int cc = 0; cc < chi; cc++) {
                    for(int _ky=_ky0; _ky<_ky1; _ky++){
                        for(int _kx=_kx0; _kx<_kx1; _kx++){
                            int k = _ky*kw + _kx;
                            sbuf[sidx+k] = sptr[k_oft[k]];
                        }
                    }
                    sidx += maxk;
                    sptr += 1;
                }
            }
            TM_PERF_ADD(t_sbuf);
            mtype_t* sptr = sbuf;    //sbuf prepare ok~
            for(int c=0; c<out->c; c++){
                sum = 0;
                wtype_t* kptr = (wtype_t*)w + c*chi*maxk;TM_PERF_START();
                tm_dot_prod(sptr, kptr, maxk*chi, &sum);TM_PERF_ADD(t_dotp);
                l_postprocess_sum(sum, b[c], act, outp, SUMSCALE, outscale, out_zp); outp++;TM_PERF_ADD(t_post);
            }
        }
    } 
    TM_PERF_PRINT(t_sbuf);TM_PERF_PRINT(t_dotp);TM_PERF_PRINT(t_post);
    return TM_OK;
}

//dwconv
TM_INLINE tm_err_t l_tml_dwconv2d(tm_mat_t* in, tm_mat_t* out, wtype_t* w, btype_t* b, \
    int kw, int kh, int sx, int sy, int dx, int dy, int act, \
    int pad_top, int pad_bottom, int pad_left, int pad_right, int dmul, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp) 
{   TM_PERF_INIT();TM_PERF_REG(t_sbuf);TM_PERF_REG(t_dotp);TM_PERF_REG(t_post);
    int pad_flag = (pad_top != 0 ||pad_bottom != 0 ||pad_left != 0 ||pad_right != 0);
    int maxk = kw*kh;
    int chi  = in->c; 
    int cho  = out->c;
    sumtype_t sum; 
    mtype_t* outp = out->data;

    int oft = 0;
    int idx = 0;
    for(int y=0; y<kh; y++){    //gen k_oft table
        for(int x=0; x<kw; x++){
            k_oft[idx] = oft;
            idx += 1;
            oft += chi;
        }
        oft += (in->w - kw)*chi; 
    }
    int slow_flag = 0; //same pad part is slow

    for (int y = 0; y < out->h; y++) {
        int src_y0 = sy*y - pad_top;
        for (int x = 0; x < out->w; x++) {
            int src_x0 = sx*x - pad_left;
            sumtype_t sum; 
            slow_flag = ((src_y0<0)+(src_x0<0)+(src_y0+kh>in->h)+(src_x0+kw>in->w)); 
            TM_PERF_START();
            if(!slow_flag) {//valid or same valid part
                mtype_t* sptr_base = (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); //?c/dmul:0
                mtype_t* sptr = sptr_base; //= (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); //sbuf 不变
                if(maxk==9){             
                    for (int c = 0; c < cho; c++) {
                        wtype_t* kptr = (wtype_t*)w + c*9;
                        tm_dot_prod_gap_3x3x1(sptr, kptr, k_oft, &sum);
                        l_postprocess_sum(sum, b[c], act, outp, SUMSCALE, outscale, out_zp); outp++;
                        sptr += 1;
                    }
                } else {
                    uint32_t sidx = 0;
                    for (int c = 0; c < cho; c++) {
                        for (int k = 0; k < maxk; k++) {
                            sbuf[sidx+k] = sptr[k_oft[k]];
                        }
                        sidx += maxk;
                        sptr += 1;
                    }
                    sptr = sbuf;    //sbuf prepare ok~
                    for(int c=0; c<out->c; c++){
                        sum = 0;
                        wtype_t* kptr = (wtype_t*)w + c*maxk;TM_PERF_START();
                        tm_dot_prod(sptr, kptr, maxk, &sum);
                        //sum = sptr[0]*kptr[0] + sptr[1]*kptr[1] + sptr[2]*kptr[2] + sptr[3]*kptr[3] + sptr[4]*kptr[4] + sptr[5]*kptr[5] + sptr[6]*kptr[6] + sptr[7]*kptr[7] + sptr[8]*kptr[8] ;
                        TM_PERF_ADD(t_dotp);
                        l_postprocess_sum(sum, b[c], act, outp, SUMSCALE, outscale, out_zp); outp++;TM_PERF_ADD(t_post);
                        sptr += maxk; //dwconv need move step
                    }
                }
            } else {        //same pad part
                int _ky0 = src_y0<0 ? -src_y0 : 0;
                int _kx0 = src_x0<0 ? -src_x0 : 0;
                int _ky1 = in->h-src_y0>kh ? kh : in->h-src_y0;
                int _kx1 = in->w-src_x0>kw ? kw : in->w-src_x0;
                uint32_t sidx=0;    //sbuf:cho,chi,maxk //dw:chi==1;
                uint32_t s_step = (_ky1-_ky0)*(_kx1-_kx0);
                mtype_t* sptr_base = (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); 
                mtype_t* sptr = sptr_base;
            #if TM_MDL_TYPE == TM_MDL_INT8
                memset(sbuf, in_zp, cho*maxk);    //do padding
            #elif (TM_MDL_TYPE == TM_MDL_FP32)||(TM_MDL_TYPE == TM_MDL_FP16)
                memset(sbuf, 0, cho*maxk*sizeof(mtype_t));
            #else
            #error "unsupport mdl type"
            #endif
                for (int cc = 0; cc < cho; cc++) {
                    for(int _ky=_ky0; _ky<_ky1; _ky++){
                        for(int _kx=_kx0; _kx<_kx1; _kx++){
                            int k = _ky*kw + _kx;
                            sbuf[sidx+k] = sptr[k_oft[k]];
                        }
                    }
                    sidx += maxk;
                    sptr += 1;
                }
                sptr = sbuf;    //sbuf prepare ok~
                for(int c=0; c<out->c; c++){
                    sum = 0;
                    wtype_t* kptr = (wtype_t*)w + c*maxk;TM_PERF_START();
                    tm_dot_prod(sptr, kptr, maxk, &sum);
                    //sum = sptr[0]*kptr[0] + sptr[1]*kptr[1] + sptr[2]*kptr[2] + sptr[3]*kptr[3] + sptr[4]*kptr[4] + sptr[5]*kptr[5] + sptr[6]*kptr[6] + sptr[7]*kptr[7] + sptr[8]*kptr[8] ;
                    TM_PERF_ADD(t_dotp);
                    l_postprocess_sum(sum, b[c], act, outp, SUMSCALE, outscale, out_zp); outp++;TM_PERF_ADD(t_post);
                    sptr += maxk; //dwconv need move step
                }
            }
        }
    } 
    TM_PERF_PRINT(t_sbuf);TM_PERF_PRINT(t_dotp);TM_PERF_PRINT(t_post);
    return TM_OK;
}

//dmul>1 dwconv //not opt yet
TM_INLINE tm_err_t l_tml_mdwconv2d(tm_mat_t* in, tm_mat_t* out, wtype_t* w, btype_t* b, \
    int kw, int kh, int sx, int sy, int dx, int dy, int act, \
    int pad_top, int pad_bottom, int pad_left, int pad_right, int dmul, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp) 
{   TM_PERF_INIT();TM_PERF_REG(t_sbuf);TM_PERF_REG(t_dotp);TM_PERF_REG(t_post);
    int pad_flag = (pad_top != 0 ||pad_bottom != 0 ||pad_left != 0 ||pad_right != 0);
    int maxk = kw*kh;
    int chi  = in->c; 
    int cho  = out->c;
    sumtype_t sum; 
    mtype_t* outp = out->data;

    int oft = 0;
    int idx = 0;
    for(int y=0; y<kh; y++){    //gen k_oft table
        for(int x=0; x<kw; x++){
            k_oft[idx] = oft;
            idx += 1;
            oft += chi;
        }
        oft += (in->w - kw)*chi; 
    }
    int slow_flag = 0; //same pad part is slow

    for (int y = 0; y < out->h; y++) {
        int src_y0 = sy*y - pad_top;
        for (int x = 0; x < out->w; x++) {
            int src_x0 = sx*x - pad_left;
            sumtype_t sum; 
            slow_flag = ((src_y0<0)+(src_x0<0)+(src_y0+kh>in->h)+(src_x0+kw>in->w)); 
            TM_PERF_START();
            if(!slow_flag) {//valid or same valid part
                mtype_t* sptr_base = (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); //?c/dmul:0
                mtype_t* sptr = sptr_base; //= (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); //sbuf 不变
                uint32_t sidx=0;    //sbuf:cho,chi,maxk //dw:chi==1;
                for (int cc = 0; cc < cho; cc++) {
                    for (int k = 0; k < maxk; k++) {
                        sbuf[sidx+k] = sptr[k_oft[k]];
                    }
                    sidx += maxk;
                    sptr = sptr_base + (cc+1)/dmul;
                }
            } else {        //same pad part
                int _ky0 = src_y0<0 ? -src_y0 : 0;
                int _kx0 = src_x0<0 ? -src_x0 : 0;
                int _ky1 = in->h-src_y0>kh ? kh : in->h-src_y0;
                int _kx1 = in->w-src_x0>kw ? kw : in->w-src_x0;
                uint32_t sidx=0;    //sbuf:cho,chi,maxk //dw:chi==1;
                uint32_t s_step = (_ky1-_ky0)*(_kx1-_kx0);
                mtype_t* sptr_base = (mtype_t*)TM_MATP(in, src_y0, src_x0, 0); 
                mtype_t* sptr = sptr_base;
            #if TM_MDL_TYPE == TM_MDL_INT8
                memset(sbuf, in_zp, cho*maxk);    //do padding
            #elif (TM_MDL_TYPE == TM_MDL_FP32)||(TM_MDL_TYPE == TM_MDL_FP16)
                memset(sbuf, 0, cho*maxk*sizeof(mtype_t));
            #else
            #error "unsupport mdl type"
            #endif
                for (int cc = 0; cc < cho; cc++) {
                    for(int _ky=_ky0; _ky<_ky1; _ky++){
                        for(int _kx=_kx0; _kx<_kx1; _kx++){
                            int k = _ky*kw + _kx;
                            sbuf[sidx+k] = sptr[k_oft[k]];
                        }
                    }
                    sidx += maxk;
                    sptr = sptr_base + (cc+1)/dmul;
                }
            }
            TM_PERF_ADD(t_sbuf);
            mtype_t* sptr = sbuf;    //sbuf prepare ok~
            for(int c=0; c<out->c; c++){
                sum = 0;
                wtype_t* kptr = (wtype_t*)w + c*maxk;TM_PERF_START();
                tm_dot_prod(sptr, kptr, maxk, &sum);TM_PERF_ADD(t_dotp);
                l_postprocess_sum(sum, b[c], act, outp, SUMSCALE, outscale, out_zp); outp++;TM_PERF_ADD(t_post);
                sptr += maxk; //dwconv need move step
            }
        }
    } 
    TM_PERF_PRINT(t_sbuf);TM_PERF_PRINT(t_dotp);TM_PERF_PRINT(t_post);
    return TM_OK;
}


tm_err_t tml_conv2d_dwconv2d(tm_mat_t* in, tm_mat_t* out, wtype_t* w, btype_t* b, \
    int kw, int kh, int sx, int sy, int dx, int dy, int act, \
    int pad_top, int pad_bottom, int pad_left, int pad_right, int dmul, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp) //kernel: (cho, chi, h, w)
{   TM_DBGT_INIT();
    int pad_flag = (pad_top != 0 ||pad_bottom != 0 ||pad_left != 0 ||pad_right != 0);
    if(dx!=1 || dy!= 1) return TM_ERR_TODO;   
    if(act >= TM_ACT_MAXCNT) return TM_ERR_UNSUPPORT;   
    int maxk = kw*kh;
    if(maxk>TM_MAX_KSIZE) return TM_ERR_KSIZE;
    if(maxk==1 && (pad_flag||dmul)) return TM_ERR_UNSUPPORT;   //assume no pad or dwconv when pwconv

    #if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16)
    #if TM_FASTSCALE
        outscale = (1<<TM_FASTSCALE_SHIFT)/out_s;
        for(int c=0; c<out->c;c++) sumscale[c]=1.0/ws[c]/in_s;
    #else
        outscale = out_s;
        for(int c=0; c<out->c;c++) sumscale[c]=ws[c]*in_s;
    #endif
    #else
        outscale = out_s;
    #endif

    if(maxk == 1) {         //pw conv
        l_tml_pwconv2d(in,out,w,b, kw,kh, sx,sy, dx,dy, act, \
            pad_top, pad_bottom, pad_left, pad_right, dmul, ws, in_s, in_zp, out_s, out_zp);
    } else if(dmul == 0) {  //normal conv
        l_tml_conv2d(in,out,w,b, kw,kh, sx,sy, dx,dy, act, \
            pad_top, pad_bottom, pad_left, pad_right, dmul, ws, in_s, in_zp, out_s, out_zp);
    } else if(dmul == 1){   //dw conv
        l_tml_dwconv2d(in,out,w,b, kw,kh, sx,sy, dx,dy, act, \
            pad_top, pad_bottom, pad_left, pad_right, dmul, ws, in_s, in_zp, out_s, out_zp);
    } else {                //dmul>1 dwconv, rare
        l_tml_mdwconv2d(in,out,w,b, kw,kh, sx,sy, dx,dy, act, \
            pad_top, pad_bottom, pad_left, pad_right, dmul, ws, in_s, in_zp, out_s, out_zp);
    }

    return TM_OK;
}


#endif 

