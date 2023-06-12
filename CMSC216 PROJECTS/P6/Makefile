# 1. all: make all executables
#2. clean: delete all object files and executables
#3. lexer.o, parser.tab.o, executor.o, and d8sh.o, the object files for the parsing code and shell code
#4. d8sh: the executable created by linking the parsing object file with the shell object files and linked with
#the readline library using -lreadline.

CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
         -pedantic-errors -fstack-protector-all -Wextra
PROGS = d8sh

all: $(PROGS)

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

parser.tab.o: parser.tab.c parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

executor.o: executor.c executor.h
	$(CC) $(CFLAGS) -c executor.c

d8sh: d8sh.o lexer.o parser.tab.o executor.o
	$(CC) $(CFLAGS) -lreadline -o d8sh d8sh.o lexer.o executor.o parser.tab.o

d8sh.o: d8sh.c 
	$(CC) $(CFLAGS) -c d8sh.c

.PHONY: clean 

clean:
	rm -f *.o$(PROGS) a.out

