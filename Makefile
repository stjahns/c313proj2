OBJS = main.o simulator.o station.o tdmstation.o ibstation.o tbebstation.o pbstation.o conf_int.o
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

pbstation.o: pbstation.cpp pbstation.h
	$(CC) $(CFLAGS) $<

ibstation.o: ibstation.cpp ibstation.h
	$(CC) $(CFLAGS) $<

tbebstation.o: tbebstation.cpp tbebstation.h
	$(CC) $(CFLAGS) $<

conf_int.o: conf_int.cpp conf_int.h
	$(CC) $(CFLAGS) $<

ctags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

report: report.tex
	pdflatex report.tex

clean:
	rm -rf *.o psim report.pdf

run:
	./psim T 5 0.01 1000 1 1
