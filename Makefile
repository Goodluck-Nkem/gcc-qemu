# The compiler versions may not match yours, just replace with yours.

none: 
	@echo "No target chosen!"

all: amd64 x86 arm aarch64 mips mips64 riscv64

amd64: x86_64
x86_64: 
	x86_64-linux-gnu-gcc-13 -g main.c -o amd64.prog

i686: i386
i386: x86
x86:
	i686-linux-gnu-gcc-13 -g main.c -o x86.prog

arm64: aarch64
aarch64:
	aarch64-linux-gnu-gcc-13 -g main.c -o arm64.prog

arm32: arm
arm:
	arm-linux-gnueabi-gcc-13 -g main.c -o arm32.prog

mips: mips32
mips32:
	mips-linux-gnu-gcc-12 -g main.c -o mips32.prog

mips64:
	mips64-linux-gnuabi64-gcc-12 -g main.c -o mips64.prog

riscv64:
	riscv64-linux-gnu-gcc-13 -g main.c -o riscv64.prog

clean:
	rm -f ${wildcard *.prog}

