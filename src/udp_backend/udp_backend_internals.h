#ifndef UDP_BACKEND_INTERNALS_H
#define UDP_BACKEND_INTERNALS_H

typedef struct udp_backend_ctx_t {
    int socket;
    struct sockaddr_in sa;
	char *fmt;
} udp_backend_ctx_t;

#endif
