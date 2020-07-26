#ifndef CSV_BACKEND_INTERNALS_H
#define CSV_BACKEND_INTERNALS_H

typedef struct csv_backend_ctx_t {
	int fd;
	char *pathname;
	char *fmt;
	int mode;
} csv_backend_ctx_t;

#endif
