CFLAGS=-ggdb -Wall

all: test csv_backend.a

clean:
	rm -f *.o *.a csv_backend_test

test: csv_backend_test

csv_backend.a: csv_backend.o 
	$(AR) -rcv $@ $?

csv_backend_test: csv_backend.c 
	$(CC) $(CFLAGS) -D_TEST -o $@ $?

