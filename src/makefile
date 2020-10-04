all: greedy1.c
	gcc -Wall -o greedy1 greedy1.c

greedy1.o: greedy1.c
	gcc -Wall -fsanitize=address -c greedy1.c

clean:
	rm -f greedy1.o greedy1
