CFLAGS=-D_WITH_INTERACTIVE_TERMINAL `pkg-config --cflags mysqlclient` -ggdb -Wall
LDFLAGS=`pkg-config --libs mysqlclient`

all: test mysql_backend.a

clean:
	rm -f *.o *.a mysql_backend_test

test: mysql_backend_test

mysql_backend.a: mysql_backend.o mysql_backend_utils.o
	$(AR) -rcv $@ $?

mysql_backend_test: mysql_backend.c mysql_backend_utils.c
	$(CC) $(CFLAGS) -D_TEST `pkg-config --cflags --libs mysqlclient` -o $@ $?

