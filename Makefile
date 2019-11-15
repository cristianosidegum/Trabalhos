all: main

main: tela.o main.o 
	gcc -o main tela.o main.o -lm

main.o: main.c
	gcc -c main.c

tela.o: tela.c
	gcc -c tela.c

clear:
	rm -rf *.o

rmproper: clean
	rm -rf main

