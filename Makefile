FLAGS ?= -Wall -fsanitize=undefined -fsanitize=address -g -pedantic
CC ?= gcc

Main: main.o HashMap.o
	$(CC) $(FLAGS) -o Main main.o HashMap.o

main.o: main.c
	$(CC) $(FLAGS) -c main.c

HashMap.o: HashMap.h HashMap.c
	$(CC) $(FLAGS) -c HashMap.c

test:
	./Main

libhashmap.a: HashMap.o
	ar rcs libhashmap.a HashMap.o

clean:
	rm *.o ./Main ./libhashmap.a