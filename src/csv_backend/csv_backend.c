/*
Copyright (c) 2020 Gérald Colangelo <gcolangelo@ospow.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "../smart_records_internals.h"
#include "csv_backend_internals.h"

void *csv_backend_init(char **error)
{
    csv_backend_ctx_t *ctx=NULL;
    ctx = malloc(sizeof(csv_backend_ctx_t));
    memset(ctx, 0, sizeof(csv_backend_ctx_t));
	ctx->mode = 0644;
	ctx->fmt = NULL;
	ctx->pathname = NULL;
    return ctx;
}

int csv_backend_open_records_base(void *ctx, char **error, const char *uri)
{
    csv_backend_ctx_t *c;
    if (!ctx || !uri) {
        set_error_and_fails("invalid arguments while opening records base");
    }
    c = (csv_backend_ctx_t *)ctx;

	// XXX: Implement mode parsing example: csv://<filename>:<mode>
	c->pathname = strdup(strchr(uri,':')+1);
	c->fd = open(c->pathname, O_CREAT|O_APPEND|O_WRONLY, c->mode);
	if (c->fd < 0) {
		/* And then, we're not POSIX anymore */
#define _GNU_SOURCE
		asprintf(error,"failed opening CSV file: %s", strerror(errno));
		goto failed;
	}
    return 0;
failed:
    return -1;
}

int csv_backend_set_format(void *ctx, char **error, const char *fmt)
{
    csv_backend_ctx_t *c;
    if (!ctx || !fmt) {
        set_error_and_fails("invalid parameters while setting format.")
    }
    c = (csv_backend_ctx_t *) ctx;
    c->fmt = strdup(fmt);
    return 0;
failed:
    return -1;
}

int csv_backend_write_record(void *ctx, char **error, ...)
{
    csv_backend_ctx_t *c;
    int size=0;
    va_list ap;

    if (!ctx) {
        set_error_and_fails("invalid parameters while setting format.")
    }
    c = (csv_backend_ctx_t *) ctx;

    if (!c->fmt) {
        set_error_and_fails("no format provided, please set it before writing any record");
    }

    va_start(ap,error);
    size = vsnprintf(NULL, size, c->fmt, ap);
    va_end(ap);

    va_start(ap,error);
    if (vdprintf(c->fd, c->fmt, ap) != size) {
        set_error_and_fails("vdprintf() failed");
    }
    va_end(ap);

    return 0;
failed:
    return -1;
}

int csv_backend_close_records_base(void *ctx)
{
    csv_backend_ctx_t *c;
    if(!ctx)
        return -1;
    c = (csv_backend_ctx_t *)ctx;

	if(c->fd > 0)
		close(c->fd);
    if(c->pathname)
        free(c->pathname);
    if(c->fmt)
        free(c->fmt);
    free(c);

    return 0;
}



#ifdef _TEST

#define dump_error_and_quit(func,error) \
        { \
            if (error) { \
                fprintf(stderr, "[ERROR] %s(): %s\n", func, error); \
            } else { \
                fprintf(stderr, "[ERROR] %s(): unknown error\n", func); \
            } \
            free(error); \
            exit(EXIT_FAILURE); \
        }


int main(int ac, char **av)
{
    void *ctx;
    char *error=NULL;
    int i=0;

    if(ac != 2) {
        printf("Usage: %s <mysql_uri>\n \
            mysql_uri format is mysql://user[:password]@host[:port]/database.table\n \
            database shall exists and table must obey the following scheme:\n \
            CREATE TABLE <table> (a INT, b INT, c VARCHAR(40))\n",av[0]);
        exit(EXIT_FAILURE);
    }

    ctx = csv_backend_init(&error);
    if (ctx == NULL) {
        dump_error_and_quit("csv_backend_init", error);
    }

    if (csv_backend_set_format(ctx, &error, "%d;%d;%s;\n") < 0){
        dump_error_and_quit("csv_backend_set_format", error);
    }

    if(csv_backend_open_records_base(ctx, &error, av[1]) < 0) {
        dump_error_and_quit("csv_backend_set_open_records_base", error);
    }

    for (i=0; i<4; i++) {
        if (csv_backend_write_record(ctx, &error, i, i+10, "Text here") <0 ){
            dump_error_and_quit("csv_backend_write_record", error);
        }
    }
    csv_backend_close_records_base(ctx);
}
#endif
