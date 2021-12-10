int main(int argc, char* argv[])
{
    char *str = "hello world!\n";
    asm volatile
    (
        "mov $4,%%eax\n\t"//系统调用号
        "mov $1,%%ebx\n\t"//文件描述符
        "mov $13,%%edx\n\t"//字符串长度
        "int $0x80\n\t"
        "mov $1,%%eax\n\t"//系统调用号
        "mov $0,%%ebx\n\t"//退出错误码
        "int $0x80\n\t"::"c"(str)
    );

    return 0;
}