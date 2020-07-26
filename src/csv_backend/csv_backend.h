#ifndef _SMART_RECORDS_CSV_BACKEND_H
#define _SMART_RECORDS_CSV_BACKEND_H

#include <stdarg.h>


void *csv_backend_init(char **error);
int csv_backend_open_records_base(void *ctx, char **error, const char *uri);
int csv_backend_write_record(void *ctx, char **error, va_list ap);
int csv_backend_set_format(void *ctx, char  **error, const char *format);
int csv_backend_close_records_base(void *ctx);

#endif
