
export
CROSS_=~/Desktop/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf/bin/aarch64-elf-
AS=${CROSS_}as
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy
OBJDUMP=${CROSS_}objdump
FF=-ffreestanding
NM=${GCC}-nm
MKSYSMAP=${NM} -n vmlinux.elf | grep -v '\( [aNUw] \)|\(__crc_\)|\( \$[adt]\)' > System.map
INCLUDE= -I ../include


CF= -c -Wall -nostdlib -nostdinc -fno-builtin
CFLAG=${CF} ${INCLUDE}
all:
	
	${MAKE} -C init
	${MAKE} -C arch/arm64
	${GCC} -T arch/arm64/kernel/vmlinux.lds.S -o vmlinux.elf ${FF} -O2 -nostdlib arch/arm64/kernel/head.o init/main.o -lgcc
	${OBJCOPY} vmlinux.elf -O binary arch/arm64/boot/Image.img
	${OBJDUMP} -t vmlinux.elf > System.map
	${MKSYSMAP}
	@echo -e '\n'Build Finished OK
run:
	@echo Launch the qemu ......
	@ ~/qemu-4.0.0/aarch64-softmmu/qemu-system-aarch64 -M raspi3 -serial stdio -kernel vmlinux.elf
clean:
	${MAKE} -C arch/arm64 clean
	${MAKE} -C init clean
