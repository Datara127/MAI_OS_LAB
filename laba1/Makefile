СС = gcc 
FLAGS = -c -Wall -Wextra -Werror

all: arch.o
	$(CC) arch.o -o prog/laba1

arch.o: arch.c 
	$(CC) $(FLAGS) arch.c 

clean:
	rm -rf *.o build/*.*

