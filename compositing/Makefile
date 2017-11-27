# Galen Helfter
# Makefile

SHELL=/bin/sh

# Set up Lisp compiler
LISPC=sbcl

# Define our cleanfiles
CLEANFILES = *.o *.out *.core *.fasl $(TARFILE)

# Set up the tarfile
TARFILE = ghelfte_project1.tar.gz

# Set up subsystem definitions
SUBSYSTEMS = image_load.so ltk.fasl

# Define our main function
MAINFUNC = composite:comp-main
LOCAL = local

COREFILE = composite.core

# Will change this later to have our top level program require them, there
# should be a core file that will want these two, not all. all will then
# require the core file
all: $(COREFILE)

# ******************
#     Top level
# ******************
local: $(SUBSYSTEMS)
	$(LISPC) --noprint --eval '(compile-file "composite")' \
					   --eval '(load "composite")' \
					   --eval '(compile-file "image")' \
	                   --eval '(compile-file "image_alien")' \
	                   --eval '(compile-file "operations")' \
	                   --eval '(load "./ltk/ltk")' \
	                   --eval '(compile-file "interface")' \
	                   --eval '(sb-ext:quit)'

# Now, create the core file
$(COREFILE): $(SUBSYSTEMS) $(LOCAL)
	$(LISPC) --noprint --eval '(load "composite")' \
		               --eval '(load "./ltk/ltk")' \
				   	   --eval '(load "image")' \
	                   --eval '(load-shared-object "./image_load/image_load.so")' \
	                   --eval '(load "image_alien")' \
					   --eval '(load "operations")' \
					   --eval '(load "interface")' \
					   --eval '(sb-ext:save-lisp-and-die "$@" :toplevel '"#'"'$(MAINFUNC))'

# ******************
#     Subsystems
# ******************
# Image loading
image_load.so:
	$(MAKE) -C ./image_load
# Lisp TK bindings
ltk.fasl:
	$(MAKE) -C ./ltk

# Phony elements
.PHONY: clean tar docs
clean:
	$(MAKE) clean -C ./image_load
	$(MAKE) clean -C ./ltk
	$(MAKE) clean -C ./docs/writeup
	rm -rf $(CLEANFILES)
tar:
	rm -f $(TARFILE)
	tar -czvf $(TARFILE) ../p1

# Documentation included resulting images and a pdf writeup, which is not
# included in this github publication. The tex file & Makefile are
# included, but will not compile by default, as the images are not included.

# Documentation
#docs:
#	$(MAKE) pdf -C ./docs/writeup
#	echo "Writeup documents created. They can be found ./docs/writeup"
