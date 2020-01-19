objectsA = head.o entry.o mm.o irq.o
objectsB = timer.o irq.o mm.o sched.o
all: ${objectsA} ${objectsB}

${objectsA}:%.o:%.S
	${AS} -c ${INCLUDE} $< -o $@

timer.o : timer.c
	${GCC} ${CFLAG} -c timer.c -o timer.o
irq.o : irq.c
	${GCC} -c ${CFLAG} irq.c -o irq.o
mm.o : mm.c
	${GCC} -c ${CFLAG} mm.c -o mm.o
sched.o : sched.c
	${GCC} -c ${CFLAG} sched.c -o sched.o
clean:
	rm *.o
