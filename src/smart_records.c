#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "smart_records.h"
#include "smart_records_internals.h"

#ifdef _HAS_MYSQL
#include "mysql_backend/mysql_backend.h"
#endif

#ifdef _HAS_SQLITE
//#include "sqlite_backend.h"
#endif

#ifdef _HAS_UDP
#include "udp_backend/udp_backend.h"
#endif

#ifdef _HAS_CSV
#include "csv_backend/csv_backend.h"
#endif


// nothing but give a context for the moment
smart_records_ctx_t *smart_records_init(){
    return malloc(sizeof(smart_records_ctx_t));    
}
    
int smart_records_open_base(smart_records_ctx_t *ctx, char **error, const char *uri)
{
    int len_uri;

    if(!uri) {
        set_error_and_fails("URI is null");
    }
        
    len_uri = strlen(uri);
    if(!strncmp("mysql://", uri, len_uri < 8 ? len_uri : 8)) {
#ifdef _HAS_MYSQL
        ctx->backend_ctx = NULL;
        ctx->write_record = mysql_backend_write_record;
        ctx->set_format = mysql_backend_set_format;
        ctx->close_records_base = mysql_backend_close_records_base;
        if (!(ctx->backend_ctx = mysql_backend_init(error)))
            goto failed;
#else
        set_error_and_fails("built without support for mysql:// URIs");
#endif
    }

    else if(!strncmp("udp://", uri, len_uri < 8 ? len_uri : 8)) {
#ifdef _HAS_UDP
        ctx->backend_ctx = NULL;
        ctx->write_record = udp_backend_write_record;
        ctx->set_format = udp_backend_set_format;
        ctx->close_records_base = udp_backend_close_records_base;
        if (!(ctx->backend_ctx = udp_backend_init(error)))
            goto failed;
#else
        set_error_and_fails("built without support for udp:// URIs");
#endif
    }

    else if(!strncmp("csv://", uri, len_uri < 8 ? len_uri : 8)) {
#ifdef _HAS_CSV
        ctx->backend_ctx = NULL;
        ctx->write_record = csv_backend_write_record;
        ctx->set_format = csv_backend_set_format;
        ctx->close_records_base = csv_backend_close_records_base;
        if (!(ctx->backend_ctx = csv_backend_init(error)))
            goto failed;
#else
        set_error_and_fails("built without suppor fort csv:// URIs");
#endif
    }
    else if(!strncmp("sqlite://", uri, len_uri < 9 ? len_uri : 9)) {
        set_error_and_fails("support for sqlite:// URIs not yet implemented");
    }

    else {
        set_error_and_fails("Unknown scheme or bad URI.");
        goto failed;
    }
    return 0;
failed:
    return -1;
}

int smart_records_set_format(smart_records_ctx_t *ctx, char **error, const char *fmt)
{
    return ctx->set_format(ctx, error, fmt);
}

int smart_records_close_base(smart_records_ctx_t *ctx)
{
    return ctx->close_records_base(ctx);
}   

int smart_records_write(smart_records_ctx_t *ctx, char **error, va_list ap)
{
    return ctx->write_record(ctx, error, ap);
}

int main(int ac, char **av)
{
    printf("coucou !\n");
    return 0;
}
