CC=gcc
XMLFLAGS=`pkg-config --libs --cflags  libxml-2.0`
BDFLAGS=-I /usr/include/mysql -l mysqlclient
CCFLAGS1=-Wall -Wredundant-decls -Wuninitialized -Wreturn-type
CCFLAGS2=-Wpedantic -O0 -Woverflow -Wno-write-strings -Wfatal-errors -Wformat-overflow  
CCFLAGS3=-Wunused-variable 

ALLFLAGS=$(BDFLAGS) $(XMLFLAGS) $(CCFLAGS1) $(CCFLAGS2) $(CCFLAGS3)

OBJS=migrate.o options.o conf.o sql.o
TARGET_FILE=migrate.c
RESULT_FILE=migrate

all: $(OBJS)
	$(CC) $(OBJS) -o $(RESULT_FILE) $(ALLFLAGS)

migrate.o:
	$(CC) migrate.c -c  $(ALLFLAGS) -I .

options.o:
	$(CC) options.c -c  $(ALLFLAGS) -I .

conf.o:
	$(CC) conf.c -c  $(ALLFLAGS) -I .

sql.o:
	$(CC) sql.c -c  $(ALLFLAGS) -I .

clear:
	rm *.o *.gcda $(RESULT_FILE)