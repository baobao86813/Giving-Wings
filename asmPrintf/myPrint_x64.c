int main(int argc, char* argv[])
{
    char *str = "hello world!\n";
    asm volatile
    (
        "mov $1,%%rax\n\t"//系统调用号
        "mov $1,%%rdi\n\t"//文件描述符
        "mov $13,%%rdx\n\t"//字符串长度
        "syscall\n\t"
        "mov $60,%%rax\n\t"//系统调用号
        "mov $0,%%rdi\n\t"//退出错误码
        "syscall\n\t"::"S"(str)
    );

    return 0;
}