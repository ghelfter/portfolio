# Galen Helfter (ghelfter@gmail.com, ghelfte@clemson.edu)
# CPSC 4040
# Project 5
# Makefile

SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o .cpp .h

C_CC = gcc
CC = g++

CFLAGS = -c -g -O2 -std=c++11 -Wall
C_CFLAGS = -c -g -O2 -std=gnu11 -Wall
LINKFLAGS = -g -O2 -std=c++11 -Wall
C_LINKFLAGS = -g -O2 -std=gnu11 -Wall
LINKOPTION =	-o

CLEANFILES = *.o *.out core.*

PROGS = cagify

MAIN = cagifier.cpp 

OBJECTS =	ghelfter_vector.o \
		ghelfter_matrix.o \
		initialize.o \
		user_input.o \
		g_float.o \
		rendering.o \
		warpline.o \
		img_convert.o \
		warping.o \
		masking.o
SOURCES =	ghelfter_vector.cpp \
		ghelfter_matrix.cpp \
		initialize.cpp \
		user_input.cpp \
		g_float.cpp \
		rendering.cpp \
		warpline.cpp \
		img_convert.cpp \
		warping.cpp \
		masking.c

# Put all the fancy OpenImageIO stuff and library things that he had in sample
# makefile.

# Credit to Joshua Levine for the next 3 lines of library stuff
OIIO_INC =
OIIO_LIB = -lOpenImageIO
OPENGL_LIB = -lglut -lGL -lGLU
SDL_LIB = -lSDL2 # -lSDL2_image

OTHER_LIB = -lm

LIBS = $(OTHER_LIB) $(SDL_LIB) $(OPENGL_LIB) $(OIIO_LIB)

all:		$(PROGS)

.cpp.o:		$(HEADERS)
			$(CC) $(CFLAGS) $(OPTIONS) $<

.c.o:		$(HEADERS)
			$(C_CC) $(C_CFLAGS) $(OPTIONS) $<

cagify:		$(MAIN) $(OBJECTS) $(LIBS) $(SOURCES)
			$(CC) $(LINKOPTION) $@ $(MAIN) $(OBJECTS)\
			$(LINKFLAGS) $(LIBS) 

clean:
		rm -f $(PROGS) $(CLEANFILES)
