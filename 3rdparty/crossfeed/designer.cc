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

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <Accelerate/Accelerate.h>
using namespace std;

static FFTSetup fft_context;

struct magic {
    int samplerate;
    int delay;
    int len;
    int offset;
    int limit;
};

static float transfer_function(float x) {
    return pow(10, (x <= 1500 ? 2 : 2 * log2(x/750)) / -20);
}

static void compute_mono_response(float *result, float *filter, const struct magic *magic) {
    vDSP_vclr(result, 1, 512);
    for(unsigned i=0;i<magic->len;++i) {
        result[i+magic->offset] = filter[i] / 2;
    }
}

static void compute_crossfeed_response(float *result, float *filter, const struct magic *magic) {
    for(unsigned i=0;i<magic->len;++i) {
        result[i+magic->offset] = -filter[i] / 2;
    }
    result[magic->offset] += 0.5;
}

static double compute_error(float *filter, float *transfer_fn, const struct magic *magic) {
    float result[512];
    float response_memory[512];
    float response_interleaved[512];
    DSPSplitComplex response = {response_memory, response_memory + 256};
    float crossfeed_error = 0, mono_error = 0;
    compute_mono_response(result, filter, magic);
    vDSP_ctoz((DSPComplex *)result, 2, &response, 1, 256);
    vDSP_fft_zrip(fft_context, &response, 1, 9, FFT_FORWARD);
    vDSP_ztoc(&response, 1, (DSPComplex *)response_interleaved, 2, 256);
    vDSP_polar(response_interleaved, 2, response_interleaved, 2, 256);
    for(unsigned int i=0;i<256;++i) {
        float err = 1 - 0.5 * response_interleaved[2*i];
        mono_error += err*err;
    }
    compute_crossfeed_response(result, filter, magic);
    vDSP_ctoz((DSPComplex *)result, 2, &response, 1, 256);
    vDSP_fft_zrip(fft_context, &response, 1, 9, FFT_FORWARD);
    vDSP_ztoc(&response, 1, (DSPComplex *)response_interleaved, 2, 256);
    for(unsigned int i=0;i<magic->limit;++i) {
        float freq = (i * magic->samplerate) / 512.;
        float delay = (magic->delay * 1000000.) / magic->samplerate - (0.5*transfer_fn[i] - pow(10, -2./20)) * 500;
        float phase = 2*M_PI*((magic->delay / 1000000.) / (1. / freq));
        float err_s = transfer_fn[i]*sin(phase) - 0.5 * response_interleaved[2*i];
        float err_c = transfer_fn[i]*sin(phase) - 0.5 * response_interleaved[2*i+1];
        crossfeed_error += (err_s*err_s + err_c*err_c) / 2;
    }
    return (mono_error / 256) + (crossfeed_error / magic->limit);
}

static double window_fn(int i, int N) {
    return 0.42 - 0.5 * cos((2*M_PI*i)/(N-1)) + 0.08 * cos((4*M_PI*i)/(N-1));
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    float filter[74] = {0};
    float slope[74];
    float weight[74];
    float transfer_fn[256];
    float mu = 0.2;
    float err;
    unsigned int pass = 0;
    const float delta = 0.00001;
    struct magic magic;
    magic.samplerate = argc > 1 ? atoi(argv[1]) : 96000;
    magic.delay = (magic.samplerate * 250) / 1000000;
    magic.len = 3 * magic.delay + 2;
    magic.offset = 256 - magic.len / 2;
    magic.limit = 256;
    if(magic.limit > 256)
        magic.limit = 256;
    for(unsigned int i=0;i<256;++i) {
        transfer_fn[i] = transfer_function((i * magic.samplerate) / 512.);
    }
    filter[magic.delay] = 1;
    filter[2*magic.delay+1] = -1;
    for(unsigned int i=0;i<magic.len;++i) {
        if(i < magic.delay) {
            weight[i] = window_fn(i, 2*magic.delay+1);
        } else if (i < magic.len - magic.delay) {
            weight[i] = 1;
        } else {
            weight[i] = weight[magic.len - i];
        }
    }
    fft_context = vDSP_create_fftsetup(9, 2);
    err = compute_error(filter, transfer_fn, &magic);
    while(true) {
        if(err < 1. / (1 << 24) || mu < 1. / (1 << 24))
            break;
        float new_filter[74];
        memcpy(new_filter, filter, 74*sizeof(float));
        for(unsigned int i=0; i<magic.len; ++i) {
            new_filter[i] += delta;
            slope[i] = (compute_error(new_filter, transfer_fn, &magic) - err) / delta;
            new_filter[i] = filter[i];
        }
        for(unsigned int i=0; i<magic.len; ++i) {
            new_filter[i] -= slope[i] * weight[i] * mu;
        }
        float new_err = compute_error(new_filter, transfer_fn, &magic);
        if(new_err < err) {
            memcpy(filter, new_filter, 74*sizeof(float));
            err = new_err;
        } else {
            mu /= 2;
        }
        if(pass % 100 == 0) {
            cout << "Pass " << pass << ", error: " << pow(10, err/20) << "dBFS" << endl;
        }
        ++pass;
    }
    ofstream output("filter.txt");
    output << setprecision(numeric_limits<float>::digits10+2);
    for(unsigned int i=0;i<magic.len;++i) {
        output << filter[i] << '\n';
    }
    vDSP_destroy_fftsetup(fft_context);
}
