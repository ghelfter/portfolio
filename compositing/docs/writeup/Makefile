# Galen Helfter
# Makefile
# 
# Makefile used for compiling documents using LaTeX

SHELL = /bin/sh

NAME = ghelfte_p1_writeup

TARFILE = $(NAME).tar.gz

all: $(NAME).dvi

# Compile the tex file
$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex

# Clear all of the created files
.PHONY: clean
clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).pdf $(TARFILE)

# Display the document
.PHONY: display
display:
	xdvi $(NAME).dvi

.PHONY: pdf
pdf: $(NAME).tex
	pdflatex $(NAME).tex

# Easy way to edit the document
.PHONY: edit
edit:
	vim $(NAME).tex

.PHONY: tar
tar:
	rm -f $(TARFILE)
	tar -czvf $(TARFILE) ./*
