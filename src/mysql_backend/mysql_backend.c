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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../smart_records_internals.h"
#include "mysql_backend_internals.h"

void *mysql_backend_init(char **error)
{
    mysql_backend_ctx_t *ctx=NULL;
    ctx = malloc(sizeof(mysql_backend_ctx_t));
    memset(ctx, 0, sizeof(mysql_backend_ctx_t));
    ctx->mysql_obj = mysql_init(NULL);
    return ctx;
}

int mysql_backend_open_records_base(void *ctx, char **error, const char *uri)
{
    mysql_backend_ctx_t *c;
    if (!ctx || !uri) {
        set_error_and_fails("invalid arguments while opening records base");
    }
        
    c = (mysql_backend_ctx_t *)ctx;
	c->uri = mysql_parse_uri(error, uri);
	if (!c->uri)
        goto failed;
    if(!mysql_real_connect(c->mysql_obj, c->uri->host,
                        c->uri->user,
                        c->uri->passwd,
                        c->uri->db,
                        c->uri->port,
                        NULL,0)) {

        set_error_and_fails(mysql_error(c->mysql_obj));
    }
    return 0;
failed:
    if(c->uri)
        mysql_free_uri(c->uri);
    return -1;
}

int mysql_backend_set_format(void *ctx, char **error, const char *fmt)
{
    mysql_backend_ctx_t *c;
    if (!ctx || !fmt) {
        set_error_and_fails("invalid parameters while setting format.")
    }
    c = (mysql_backend_ctx_t *) ctx;
    c->fmt = strdup(fmt);
    return 0;
failed:
    return -1;
}

int mysql_backend_write_record(void *ctx, char **error, ...)
{
    mysql_backend_ctx_t *c;
    char *record = NULL;
    int ret=0, size=0;
    va_list ap;

    if (!ctx) {
        set_error_and_fails("invalid parameters while setting format.")
    }
    c = (mysql_backend_ctx_t *) ctx;

    if (!c->fmt) {
        set_error_and_fails("no format provided, please set it before writing any record");
    }

    va_start(ap,error);
    size = vsnprintf(NULL, size, c->fmt, ap);
    va_end(ap);

    va_start(ap,error);
    record = malloc(sizeof(char) * (size+1));
    if (vsnprintf(record, size, c->fmt, ap) != size) {
        set_error_and_fails("request prepation failed: check your format")
    }
    va_end(ap);

    printf("--> %s\n", record);
    if (mysql_query(c->mysql_obj, record) != 0) {
        set_error_and_fails(mysql_error(c->mysql_obj))
    }
    free(record);
    return 0;
failed:
    if(record)
        free(record);
    return -1;
}

int mysql_backend_close_records_base(void *ctx)
{
    mysql_backend_ctx_t *c;
    if(!ctx)
        return -1;
    c = (mysql_backend_ctx_t *)ctx;

    mysql_close(c->mysql_obj);
    if(c->uri)
        mysql_free_uri(c->uri);
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

    ctx = mysql_backend_init(&error);
    if (ctx == NULL) {
        dump_error_and_quit("mysql_backend_init", error);
    }

    if (mysql_backend_set_format(ctx, &error, "INSERT INTO test (a,b,c) VALUES (%d,%d,\"%s\");") < 0){
        dump_error_and_quit("mysql_backend_set_format", error);
    }

    if(mysql_backend_open_records_base(ctx, &error, av[1]) < 0) {
        dump_error_and_quit("mysql_backend_set_open_records_base", error);
    }

    for (i=0; i<4; i++) {
        if (mysql_backend_write_record(ctx, &error, i, i+10, "Text here") <0 ){
            dump_error_and_quit("mysql_backend_write_record", error);
        }
    }
    mysql_backend_close_records_base(ctx);
}
#endif
