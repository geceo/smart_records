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
failed:
    if(c->uri)
        mysql_free_uri(c->uri);
    return -1;
}

void *mysql_backend_init(char **error)
{
    mysql_backend_ctx_t *ctx;
    ctx = malloc(sizeof(mysql_backend_ctx_t));
    ctx->mysql_obj = mysql_init(NULL);	
    return ctx;
}
