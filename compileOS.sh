#assemble bootload
nasm bootload.asm

# image file of a floppy disk
dd if=/dev/zero of=floppya.img bs=512 count=2880

#
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc

#
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

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

# compile
gcc -o loadFile loadFile.c

bcc -ansi -c -o shell.o shell.c
as86 lib.asm -o lib.o
ld86 -o shell -d shell.o lib.o



./loadFile shell
./loadFile message.txt
./loadFile tstpr2
./loadFile tstprg
./loadFile hello1
./loadFile hello2
./loadFile phello
# excute
# chmod +x compileOS.sh


