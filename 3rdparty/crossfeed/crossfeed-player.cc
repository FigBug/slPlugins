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
#include <stdio.h>
#include <pthread.h>
#include <termios.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <vector>
#include <string>
#include "cautil.h"
#include "message_queue.h"
#include "crossfeed.h"

class playlist {
public:
    void shuffle() { _shuffle = true; }
    void add(const char *file) {
        struct stat st;
        if(!stat(file, &st)) {
            if(st.st_mode & S_IFREG)
                _files.push_back(file);
            else if(st.st_mode & S_IFDIR) {
                char buf[PATH_MAX];
                DIR *dir = opendir(file);
                if(dir) {
                    struct dirent ent, *result;
                    while(!readdir_r(dir, &ent, &result) && result) {
                        if(strcmp(".", ent.d_name) == 0 ||
                           strcmp("..", ent.d_name) == 0 ||
                           strcmp(".DS_Store", ent.d_name) == 0 ||
                           strncmp("._", ent.d_name, 2) == 0)
                            continue;
                        snprintf(buf, PATH_MAX, "%s/%s", file, ent.d_name);
                        add(buf);
                    }
                    closedir(dir);
                }
            }
        }
    }
    const char *next() {
        if(_shuffle) {
            srand(time(NULL));
            random_shuffle(_files.begin(), _files.end(), [](unsigned int n) {
                return rand() % n;
            });
            _shuffle = false;
        }
        ++pos;
        return pos >= _files.size() ? NULL : _files[pos].c_str();
    }
    const char *prev() {
        pos = pos > 0 ? pos - 1 : pos;
        return pos >= _files.size() ? NULL : _files[pos].c_str();
    }
    void erase_current() {
        _files.erase(_files.begin()+pos);
        prev();
    }
private:
    std::vector<std::string> _files;
    bool _shuffle = false;
    ssize_t pos = -1;
};

static float scale_db = 0;
static float scale = 1;

static crossfeed_t crossfeed;

static message_queue cmq, acmq;

static struct termios t_orig_params, t_params;

static void set_volume(float volume) {
    scale_db = volume;
    scale = pow(10, scale_db/20);
}

static void tell(message_queue *queue, char c) {
    char *msg = (char *)message_queue_message_alloc_blocking(queue);
    *msg = c;
    message_queue_write(queue, msg);
}

static void console_init() {
    tcgetattr(0, &t_params);
    t_orig_params = t_params;
    cfmakeraw(&t_params);
    tcsetattr(0, TCSANOW, &t_params);
}

static void console_reset() {
    tcsetattr(0, TCSANOW, &t_orig_params);
}

static void *conio_threadproc(void *data) {
    while(true) {
        int r = getchar();
        char c = r == EOF ? 'q' : r;
        tell(&cmq, c);
    }
    return data;
}

static void event_handler(struct PlayerEvent *evt) {
    switch(evt->type) {
    case PlayerEvent::PLAYER_RENDER:
        crossfeed_filter_inplace_noninterleaved(&crossfeed, evt->left, evt->right, evt->size);
        for(unsigned int i=0;i<evt->size;++i) {
            evt->left[i] *= scale;
            evt->right[i] *= scale;
        }
        break;
    case PlayerEvent::PLAYER_DONE:
        tell(&acmq, '>');
        break;
    }
}

static void play_next(Player *player, playlist *pl) {
    while(true) {
        const char *file = pl->next();
        if(file) {
            fprintf(stderr, "Playing `%s'...\r\n", file);
            if(CAPlayFile(player, file)) {
                fprintf(stderr, "Error playing `%s'\r\n", file);
                pl->erase_current();
            } else
                break;
        } else {
            tell(&cmq, 'q');
            break;
        }
    }
}

static void play_prev(Player *player, playlist *pl) {
    const char *file = pl->prev();
    fprintf(stderr, "Playing `%s'...\r\n", file);
    CAPlayFile(player, file);
}


static void *audio_threadproc(void *data) {
    struct Player player;
    playlist *pl = (playlist *)data;
    const char *file;
    bool running = true;
    if(CAInitPlayer(&player, &event_handler)) {
        fprintf(stderr, "Error initializing audio output\n");
        goto e_done;
    }
    if(crossfeed_init(&crossfeed, player.samplerate)) {
        fprintf(stderr, "Filter not available for %dHz\n", player.samplerate);
        goto e_destroy_player;
    }
    play_next(&player, pl);
    while(running) {
        char *msg = (char *)message_queue_read(&acmq);
        char c = *msg;
        message_queue_message_free(&acmq, msg);
        switch(c) {
        case '<':
            CAStopPlayback(&player);
            play_prev(&player, pl);
            break;
        case '>':
            CAStopPlayback(&player);
            play_next(&player, pl);
            break;
        case 'q':
            running = false;
            break;
        }
    }
    CADestroyPlayer(&player);
    return data;
e_destroy_player:
    CADestroyPlayer(&player);
e_done:
    tell(&cmq, 'q');
    return data;
}

int main(int argc, char *argv[]) {
    playlist playlist;
    pthread_t conio, audio;
    bool running = true;
    if(argc < 2) {
        fprintf(stderr, "Usage: %s [-s] [-g dBFS] /foo/bar\n", argc == 1 ? argv[0] : "crossfeed-player");
        return EXIT_FAILURE;
    }
    for(int i = 1; i < argc; ++i) {
        if(strcmp("-g", argv[i]) == 0) {
            if(++i >= argc)
                break;
            set_volume(atof(argv[i]));
        } else if(strcmp("-s", argv[i]) == 0) {
            playlist.shuffle();
        } else {
            playlist.add(argv[i]);
        }
    }
    message_queue_init(&cmq, 1, 16);
    message_queue_init(&acmq, 1, 16);
    console_init();
    pthread_create(&conio, NULL, &conio_threadproc, NULL);
    pthread_create(&audio, NULL, &audio_threadproc, &playlist);
    while(running) {
        char *msg = (char *)message_queue_read(&cmq);
        char c = *msg;
        message_queue_message_free(&cmq, msg);
        switch(c) {
        case '*':
            set_volume(scale_db+0.5);
            fprintf(stderr, "Volume: %.1f  \r", scale_db);
            break;
        case '/':
            set_volume(scale_db-0.5);
            fprintf(stderr, "Volume: %.1f  \r", scale_db);
            break;
        case 'q':
            running = false;
        case '<':
        case '>':
            tell(&acmq, c);
            break;
        case 'c':
            crossfeed.bypass = crossfeed.bypass ? 0 : 1;
            fprintf(stderr, "XFeed: %s    \r", crossfeed.bypass ? "Off" : "On");
            break;
        }
    }
    pthread_cancel(conio);
    pthread_detach(conio);
    console_reset();
    pthread_join(audio, NULL);
    message_queue_destroy(&acmq);
    message_queue_destroy(&cmq);
    return EXIT_SUCCESS;
}
