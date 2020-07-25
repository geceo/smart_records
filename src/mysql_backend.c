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
#include "smart_records_internals.h"
//#include <mysql.h>

#define URI_SCHEME "mysql://user[:password]@host[:port]/db_name.table"

typedef struct mysql_uri {
    char *host;
    char *user;
    char *passwd;
    char *db;
    char *table;
    unsigned int port;
} mysql_uri_t;

void mysql_free_uri(mysql_uri_t * uri)
{
    if (!uri)
        return;
    if (uri->host)
        free(uri->host);
    if (uri->user)
        free(uri->user);
    if (uri->passwd)
        free(uri->passwd);
    if (uri->db)
        free(uri->db);
    if (uri->table)
        free(uri->table);
    free(uri);
}



mysql_uri_t *mysql_parse_uri( /* MYSQL *mysql, */ const char *uri, char **error)
{
    int conv;
    mysql_uri_t *r=NULL;
    char *p1=NULL, *p2=NULL, *p3=NULL, *tmp=NULL;

    if(!uri) {
        set_error_and_fails("URI can't be null");
    }

    r = malloc(sizeof(mysql_uri_t));
    memset(r,0,sizeof(mysql_uri_t));

    /* Split the URI in 3 group: identifier, host identifier, db identifier */
    conv = sscanf(uri, "mysql://%m[^@]@%m[^/]/%ms", &p1, &p2, &p3);
    if (conv != 3) {
        set_error_and_fails("URI doesn't respect the scheme");
    }

    /* Parse identifier */
    /* We consider that 'login:' stands for empty password */
    /* XXX: there's a leak here when : is present. Must fix !*/
    conv = sscanf(p1, "%m[^:]%m[:]%ms", &r->user, &tmp, &r->passwd);
    if (!conv) {
        set_error_and_fails("Bad URI: no identifier provided");
    }
	else if (conv == 1) {
#ifdef _WITH_INTERACTIVE_TERMINAL // XXX: is there a way to detect this at runtime ?
        char *tmp = malloc(128);
        size_t n = 128, read;
        fprintf(stdout, "Please, enter password for connection to %s\n", uri);
        
        read = getline(&tmp, &n, stdin);
        if (read >= 1)
            /* Remove trailing \n */
            r->passwd = strndup(tmp, strlen(tmp) - 1);
        free(tmp);
#else
        set_error_and_fails("No password provided and built without support of INTERACTIVE_TERMINAL");
#endif
    }
    free(p1);
    p1 = NULL;

    /* Parse host identifier */
    conv = sscanf(p2, "%m[^:]:%d", &r->host, &r->port);
    if (!conv) {
        set_error_and_fails("Bad URI: need a port number after host:");
    }
	else if (conv == 1) {
        r->port = 3306;
    }
    free(p2);
    p2 = NULL;

    /* Par db identifier */
    conv = sscanf(p3, "%m[^.].%ms", &r->db, &r->table);
    if (conv != 2) {
        set_error_and_fails("Bad URI: need a database.table syntax");
    }
    free(p3);
    p3 = NULL;
    
    return r;
failed:
    if (tmp)
        free(tmp);
    if (r) 
        mysql_free_uri(r);
    if (p1)
        free(p1);
    if (p2)
        free(p2);
    if (p3)
        free(p3);
    return NULL;
}


#ifdef _TEST
char *tests[7][2] = {
    { "Every info provided",
     "mysql://leon:bruxelles@lafrite:1024/STOCK_PATATE.avril" },
/*
    { "Every info provided except password",
     "mysql://leon@lafrite:1024/STOCK_PATATE.avril" },
*/
    { "Every info provided but empty password",
     "mysql://leon:@lafrite:1024/STOCK_PATATE.avril" },
    { "Every info provided except port number",
     "mysql://leon:coin@lafrite/STOCK_PATATE.avril" },
/*
    { "Bad URI #1", "mysql://lafrite/PATATE.avril" },
    { "Bad URI #1", "mysql://lafrite/" },
*/
    { NULL, NULL },
};

int main(int ac, char **av)
{
    mysql_uri_t *r;
    char *error = NULL;

    int i = 0;
    while (tests[i][0]) {
        printf("## %s ##\nURI: %s\nResult:\n", tests[i][0], tests[i][1]);
        r = mysql_parse_uri(tests[i][1], &error);
        if (r) {
            printf("\tr->host=%s\n\tr->port=%d\n\tr->user=%s\n\tr->password=%s\n\tr->db=%s\n\tr->table=%s\n",
                 r->host, r->port, r->user, r->passwd, r->db, r->table);
            mysql_free_uri(r);
        } else {
	    	if(error) {
	    		printf("mysql_parse_uri : got error %s !\n", error);
				free(error);
			}
        }
        i++;
    }
}
#endif
