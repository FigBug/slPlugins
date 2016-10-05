#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sndfile.h>

void write_sample(SNDFILE *snd, FILE *fp, char *wav, char *name, 
        float *buf, int bufsize, uint32_t *pos, int sr)
{
    SF_INFO info;
    memset(&info, 0, sizeof(SF_INFO));
    info.format = 0;
    int count;
    SNDFILE *in = sf_open(wav, SFM_READ, &info);
    fprintf(fp, "[%s]\npos = %g\n", name, 1.0 * (*pos) / sr);
    uint32_t start = *pos;
    while((count = sf_read_float(in, buf, bufsize))) {
        sf_write_float(snd, buf, count);
        *pos += count;
    }
    fprintf(fp, "size = %g\n\n", (1.0 * (*pos) - start) / sr);
    sf_close(in);
}

int main(int argc, char *argv[]) 
{
    if(argc <= 1) {
        printf("Usage: [options] wav2smp in1.wav in1_name in2.wav in2_name...\n\n");
        printf("Flags:\n");
        printf("\t-w\tWAV file to write to (default: out.wav)\n");
        printf("\t-o\tINI file to write to (default: out.ini)\n");
        printf("\t-r\tSet samplerate. (default: 96000)\n");
        return 0;
    }


    int argpos = 1;
    int i;
    int sr = 96000;
    char wavfile[30] = "out.wav";
    char inifile[30] = "out.ini";
    float buf[4096];

    argv++;

    while(argv[0][0] == '-') {
        switch(argv[0][1]) {
            case 'w': 
                strncpy(wavfile, argv[1], 30);
                argv++; 
                argpos++;
                break;

            case 'o': 
                strncpy(inifile, argv[1], 30);
                argv++; 
                argpos++;
                break;

            case 'r': 
                sr = atoi(argv[1]);
                argv++; 
                argpos++;
                break;
                
            default: 
                fprintf(stderr, "Uknown option '%c'\n", argv[0][1]);
                break;

        }
        argv++;
        argpos++;
    }

    SF_INFO info;
    info.samplerate = sr; 
    info.channels = 1;
    info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    SNDFILE *snd = sf_open(wavfile, SFM_WRITE, &info);
    FILE *fp = fopen(inifile, "w");
    uint32_t pos = 0; 


    for(i = argpos; i < argc; ) {
        write_sample(snd, fp, argv[0], argv[1], buf, 4096, &pos, sr); 
        argv += 2;
        i += 2;
    }

    fclose(fp); 
    sf_close(snd);
    return 0;
}
