# portfolio

This repository contains certain projects, which shall be available for
use as a portfolio for job applications.

Currently, it contains:

4040Final - Project in which an image transformation was programmed
            using SDL as its display mechanism

batch\_render - Script written in Perl to push a set of renders off to a set
                of machines. Came from a problem we ran into at the Clemson
                DPA program, where our pipeline would not suffice to do what
                was needed. Has been repurposed by me to work on my own
                machines.

compositing - Project written to do many compositing operations. It uses the
              image\_load code as well as ltk, the tk bindings for Lisp. The
              project is written in Common Lisp, and uses the FFI to integrate
              with the C image reading code.

eye\_animation - Includes the most recent version of my blink animation code.
                 This code procedurally applies a blink, using a more
                 appropriate curve than a typical quadratic that an animator
                 might use. It is written in Python and expects to be run in
                 AutoDesk Maya.

image\_load - Contains code for reading PNG images into an image struct,
              using libPNG.

obj\_parser - Written in C, this code will read in a Wavefront OBJ file.

ogg\_load - Contains code for using libvorbis to read a .ogg file into
            a byte buffer.

pipeline\_util - This folder contains various utility scripts that I have
                 written for use with the Clemson DPA production pipeline.
                 Some are not explicit to the pipeline and will run outside
                 it.

vorbis\_utilities - Contains scripts for editing and working on the comments
                    of an ogg/vorbis file.
