all: pmandel

pmandel: mandel.o mandelPlot.o
	g++  -o pmandel mandelPlot.o mandel.o -lm

mandelPlot.o: mandelPlot.cc mandel.h
	g++ -g -c mandelPlot.cc 

mandel.o: mandel.cc mandel.h
	g++ -g -c mandel.cc 

clean:
	rm pmandel *.o

piclean:
	rm pmandel *.o *.ppm
