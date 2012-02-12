OBJS = ass1.o simulator.o
CC = g++
CFLAGS = -Wall -c -g
LFLAGS = -Wall -lm

default:
	make esim
	#make ctags

esim: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

ass1.o: ass1.cpp
	$(CC) $(CFLAGS) $<

simulator.o: simulator.cpp
	$(CC) $(CFLAGS) $<

ctags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

run:
	./esim 100 1 8000 0.00005 10000000 5 1 2 3 4 5

report: report.tex
	pdflatex report.tex

clean:
	rm -rf *.o ass1 report.pdf output/*
