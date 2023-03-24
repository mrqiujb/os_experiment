#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//tow arg
//源文件的路径 目的文件的路径

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
    
    default:
        break;
    }
    exit(-1);
}

int main(int argc ,char ** argv)
{
    if(argc != 3)
    {
        error_handler(1);
    }
    //检查地址是否正确
    for (size_t i = 0; i < argc; i++)
    {
        printf("%s\n",argv[i]);
    }
    int srcfd=open(argv[1],O_RDONLY);
    
    if(srcfd==-1)
    {
        error_handler(2);
    }
    int dstfd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0727);
    char buf[1024];
    int size=0;
    while ((size=read(srcfd,buf,sizeof(char)*1024))>0)
    {
        printf("%s",buf);
        write(dstfd,buf,size);
    }
    write(dstfd,buf,size);
    exit(0);
}