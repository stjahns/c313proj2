OBJS = main.o simulator.o station.o tdmstation.o
CC = g++
CFLAGS = -Wall -c -g
LFLAGS = -Wall -lm

default:
	make psim

psim: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

main.o: main.cpp simulator.h
	$(CC) $(CFLAGS) $<

simulator.o: simulator.cpp simulator.h
	$(CC) $(CFLAGS) $<

station.o: station.cpp station.h
	$(CC) $(CFLAGS) $<

tdmstation.o: tdmstation.cpp tdmstation.h
	$(CC) $(CFLAGS) $<

ctags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

report: report.tex
	pdflatex report.tex

clean:
	rm -rf *.o psim report.pdf

run:
	./psim T 5 0.01 10 5 1 2 3 4 5
