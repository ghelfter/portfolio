NAME = resume

all: $(NAME).dvi

# Compile the tex file
$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex

$(NAME).tex: res.cls

# Clear all of the created files
.PHONY: clean
clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux

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
