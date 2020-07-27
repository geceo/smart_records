CFLAGS=-D_WITH_INTERACTIVE_TERMINAL -ggdb -Wall

smart_records_standalone: smart_records.c mysql_backend/mysql_backend.a csv_backend/csv_backend.a
	$(CC) $(CFLAGS) -D_HAS_MYSQL -D_HAS_CSV -D_HAS_UDP `pkg-config --cflags --libs mysqlclient` -o $@ $?

mysql_backend/mysql_backend.a:
	$(MAKE) -C mysql_backend mysql_backend.a

csv_backend/csv_backend.a:
	$(MAKE) -C csv_backend csv_backend.a
	
clean:
	rm -rf *.o smart_records_standalone
	make -C mysql_backend clean
	make -C csv_backend clean

