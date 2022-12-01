CFLAGS = -std=c99 -Wall -g
all: lattes.o model.o math.o chart.o summarization.o
	gcc $(CFLAGS) lattes.o model.o math.o chart.o summarization.o -o lattes 

lattes.o: lattes.c
	gcc $(CFLAGS) -c lattes.c

model.o: model.c
	gcc $(CFLAGS) -c model.c

math.o: math.c
	gcc -c math.c

chart.o: chart.c
	gcc -c chart.c

summarization.o: summarization.c
	gcc -c summarization.c

clean:
	rm *.o *.temp

purge: clean
	rm lattes