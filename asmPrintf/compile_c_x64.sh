#!/bin/bash
gcc myPrint_x64.c -o myPrint_x64

#可以将上面命令替换为下面三行，生成的可执行文件更小,并且没有任何依赖,ld的-e参数指定入口
#gcc -c myPrint_x64.c -o myPrint_x64.o
#ld -s -e main myPrint_x64.o -o myPrint_x64
#rm myPrint_x64.o