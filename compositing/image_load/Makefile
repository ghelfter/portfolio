# Galen Helfter
# Makefile

SHELL = /bin/sh

# Compiler and linker flags
CC = gcc
CFLAGS = -g -W -Wall -Wdeclaration-after-statement \
		 -pedantic -Wextra -O0 -std=c89 -fpic
LDFLAGS = -g -W -Wall -Wdeclaration-after-statement \
		  -pedantic -Wextra -O0 -std=c89 --shared
LIBS = -lpng

CLEANFILES = *.o *.out core.*

# Object file list
OBJECTS =	image_load.o \
			image.o

# Programs

PROGS = image_load.so

all:		$(PROGS)

image_load.so:  $(OBJECTS) $(LIBS)
				$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

# Object file dependencies
image_load.o: image_load.h image.h
image.o: image.h

.PHONY: clean
clean:
		rm -f $(PROGS) $(CLEANFILES)
