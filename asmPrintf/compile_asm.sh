#!/bin/bash
nasm -f elf64 asmPrint.asm
ld -s -o asmPrint asmPrint.o
rm asmPrint.o