#ifndef _SMART_RECORDS_H
#define _SMART_RECORDS_H

#include <stdarg.h>
#include "smart_records_internals.h"
   
smart_records_ctx_t *smart_records_init();
int smart_records_open_base(smart_records_ctx_t *ctx, char **error, const char *uri);
int smart_records_set_format(smart_records_ctx_t *ctx, char **error, const char *fmt);
int smart_records_write(smart_records_ctx_t *ctx, char **error, va_list ap);
int smart_records_close_base(smart_records_ctx_t *ctx);
#endif
