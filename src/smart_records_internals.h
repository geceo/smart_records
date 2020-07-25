#ifndef SMART_RECORDS_INTERNALS_H
#define SMART_RECORDS_INTERNALS_H

/*
 * Small helper to populate char **error and jump to failed label.
 */
#define set_error_and_fails(x) { if (error) *error = strdup(x); goto failed; }

typedef struct smart_records_ctx_t {
    void *CTX;
    int (*set_format) (struct smart_records_ctx_t *ctx, char **error, char *fmp);
    int (*write_record) (struct smart_records_ctx_t *ctx, char **error, va_list ap);
    int (*close_records_base) (struct smart_records_ctx_t *ctx);
} smart_records_ctx_t ;   

#endif

