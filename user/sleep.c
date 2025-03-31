#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define USAGE_ERROR_MSG "Usage: sleep <int>\n"

int main(int argc, char const *argv[])
{

    if (argc != 2)
    {
        write(1, USAGE_ERROR_MSG, strlen(USAGE_ERROR_MSG));
        exit(0);
    }

    int ticks = atoi(argv[1]);
    sleep(ticks);
    exit(0);
}
