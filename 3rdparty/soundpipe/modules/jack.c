#include <stdlib.h>
#include <jack/jack.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "soundpipe.h"

typedef struct sp_jack{
    sp_data *sp;
    jack_port_t **output_port;
    jack_client_t **client;
    void *ud;
    void (*callback)(sp_data *, void *);
}sp_jack;


/*
 * Simple UDP server from modified code from
 * http://cs.baylor.edu/~donahoo/practical/CSockets/code/UDPEchoServer.c
 *
 */

static void DieWithError(char *errorMessage)  {
    printf("%s\n", errorMessage);
}

static void start_udp_server(int port) {
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr;
    unsigned int cliAddrLen;
    char echoBuffer[255];
    unsigned short echoServPort;
    int recvMsgSize;
    int run = 1;
    echoServPort = port;

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    while(run)
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, 255, 0,
            (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");

        if(!strncmp(echoBuffer, "exit", 4)) run = 0;

        /* Send received datagram back to the client */
        if (sendto(sock, echoBuffer, recvMsgSize, 0,
             (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
            DieWithError("sendto() sent a different number of bytes than expected");
    }
}

static int sp_jack_cb(jack_nframes_t nframes, void *arg)
{
    int i, chan;
    sp_jack *jd = arg;
    jack_default_audio_sample_t  *out[jd->sp->nchan];
    for(chan = 0; chan < jd->sp->nchan; chan++)
    out[chan] = jack_port_get_buffer (jd->output_port[chan], nframes);
    for(i = 0; i < nframes; i++){
        jd->callback(jd->sp, jd->ud);
        for(chan = 0; chan < jd->sp->nchan; chan++)
        out[chan][i] = jd->sp->out[chan];
    }
    return 0;
}

void sp_jack_shutdown (void *arg)
{
    exit (1);
}

int sp_jack_process(sp_data *sp, void *ud, void (*callback)(sp_data *, void *))
{
    const char **ports;
    const char *client_name = "soundpipe";
    const char *server_name = NULL;
    int chan;
    jack_options_t options = JackNullOption;
    jack_status_t status;
    sp_jack jd;
    jd.sp = sp;
    jd.callback = callback;
    jd.ud = ud;

    jd.output_port = malloc(sizeof(jack_port_t *) * sp->nchan);
    jd.client = malloc(sizeof(jack_client_t *));

    jd.client[0] = jack_client_open (client_name, options, &status, server_name);
    if (jd.client[0] == NULL) {
        fprintf (stderr, "jack_client_open() failed, "
             "status = 0x%2.0x\n", status);
        if (status & JackServerFailed) {
            fprintf (stderr, "Unable to connect to JACK server\n");
        }
        exit (1);
    }
    if (status & JackServerStarted) {
        fprintf (stderr, "JACK server started\n");
    }
    if (status & JackNameNotUnique) {
        client_name = jack_get_client_name(jd.client[0]);
        fprintf (stderr, "unique name `%s' assigned\n", client_name);
    }
    jack_set_process_callback (jd.client[0], sp_jack_cb, &jd);
    jack_on_shutdown (jd.client[0], sp_jack_shutdown, 0);

char chan_name[50];
for(chan = 0; chan < sp->nchan; chan++) {
    sprintf(chan_name, "output_%d", chan);
    printf("registering %s\n", chan_name);
    jd.output_port[chan] = jack_port_register (jd.client[0], chan_name,
                      JACK_DEFAULT_AUDIO_TYPE,
                      JackPortIsOutput, chan);

    if (jd.output_port[chan] == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
        exit (1);
    }

    if (jack_activate (jd.client[0])) {
        fprintf (stderr, "cannot activate client");
        exit (1);
    }
}
    ports = jack_get_ports (jd.client[0], NULL, NULL,
                JackPortIsPhysical|JackPortIsInput);
    if (ports == NULL) {
        fprintf(stderr, "no physical playback ports\n");
        exit (1);
    }
for(chan = 0; chan < sp->nchan; chan++) {
    if (jack_connect (jd.client[0], jack_port_name (jd.output_port[chan]), ports[chan])) {
        fprintf (stderr, "cannot connect output ports\n");
    }
}
    free (ports);
    int portnum = 9999;
    printf("To quit, Send the message \"exit\" to UDP port %d.\n", portnum);
    printf("Use this command: 'echo exit | nc -u localhost 9999'\n");
    start_udp_server(portnum);
    jack_client_close (jd.client[0]);
    free(jd.output_port);
    free(jd.client);

    return SP_OK;
}


