CFLAGS=-D_WITH_INTERACTIVE_TERMINAL -ggdb -Wall

all: test

clean:
	rm -f *.o mysql_backend_test

test: mysql_backend_test

mysql_backend.o: mysql_backend.c
	$(CC) $(CFLAGS) `pkg-config --cflags mysqlclient` -c -o $@ $?

mysql_backend_utils.o: mysql_backend_utils.c
	$(CC) $(CFLAGS) `pkg-config --cflags mysqlclient` -c -o $@ $?

mysql_backend_test: mysql_backend.c mysql_backend_utils.o
	$(CC) $(CFLAGS) -D_TEST `pkg-config --cflags --libs mysqlclient` -o $@ $?

