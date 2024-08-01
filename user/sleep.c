#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("argument num is invalid");
        exit(-1);
    }

    int num_of_ticks = 0;
    num_of_ticks = atoi(argv[1]);
    if (sleep(num_of_ticks) == -1)
    {
        printf("sleep error, can't sleep");
        exit(-1);
    }

    exit(0);
}