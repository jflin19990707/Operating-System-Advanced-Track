
export
CROSS_=~/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
AS=${CROSS_}as
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy
OBJDUMP=${CROSS_}objdump
FF=-ffreestanding
NM=${GCC}-nm
MKSYSMAP=${NM} -n vmlinux.elf | grep -v '\( [aNUw] \)|\(__crc_\)|\( \$[adt]\)' > System.map
INCLUDE= -I /home/cyy/lab-4/include -I /home/cyy/lab-4/arch/arm64/include


CF=-g
CFLAG=${CF} ${INCLUDE}

ifeq ($(non_prio),1)
        CFLAG += -D non_priority
endif

all:
	${MAKE} -C arch/arm64
	${MAKE} -C init
	${LD} -T arch/arm64/kernel/vmlinux.lds.S -o vmlinux.elf init/main.o arch/arm64/kernel/mm.o arch/arm64/kernel/mms.o arch/arm64/kernel/head.o arch/arm64/kernel/function.o arch/arm64/kernel/irq.o arch/arm64/kernel/entry.o arch/arm64/kernel/sched.o arch/arm64/kernel/timer.o
	${OBJCOPY} vmlinux.elf -O binary arch/arm64/boot/Image.img
	${OBJDUMP} -t vmlinux.elf > System.map
	${MKSYSMAP}
	@echo -e '\n'Build Finished OK
non_prio:
	@make non_prio=1
run:
	@echo Launch the qemu ......
	@ ~/qemu-4.0.0/aarch64-softmmu/qemu-system-aarch64 -M raspi3 -kernel arch/arm64/boot/Image.img -nographic
debug:
	@ ~/qemu-4.0.0/aarch64-softmmu/qemu-system-aarch64 -M raspi3 -serial null -serial mon:stdio -kernel arch/arm64/boot/Image.img -S -s
clean:
	${MAKE} -C arch/arm64 clean
	${MAKE} -C init clean
	rm *.map
