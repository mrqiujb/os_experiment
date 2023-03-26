#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <sys/shm.h>
#include <string.h>
#define N 9 //
#define READ_END 0
#define WRITE_END 1
void error_handler(int code)
{
    switch (code)
    {
    case 1:
        printf("fork 失败！");
        break;
    case 2:
        printf("pipe 失败！");
        break;
    case 3:
        printf("参数错误！");
        break;
    case 5:
        printf("共享内存错误！");
        break;

    default:
        break;
    }
    exit(-1);
}
// have some bug
// 大于10的数字以char 的形式存储但是很完蛋
//
int collatz_shm(char *arg)
{
    int num = atoi(arg);
    if (num <= 0)
        error_handler(3);

    const char *path = "./";
    int proId = 1000;
    int shmKey = ftok(path, proId);
    int shmid = shmget(shmKey, 1024, IPC_CREAT | 0664); // key可以是自己设置整数
    if (shmid == -1)
    {
        error_handler(5);
    }

    int pid = fork();
    if (pid < 0)
    {
        error_handler(1);
    }
    else if (pid == 0) // child
    {
        // 2. 当前进程和共享内存关联
        void *ptr = shmat(shmid, NULL, 0);
        if (ptr == (void *)-1)
        {
            error_handler(5);
        }
        int nums[256] = {-1};
        int i = 0;
        while (num != 1)
        {
            nums[i++] = num;

            if (num % 2 == 0)
            {
                num /= 2;
            }
            else
            {
                num = 3 * num + 1;
            }
        }
        nums[i] = 1;
        memcpy(ptr, nums, sizeof(int)*256);
        exit(0);
    }
    else if (pid > 0) // father
    {

        // 2. 当前进程和共享内存关联
        void *ptr = shmat(shmid, NULL, 0);
        if (ptr == (void *)-1)
        {
            error_handler(5);
        }

        wait(NULL);
        int tem[256] = {-1};
        memcpy(tem, ptr, sizeof(int)*256);
        printf("father print:\n");
        for (size_t i = 0; i < 256; i++)
        {
            printf("%d ", tem[i]);
        }

        shmctl(shmid, IPC_RMID, NULL);
    }

    exit(0);
}
int collatz_fork(char *arg)
{
    int num = atoi(arg);
    if (num <= 0)
        error_handler(3);
    int pid = fork();
    if (pid < 0)
    {
        error_handler(1);
    }
    else if (pid == 0) // child
    {
        while (num != 1)
        {
            printf("%d ", num);
            if (num % 2 == 0)
            {
                num /= 2;
            }

            else
            {
                num = 3 * num + 1;
            }
        }
    }
    else if (pid > 0) // father
    {

        wait(NULL);
    }
    exit(0);
}
int pipe_reverse(char *arg)
{
    int fd_rx[2], fd_tx[2];
    if (pipe(fd_rx) == -1 || pipe(fd_tx) == -1)
    {
        error_handler(2);
    }
    int pid = fork();
    if (pid < 0)
    {
        error_handler(1);
    }
    else if (pid == 0) // child
    {
        char rec_buf[512];
        close(fd_rx[WRITE_END]);
        close(fd_tx[READ_END]);
        read(fd_rx[READ_END], rec_buf, 512);
        printf("child rec:%s\n", rec_buf);
        int i = 0;
        while (rec_buf[i] != '\0')
        {
            if (rec_buf[i] >= 97 && rec_buf[i] <= 122)
                rec_buf[i] = rec_buf[i] - 32;
            else if (rec_buf[i] >= 65 && rec_buf[i] <= 90)
                rec_buf[i] = rec_buf[i] + 32;
            i++;
        }
        write(fd_tx[WRITE_END], rec_buf, sizeof(rec_buf));
    }
    else if (pid > 0) // father
    {
        close(fd_rx[READ_END]);
        close(fd_tx[WRITE_END]);
        char buf[512] = "i LOve GPlNu";
        write(fd_rx[WRITE_END], buf, sizeof(buf));
        char rec_buf[512];
        read(fd_tx[READ_END], rec_buf, 512);
        printf("father rec:%s\n", rec_buf);
    }
}
int process_status(char *arg)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        error_handler(1);
    }
    else if (pid == 0) // child process
    {
        printf("child process id %d\n", getpid());
        // end
        exit(0);
    }
    else if (pid > 0) // father process
    {
        // do something no end
        printf("father process id %d\n", getpid());
        sleep(20);
    }
    // shell 使用ps命令打印不出来进程？
    system("ps -l");
}

void main(int argc, char **argv)
{
    int (*handler[])(char *arg) = {process_status, pipe_reverse, collatz_fork, collatz_shm};
    int choosen = -1;
    printf("选择习题！\n");
    scanf("%d", &choosen);
    char arg[10] = {'\0'};
    scanf("%s", &arg);
    if (choosen >= 0 && choosen < N)
    {
        handler[choosen](&arg);
    }
}