#ifndef MYSQL_BACKEND_INTERNALS_H
#define MYSQL_BACKEND_INTERNALS_H

#include <mysql.h>
#define URI_SCHEME "mysql://user[:password]@host[:port]/db_name.table"

typedef struct mysql_uri {
    char *host;
    char *user;
    char *passwd;
    char *db;
    char *table;
    unsigned int port;
} mysql_uri_t;

typedef struct mysql_backend_ctx_t {
    MYSQL *mysql_obj;
    mysql_uri_t *uri;
    char *fmt;
} mysql_backend_ctx_t;


void mysql_free_uri(mysql_uri_t * uri);
mysql_uri_t *mysql_parse_uri(char **error, const char *uri);
#endif
