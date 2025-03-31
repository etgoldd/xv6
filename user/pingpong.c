#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int child_pipefds[2];
    int parent_pipefds[2];
    pipe(child_pipefds);
    pipe(parent_pipefds);
    char buf[1];
    int pid = fork();
    if (pid == 0)
    {
        // Child process
        read(parent_pipefds[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(child_pipefds[1], "G", 1);

        exit(0);
    }
    else
    {
        // Parent process
        write(parent_pipefds[1], "G", 1);
        read(child_pipefds[0], buf, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
    return 0;
}