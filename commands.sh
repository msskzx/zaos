#assemble bootload
nasm bootload.asm

# image file of a floppy disk
dd if=/dev/zero of=floppya.img bs=512 count=2880

# copy bootload to the beginning of floppya.img
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

# compile kernel.c
bcc -ansi -c -o kernel.o kernel.c

# assemble kernel.asm
as86 kernel.asm -o kernel_asm.o

# link kernel.c with kernel.o
ld86 -o kernel -d kernel.o kernel_asm.o

# copy kernel.c to floppya.img at sector 3
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

# excute
chmod +x commands.sh
