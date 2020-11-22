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

#include <stdlib.h>
#include <sndfile.h>
#include "crossfeed.h"

int main(int argc, char *argv[]) {
    char *in_filename = argv[1];
    char *out_filename = argv[2];
    SF_INFO info = {0};
    SNDFILE *in_file, *out_file;
    crossfeed_t filter;
    float buf[2048], obuf[2048], tmp;
    int read, i;
    in_file = sf_open(in_filename, SFM_READ, &info);
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    out_file = sf_open(out_filename, SFM_WRITE, &info);
    crossfeed_init(&filter, 96000);
    while((read = sf_read_float(in_file, buf, 2048)) > 0) {
        crossfeed_filter(&filter, buf, obuf, read/2);
        for(i=0;i<read;++i) {
            obuf[i] = obuf[i] > 1 ? 1 : (obuf[i] < -1 ? -1 : obuf[i]);
        }
        sf_write_float(out_file, obuf, read);
    }
    sf_close(out_file);
    sf_close(in_file);
    return EXIT_SUCCESS;
}
