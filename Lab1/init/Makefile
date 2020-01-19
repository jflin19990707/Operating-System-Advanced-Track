all: main.o

%.o:%.c
	${GCC} ${FF} -std=gnu11 -c $< -o $@ -O2 -Wall -Wextra
clean:
	rm *.o 
