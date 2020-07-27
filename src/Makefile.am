SUBDIRS = 
lib_LTLIBRARIES = libsmartrecords.la
libsmartrecords_la_SOURCES = smart_records.c
libsmartrecords_la_LIBADD = 

if HAS_MYSQL
SUBDIRS += mysql_backend
libsmartrecords_la_LIBADD += mysql_backend/libmysql_backend.la
endif

if HAS_UDP
SUBDIRS += udp_backend
libsmartrecords_la_LIBADD += udp_backend/libudp_backend.la
endif

if HAS_CSV
SUBDIRS += csv_backend
libsmartrecords_la_LIBADD += csv_backend/libcsv_backend.la
endif
