#ifndef _SMART_RECORDS_MYSQL_BACKEND_H
#define _SMART_RECORDS_MYSQL_BACKEND_H

#include <stdarg.h>


void *mysql_backend_init(char **error);
int mysql_backend_open_records_base(void *ctx, char **error, const char *uri);
int mysql_backend_write_record(void *ctx, char **error, va_list ap);
int mysql_backend_set_format(void *ctx, char  **error, char *format);
int mysql_backend_close_records_base(void *ctx);

#endif
