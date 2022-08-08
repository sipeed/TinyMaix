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

#include "tinymaix.h"
#include "float.h"
#include "math.h"

/*************************** TML_CONV2D **********************************/
static uint32_t k_oft[TM_MAX_KSIZE]; 
//for valid or kernel in valid part, use fast method
tm_err_t __attribute__((weak)) tml_conv2d_dwconv2d(tm_mat_t* in, tm_mat_t* out, wtype_t* w, btype_t* b, \
    int kw, int kh, int sx, int sy, int dx, int dy, int act, \
    int pad_top, int pad_bottom, int pad_left, int pad_right, int dmul, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp) //kernel: (cho, chi, h, w)
{   TM_DBGT_INIT();
    int pad_flag = (pad_top != 0 ||pad_bottom != 0 ||pad_left != 0 ||pad_right != 0);
    if(dx!=1 || dy!= 1) return TM_ERR_TODO;   
    if(act >= TM_ACT_MAXCNT) return TM_ERR_UNSUPPORT;   
    int maxk = kw*kh;
    if(maxk>TM_MAX_KSIZE) return TM_ERR_KSIZE;
    int chi  = in->c; 
    int cho  = out->c;

    int oft = 0;
    int idx = 0;
    for(int y=0; y<kh; y++){
        for(int x=0; x<kw; x++){
            k_oft[idx] = oft;
            idx += 1;
            oft += chi;
        }
        oft += (in->w - kw)*chi; 
    }

    chi  = dmul ? 1 : in->c; // dmul>=1 indicate depthwise; dummy chi for dwconv compatible
    mtype_t* outp;
    int slow_flag = 0; //same pad part is slow
    sumtype_t sum_pad = 0;
    int32_t os = (1<<TM_FASTSCALE_SHIFT)/out_s;
    for (int c = 0; c < out->c; c++) {//TM_DBGL();
        int32_t scale=1.0/ws[c]/in_s;
        outp = out->data + c;
        sum_pad = 0;
        if(pad_flag && TM_MDL_TYPE != TM_MDL_FP32) { // fix pad sum fuse
            for (int k = c*chi*maxk; k < (c+1)*chi*maxk; k++) 
                sum_pad += in_zp*((wtype_t*)w)[k];
        }
        for (int y = 0; y < out->h; y++) {//TM_DBGL();
            int src_y0 = sy*y - pad_top;
            int src_y1 = src_y0+kh;
            for (int x = 0; x < out->w; x++) {
                int src_x0 = sx*x - pad_left;
                int src_x1 = src_x0+kw;
                sumtype_t sum = 0;
                slow_flag = ((src_y0<0)+(src_x0<0)+(src_y1>in->h)+(src_x1>in->w));
                if(!slow_flag) { //valid or same valid part
                    wtype_t* kptr = (wtype_t*)w + c*chi*maxk;
                    mtype_t* sptr = (mtype_t*)TM_MATP(in, src_y0, src_x0, dmul?c/dmul:0); 
                    if(maxk==1){ //speed up pointwise conv
                        for (int cc = 0; cc < chi; cc++) { 
                            sum +=  (sumtype_t)sptr[0]  * (sumtype_t)kptr[0];kptr += 1;sptr += 1;
                        }
                    }else {
                        for (int cc = 0; cc < chi; cc++) {
                            for (int k = 0; k < maxk; k++) {
                                sumtype_t val = (sumtype_t)sptr[k_oft[k]];
                                sumtype_t wt  = (sumtype_t)kptr[k];
                                sum += val * wt;
                            }
                            kptr += maxk;
                            sptr += 1;
                        }
                    }
                } else {    //same pad part  //slower
                    int _ky0 = src_y0<0 ? -src_y0 : 0;
                    int _kx0 = src_x0<0 ? -src_x0 : 0;
                    int _ky1 = in->h-src_y0>kh ? kh : in->h-src_y0;
                    int _kx1 = in->w-src_x0>kw ? kw : in->w-src_x0;
                    wtype_t* kptr = (wtype_t*)w + c*chi*maxk;
                    mtype_t* sptr = (mtype_t*)TM_MATP(in, src_y0, src_x0, dmul?c/dmul:0);  //virtual sptr position in pad
                    sum += sum_pad; //pad_all
                    for (int cc = 0; cc < chi; cc++) {
                        for(int _ky=_ky0; _ky<_ky1; _ky++){
                            for(int _kx=_kx0; _kx<_kx1; _kx++){
                                int k = _ky*kw + _kx;
                                sumtype_t val = ((sumtype_t)sptr[k_oft[k]]-in_zp);
                                sumtype_t wt  = (sumtype_t)kptr[k];
                                sum += val * wt;
                            }
                        }
                        kptr += maxk;
                        sptr += 1;
                    }
                }
                sum += b[c];    //bias, fuse with in_zp
                
            #if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16 )
                #if !TM_FASTSCALE
                    float sumf = sum*ws[c]*in_s;
                #else 
                    sumtype_t sumf = (sum<<TM_FASTSCALE_SHIFT)/scale;
                #endif
            #else
                float sumf = sum;
            #endif
                switch(act){    //activation func
                case TM_ACT_RELU:
                    sumf = sumf>0?sumf:0;
                    break;
                case TM_ACT_RELU6:
                    sumf = sumf>0?sumf:0;
                #if (TM_MDL_TYPE == TM_MDL_FP32) || (!TM_FASTSCALE)
                    sumf = sumf>6?6:sumf;
                #else
                    sumf = sumf>(6<<TM_FASTSCALE_SHIFT)?(6<<TM_FASTSCALE_SHIFT):sumf;
                #endif
                default:
                    break;
                }

            #if TM_MDL_TYPE == TM_MDL_INT8 || TM_MDL_TYPE == TM_MDL_INT16   //requant 
                #if !TM_FASTSCALE
                    *outp = (mtype_t)(sumf/out_s + out_zp);  //(mtype_t)((int)(sumf/out_s) + out_zp) //(mtype_t)((int)(sumf/out_s +0.5) + out_zp)
                #else 
                    *outp = (mtype_t)(((sumf*os)>>(TM_FASTSCALE_SHIFT+TM_FASTSCALE_SHIFT))+out_zp);
                #endif
            #else
                *outp = (mtype_t)sumf;
            #endif
                outp += out->c;
            }
        }
    }
    return TM_OK;
}

/*************************** TML_GAP **********************************/
tm_err_t __attribute__((weak)) tml_gap(tm_mat_t* in, tm_mat_t* out, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp)
{   TM_DBGT_INIT();
    mtype_t* data;
    for(int c=0; c <out->c; c++){
        sumtype_t sum = 0;
        data = in->data + c;
        for(int y=0; y <in->h; y++){
            for(int x=0; x <in->w; x++){
                sum  += ((sumtype_t)(*data));
                data += out->c;
            }
        }
    #if TM_MDL_TYPE == TM_MDL_INT8 || TM_MDL_TYPE == TM_MDL_INT16 
        out->data[c] = (mtype_t)((sum/((in->h)*(in->w))-in_zp)*in_s/out_s + out_zp); //requant
    #else
        out->data[c] = (mtype_t)(sum/((in->h)*(in->w)));
    #endif
    }
    return TM_OK;
}

/*************************** TML_FC **********************************/
tm_err_t __attribute__((weak)) tml_fc(tm_mat_t* in, tm_mat_t* out,  wtype_t* w, btype_t* b, \
    sctype_t* ws, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp)
{   TM_DBGT_INIT();
    mtype_t* data = in->data;
    for(int c=0; c <out->c; c++){
        sumtype_t sum = 0;
        for(int cc=0; cc <in->c; cc++){
            sum += ((sumtype_t)data[cc])*(w[c*in->c+cc]);
        }
        sum += b[c];    //fuse with zp
    #if TM_MDL_TYPE == TM_MDL_INT8 || TM_MDL_TYPE == TM_MDL_INT16 
        out->data[c] = (mtype_t)(sum*in_s*ws[0]/out_s + out_zp); //requant
    #else
        out->data[c] = (mtype_t)(sum);
    #endif
    }
    return TM_OK;
}

/*************************** TML_SOFTMAX **********************************/
tm_err_t __attribute__((weak)) tml_softmax(tm_mat_t* in, tm_mat_t* out, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp)
{   TM_DBGT_INIT(); //note we have float size output buf even in INT8/INT16 mode
    mtype_t* din = in->data;
    float*  dout = (float*)(out->data); 
    float   dmax =  -FLT_MAX;
    for(int c=0; c <in->c; c++){
    #if TM_MDL_TYPE == TM_MDL_INT8 || TM_MDL_TYPE == TM_MDL_INT16 
        dout[c] = (float)((sumtype_t)din[c] - in_zp)*in_s;
    #else
        dout[c] = din[c];
    #endif
        if(dout[c] > dmax) dmax = dout[c];
    }
    float sum = 0;
    for(int c=0; c <in->c; c++){
        dout[c] -= dmax;
        dout[c] = (float)exp(dout[c]);
        sum     += dout[c];
        dout[c] -= 0.000001;  //prevent 1.0 value (cause 256 overflow)
    }
    for(int c=0; c <in->c; c++){  //int8/int16 <= fp32, so it is ok
    #if TM_MDL_TYPE == TM_MDL_INT8 || TM_MDL_TYPE == TM_MDL_INT16 
        out->data[c] = (mtype_t)(dout[c]/sum/out_s + out_zp); //requant
    #else
        out->data[c] = (mtype_t)(dout[c]/sum);
    #endif
    }
    return TM_OK;
}

/*************************** TML_RESHAPE **********************************/
tm_err_t __attribute__((weak)) tml_reshape(tm_mat_t* in, tm_mat_t* out, sctype_t in_s, zptype_t in_zp, sctype_t out_s, zptype_t out_zp)
{   
    //in fact do nothing... out shape
    return TM_OK;
}
