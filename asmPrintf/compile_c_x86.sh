#!/bin/bash
gcc -m32 -c myPrint_x86.c -o myPrint_x86.o
#ld的-e参数指定入口
ld -s -m elf_i386 -e main myPrint_x86.o -o myPrint_x86
rm myPrint_x86.o