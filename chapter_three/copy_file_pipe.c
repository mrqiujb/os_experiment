#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READ_END 0
#define WRITE_END 1
// tow arg
// 源文件的路径 目的文件的路径

void error_handler(int error_code)
{
    switch (error_code)
    {
    case 1:
        printf("参数错误！\n");
        break;
    case 2:
        printf("路径错误！\n");
        break;
    case 3:
        printf("fork错误！\n");
        break;
    case 4:
        printf("pipe错误！\n");
        break;

    default:
        break;
    }
    exit(-1);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        error_handler(1);
    }
    // 检查地址是否正确
    for (size_t i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    int srcfd = open(argv[1], O_RDONLY);

    if (srcfd == -1)
    {
        error_handler(2);
    }
    int dstfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0727);
    int pid = fork();
    int fd[2];
    if(pipe(fd)<0)
    {
        error_handler(4);
    }
    if (pid < 0)
    {
        error_handler(3);
    }
    else if (pid == 0) // child
    {
        close(fd[WRITE_END]);
        char buf[512];
        int size=0;
        while (size=read(fd[READ_END],buf,512)!=0)
        {
            write(dstfd, buf, size);
        }
    }
    else if (pid > 0) // father
    {
        close(fd[READ_END]);
        char buf[512];
        int size=0;
        while ((size = read(srcfd, buf, sizeof(char) * 512)) > 0)
        {
            write(fd[WRITE_END], buf, size);
        }
    }

    exit(0);
}