# Makefile for lab11 mini project
# 	"make" creates executable named "project"

all: 
	gcc -c project.c
	gcc -c minefunc.c
	gcc -o project project.o minefunc.o gfx2.o -lX11 

clean:
	rm -f project.o
	rm minefunc.o
	rm project
