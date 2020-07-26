#ifndef SMART_RECORDS_INTERNALS_H
#define SMART_RECORDS_INTERNALS_H

/*
 * Small helper to populate char **error and jump to failed label.
 */
#define set_error_and_fails(x) { if (error) *error = strdup(x); goto failed; }

typedef struct smart_records_ctx_t {
    void *backend_ctx;
    int (*set_format) (void *ctx, char **error, const char *fmt);
    int (*write_record) (void *ctx, char **error, va_list ap);
    int (*close_records_base) (void *ctx);
} smart_records_ctx_t ;   

#endif

