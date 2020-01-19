objectsA = head.o entry.o mms.o irq.o
objectsB = timer.o irq.o mm.o sched.o function.o
all: ${objectsA} ${objectsB}

${objectsA}:%.o:%.S
	${GCC} -c ${INCLUDE} $< -o $@

timer.o : timer.c
	${GCC} ${CFLAG} -c timer.c -o timer.o
irq.o : irq.c
	${GCC} -c ${CFLAG} irq.c -o irq.o
mm.o : mm.c
	${GCC} -c ${CFLAG} mm.c -o mm.o
sched.o : sched.c
	${GCC} -c ${CFLAG} sched.c -o sched.o
function.o : function.c
	${GCC} -c ${CFLAG} function.c -o function.o
clean:
	rm *.o
