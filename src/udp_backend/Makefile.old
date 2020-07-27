CFLAGS=-ggdb -Wall

all: test udp_backend.a

clean:
	rm -f *.o *.a udp_backend_test

test: udp_backend_test

udp_backend.a: udp_backend.o 
	$(AR) -rcv $@ $?

udp_backend_test: udp_backend.c 
	$(CC) $(CFLAGS) -D_TEST -o $@ $?

