objectsA = head.o entry.o
objectsB = timer.o irq.o
all: ${objectsA} ${objectsB}

${objectsA}:%.o:%.S
	${AS} -c ${INCLUDE} $< -o $@

timer.o : timer.c
	${GCC} ${INCLUDE} -c timer.c -o timer.o
irq.o : irq.c
	${GCC} -c ${INCLUDE} irq.c -o irq.o

clean:
	rm *.o
