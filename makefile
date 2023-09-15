all: pmandel

pmandel: mandel.o pmandel.o
	g++ -Wall -o pmandel pmandel.o mandel.o -lm

pmandel.o: pmandel.cc mandel.h
	g++ -g -c -Wall pmandel.cc 

mandel.o: mandel.cc mandel.h
	g++ -g -c -Wall mandel.cc 

clean:
	rm pmandel *.o

piclean:
	rm pmandel *.o *.ppm
