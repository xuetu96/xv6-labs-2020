#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 1)
    {
        printf("parameter num is invalid");
        exit(-1);
    }

    //创建两个管道的文件描述符
    int fd_ping[2];
    int fd_pong[2];

    int pid_ping = 0;
    int pid_pong = 0;

    //char buf[100];
    // buf[0] = 'a';
    
    pipe(fd_ping);
    pipe(fd_pong);

    int pid = fork();

    if(pid > 0)
    {
        close(fd_ping[0]);
        write(fd_ping[1], "a", sizeof(char));

        close(fd_pong[1]);
        read(fd_pong[0], "a", sizeof(char));

        pid_pong = getpid();
        printf("%d: received pong\n", pid_pong);

    } else if(pid == 0) {

        close(fd_ping[1]);
        read(fd_ping[0], "a", sizeof(char));

        pid_ping = getpid();
        printf("%d: received ping\n", pid_ping);

        close(fd_pong[0]);
        write(fd_pong[1], "a", sizeof(char));

    } else {
        printf("fork error");
        exit(-1);
    }

    exit(0);
}