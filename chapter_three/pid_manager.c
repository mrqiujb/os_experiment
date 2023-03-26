#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000
#define MAP_LEN 588


//5000-300=4700 < 1024*5/8 = 
static char pid_map[MAP_LEN];;



int allocate_map(void)
{
    //加锁
    memset(pid_map,0,MAP_LEN);
    
    for (size_t i = 0; i < MAP_LEN; i++)
    {
        if(pid_map[i]!=0)
        printf("init error!\n");
    }
    
}

int allocate_pid(void)
{
    int pid=-1;
    for (size_t i = 0; i < MAP_LEN; i++)
    {
        if(pid_map[i]!=0xFF)//find first can allocate char
        {
            for (size_t j = 0; j < 8; j++)
            {
                // pid_map | 1111..10000 1111..11000 
                // 如果第四位是0则不相等 如果是1则相等
                // 从左向右找到第一个不相等的就是第一个pid
                //printf("0x%x\n",~(1L<<j));
                //printf("0x%x\n",pid_map[i] | ~(1L<<j));
                u_int8_t mask=(u_int8_t)pid_map[i] | ~(1<<j);
                if((mask) != 0xFF)
                {
                    pid_map[i]=pid_map[i] | 1<<j;
                    //find
                    pid=i*8+j+300;
                    if(pid>5000) return -1;
                    return pid;
                }
            }
        }
    }
    return -1;
}

int release_pid(int pid)
{
    if(pid<MIN_PID || pid>MAX_PID) return -1;
    pid=pid-300;
    int i=0,j=0;
    i=pid%8;
    j=pid/8;
    pid_map[j]=pid_map[j]& (~(1L<<i));
    return 1;
}


int main()
{
    //test
    allocate_map();
    for (size_t i = 0; i < 1000; i++)
    {
        allocate_pid();
    }
    int pid=allocate_pid();
    release_pid(pid);
}