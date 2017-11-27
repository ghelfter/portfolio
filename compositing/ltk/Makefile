# Galen Helfter
# Makefile

SHELL = /bin/sh

# Set up Lisp compiler
LISPC=sbcl
CFLAGS=--noinform

# Define our cleanfiles
CLEANFILES = *.o *.out *.core *.fasl

# Programs
PROGS = ltk.fasl

all: $(PROGS)

ltk.fasl:
	$(LISPC) $(CFLAGS) --eval '(progn (compile-file "ltk") (sb-ext:quit))'

.PHONY: clean
clean:
		rm -f $(PROGS) $(CLEANFILES)
