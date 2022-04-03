PREFIX ?= /usr/local
CC ?= cc

default: libcuppa.so

install: libcuppa.so cuppa.pc
	cp cuppa.h $(PREFIX)/include
	cp libcuppa.so $(PREFIX)/lib
	cp cuppa.pc $(PREFIX)/lib/pkgconfig

libcuppa.so: cuppa.o
	$(CC) cuppa.o -shared -o libcuppa.so

cuppa.o:
	$(CC) -c -Wall -Wextra -Werror -fPIC cuppa.c -o cuppa.o

cuppa.pc:
	bash pcconf.sh $(PREFIX)
