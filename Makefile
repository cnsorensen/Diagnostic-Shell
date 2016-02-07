# Makefile
# Makefile for CSC456 Prog 1: Diognostic Shell

# Author: Christine Sorensen
# Class: CSC456: Operating Systems
# Date: Spring 2016

# Usage: make target1 target2 ...

#------------------------------------------------------------------------------

# GNU C compiler and linker:
LINK = gcc

# Preprocessor and compiler flags (turn on warning, optimization, and debugging):
# CPPFLAGS = <preprocessor flags go here>

CFLAGS = -Wall -O -g
CXXFLAGS = $(CFLAGS)

#---------------------------------------

# Targets
all: dsh

#------------------------------------------------------------------------

dsh: dsh.o
	$(LINK) -o $@ $^ $(LIBS)

#------------------------------------------------------------------------

clean:
	rm -f *.o *~ core

cleanall:
	rm -f *.o *~ core dsh
