/*
 * Copyright (c) 2013 Jeremy Pepper
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of crossfeed nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "crossfeed.h"
#include <string.h>

static const float kernel_96k[] = {
    1-0.0073856832, -0.0075194174, -0.0077223326, -0.0078906622, -0.0081646387, -0.0083914027, -0.0087819435, -0.0091153709, -0.0097044604, -0.010244164, -0.01120129, -0.012166876, -0.013881951, -0.015828054, -0.019321838, -0.023897322, -0.032408956, -0.045482289, -0.070983656, -0.11206752, -0.16362341, -0.12102993
};

static const float kernel_48k[] = {
    1-0.013584239, -0.014855062, -0.01510927, -0.017407341, -0.018212127, -0.023438375, -0.027174231, -0.044839676, -0.071772039, -0.19203754, -0.27824146
};

static const float kernel_44k[] = {
    1-0.015422851, -0.0155861, -0.017845599, -0.018381938, -0.02341632, -0.026318349, -0.043148093, -0.066815346, -0.18979733, -0.29786113
};

int crossfeed_init(crossfeed_t *filter, int samplerate) {
    memset(filter, 0, sizeof(crossfeed_t));
    switch(samplerate) {
    case 44100:
        filter->filter = kernel_44k;
        filter->delay = 0;
        filter->len = sizeof(kernel_44k)/sizeof(float);
        break;
    case 48000:
        filter->filter = kernel_48k;
        filter->delay = 0;
        filter->len = sizeof(kernel_48k)/sizeof(float);
        break;
    case 96000:
        filter->filter = kernel_96k;
        filter->delay = 0;
        filter->len = sizeof(kernel_96k)/sizeof(float);
        break;
    default:
        return -1;
    }
    return 0;
}

static inline void crossfeed_process_sample(crossfeed_t *filter, float left, float right,
                                            float *oleft, float *oright) {
    float mid = (left + right) / 2;
    float side = (left - right) / 2;
    float oside = 0;
    filter->mid[(filter->pos + filter->delay) % filter->len] = mid;
    filter->side[filter->pos] = side;
    if(!filter->bypass) {
        for(unsigned int i=0;i<filter->len;++i) {
            oside += filter->side[(filter->pos + filter->len - i) % filter->len] * filter->filter[i];
        }
    } else {
        oside = filter->side[(filter->pos + filter->len - filter->delay) % filter->len];
    }
    *oleft = filter->mid[filter->pos] + oside;
    *oright = filter->mid[filter->pos] - oside;
    filter->pos = (filter->pos + 1) % filter->len;
}

void crossfeed_filter(crossfeed_t *filter, float *input, float *output, unsigned int size) {
    for(unsigned int i=0;i<size;++i) {
        crossfeed_process_sample(filter, input[i*2], input[i*2+1], &output[i*2],
                                 &output[i*2+1]);
    }
}

void crossfeed_filter_inplace_noninterleaved(crossfeed_t *filter, float *left, float *right,
                                             unsigned int size) {
    for(unsigned int i=0;i<size;++i) {
        crossfeed_process_sample(filter, left[i], right[i], &left[i],
                                 &right[i]);
    }
}
