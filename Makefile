OBJS = main.o simulator.o
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

ctags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

report: report.tex
	pdflatex report.tex

clean:
	rm -rf *.o psim report.pdf
