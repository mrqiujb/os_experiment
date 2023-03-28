#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80
#define MAX_CHAR 10
#define MAX_QUEEN 11

// 使用队列保存历史命令 但不包含shell内置命令
struct Command_QUEEN
{
    char *data[MAX_QUEEN];
    int head, end;
};

#pragma region 静态变量定义

static struct Command_QUEEN queen;
static int is_multi = 0; // 是否父进程等待子进程
static char *args[MAX_LINE / 2 + 1];
const char *sep = " "; // 分割标准

#pragma endregion

#pragma region 队列函数

void init_queen()
{
    queen.end = 0;
    queen.head = 0;
}
// 判断队列是否满
int isFull()
{
    return (queen.end + 1) % MAX_QUEEN == queen.head;
}

// 判断队列是否为空
int isEmpty()
{
    return queen.end == queen.head;
}

// 添加数据到队列

void addQueue(char *p)
{
    // 判断队列是否满
    if (isFull())
    {
        return;
    }
    queen.data[queen.end] = p;
    // 将rear后移，必须考虑取模
    queen.end = (queen.end + 1) % MAX_QUEEN;
}

// 出队列
int getQueue()
{
    // 判断队列是否空
    if (isEmpty())
    {
        return -1;
    }
    // 因为修改了保存方式使用了malloc 那么需要在移除队列的时候free

    free(queen.data[queen.head]);

    queen.head = (queen.head + 1) % MAX_QUEEN;
    return 0;
}

int add_command(char *p)
{
    if (isFull())
    {
        getQueue();
    }
    char *tem = (char *)malloc(sizeof(char) * strlen(p));
    strcpy(tem, p);
    addQueue(tem);
    return 1;
}
char *get_command(int index)
{
    return queen.data[(queen.head + index) % MAX_QUEEN];
}

#pragma endregion

#pragma region shell实现函数

char *get_command_index(int index)
{
    int j = 1; // to show how num
    for (int i = queen.head; i != queen.end; i = ((i + 1) % MAX_QUEEN))
    {
        if (j++ == index)
            return queen.data[i];
    }
    return NULL;
}

int history()
{
    int j = 1; // to show how num
    for (int i = queen.head; i != queen.end; i = ((i + 1) % MAX_QUEEN))
    {
        printf("%d:%s\n", j++, queen.data[i]);
    }
    return 1;
}

int shell_func(char *arg)
{
    int flag = 0;
    if (strcmp(arg, "exit") == 0)
        exit(0);
    else if (strcmp(arg, "!!") == 0)
    {
        flag = history();
    }
    else if (strlen(arg) == 2 && arg[0] == '!')
    {
        int index = arg[1] - '0';
        if (index < 1 || index > 9)
        {
            printf("数字错误！\n");
            return 1;
        }
        char *command = get_command_index(index);
        if (command == NULL)
        {
            printf("输入错误！\n");
            return 1;
        }

        split_command(command);
        exec_command();
        return 1;
    }
    return flag;
}

#pragma endregion

#pragma region 主要功能实现函数

int read_command(char *in_buf)
{
    int char_count = 0;
    int ch = 0; // 临时字符
                // 读取字符 直到换行 如果使用scanf读取 那么读到空格会停止 \n检测不出来
                // 采用getchar读取所有字符直到\n停止 然后strtok分割

    while ((ch = getchar()) != '\n' && char_count < MAX_LINE)
        in_buf[char_count++] = ch;
    if (strcmp(in_buf, "") == 0)
        return 1; // 如果没有输入则进入下一次
    return 0;
}

int split_command(char *in_buf)
{
    int arg_count = 0;
    // 分割字符 以空格 放入args
    for (args[arg_count] = strtok(in_buf, " "); args[arg_count] != NULL; args[arg_count] = strtok(NULL, " "))
        arg_count++;
    return arg_count;
}

void exec_command()
{
    int pid = fork();
    if (pid < 0)
    {
        printf("fork 失败！\n");
        exit(-1);
    }
    else if (pid == 0) // child
    {
        if (execvp(args[0], args) == -1)
        {
            perror("osh>error "); // 打印出错原因
        }
        exit(0);
    }
    else if (pid > 0) // father
    {
        if (is_multi == 0)
            wait(NULL);
    }
}

void is_muitl_exec(int arg_count)
{
    if (strcmp("&", args[arg_count - 1]) == 0)
    {
        is_multi = 1; 
        args[arg_count - 1] = NULL; //删除 &
    }
}

#pragma endregion

int main(void)
{
    while (1)
    {
        printf("osh>");
        fflush(stdout); // 清空输出缓存

        char in_buf[MAX_LINE] = {'\0'}; // 读取的字符串
        int arg_count = 0;              // 参数数量


        if(read_command(in_buf)) 
            continue;

        char *save_in_buf = (char *)malloc(sizeof(char) * strlen(in_buf)); // 防止split修改

        arg_count = split_command(in_buf);
        if (shell_func(args[0]) == 1) // 不保存shell支持的命令  // shell function support
            continue;               
        add_command(save_in_buf);
        is_muitl_exec(arg_count);
        exec_command(args);

        is_multi = 0;
    }
}