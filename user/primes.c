#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int *parent_fd);

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        printf("para num is err");
        exit(-1);
    }
    
    int fd[2];
    pipe(fd);//创建管道
        
    int pid = fork();
    if(pid > 0)
    {
        close(fd[0]);//关闭读端
        int i = 0;
        //主进程将2到35全部写入管道中
        for(i = 2; i < 36; i++)
        {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(0);
        exit(0);
    } else if(pid == 0) {
        prime(fd);
        exit(0);
    } else {
        printf("fork error");
        exit(-1);
    }

    return 0;
}

void prime(int *parent_fd)
{
    //读取质数，如果没有数据就返回
    int prime_num ;
    close(parent_fd[1]);//关闭写管道
    if(read(parent_fd[0], &prime_num, sizeof(int)) == 0)
    {
        close(parent_fd[0]);
        exit(0);
        //return;
    }
    printf("prime %d\n",prime_num);

    int fd[2];
    pipe(fd);
    int pid = fork();

    if(pid > 0)
    {
        close(fd[0]);//关闭右边的读通道
        int num;
        while(read(parent_fd[0], &num, sizeof(int)) != 0)
        {
            if(num % prime_num != 0)
            {
                write(fd[1], &num, sizeof(int));
            }
        }
        close(fd[1]);
        wait(0);
        exit(0);
    } else if ( pid == 0) {
        prime(fd);
        //wait(0);
        exit(0);
    } else {
        printf("fork error");
        exit(-1);
    }
    //close(fd[0]);
    exit(0);
}