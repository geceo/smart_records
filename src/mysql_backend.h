#ifndef _SMART_RECORDS_MYSQL_BACKEND_H
#define _SMART_RECORDS_MYSQL_BACKEND_H

#include <stdarg.h>
#include "smart_records_internals.h"
//#include <mysql.h>

typedef struct mysql_backend_ctx_t {
    //MYSQL *mysql;
    char *format;
} mysql_backend_ctx_t;

int mysql_backend_write_record(smart_records_ctx_t *ctx, char **error, va_list ap);
int mysql_backend_set_format(smart_records_ctx_t *ctx, char  **error, char *format);
int mysql_backend_close_records_base(smart_records_ctx_t *ctx);
void *mysql_backend_init(smart_records_ctx_t *ctx, char **error);

#endif
