#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_VALUE 280

void modulo_process(int read_fd)
{
    int modulo;
    int current;
    read(read_fd, &modulo, sizeof(int));
    printf("prime %d\n", modulo);
    if (0 == read(read_fd, &current, sizeof(int)))
    {
        return;
    }
    int right_pipe[2];
    pipe(right_pipe);
    int pid = fork();
    if (pid != 0)
    {
        // Parent process
        close(right_pipe[0]);
        write(right_pipe[1], &current, sizeof(int));
        while (0 != read(read_fd, &current, sizeof(int)))
        {
            if (current % modulo == 0)
            {
                continue;
            }
            write(right_pipe[1], &current, sizeof(int));
        }
        close(right_pipe[1]);
        close(read_fd);
        wait(&pid);
        exit(0);
    }
    else
    {
        // Child process
        close(right_pipe[1]);
        close(read_fd);
        modulo_process(right_pipe[0]);
    }
}

int main(int argc, char const *argv[])
{
    int pipe_fds[2];

    pipe(pipe_fds);
    int pid = fork();
    if (pid != 0)
    {
        // Parent process
        close(pipe_fds[0]);
        for (int i = 2; i < MAX_VALUE; i++)
        {
            write(pipe_fds[1], &i, sizeof(int));
        }
        close(pipe_fds[1]);
        wait(&pid);
    }
    else
    {
        // Child process
        close(pipe_fds[1]);
        modulo_process(pipe_fds[0]);
    }

    return 0;
}
