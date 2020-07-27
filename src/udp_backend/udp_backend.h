#ifndef _SMART_RECORDS_UDP_BACKEND_H
#define _SMART_RECORDS_UDP_BACKEND_H

#include <stdarg.h>


void *udp_backend_init(char **error);
int udp_backend_open_records_base(void *ctx, char **error, const char *uri);
int udp_backend_write_record(void *ctx, char **error, va_list ap);
int udp_backend_set_format(void *ctx, char  **error, const char *format);
int udp_backend_close_records_base(void *ctx);

#endif
