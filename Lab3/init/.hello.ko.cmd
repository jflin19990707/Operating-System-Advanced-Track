cmd_/home/cyy/lab-1/init/hello.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds  --build-id  -o /home/cyy/lab-1/init/hello.ko /home/cyy/lab-1/init/hello.o /home/cyy/lab-1/init/hello.mod.o ;  true