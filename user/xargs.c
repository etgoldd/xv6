#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXARG_LEN 1024

void execute_program(const char *prog_path, char **args)
{
    int pid = fork();
    if (pid == 0)
    {
        // Child process
        exec(prog_path, args);
        exit(0);
    }
    else
    {
        // Parent process
        wait(&pid);
    }
    return;
}

int main(int argc, char const *argv[])
{
    int index = 0;
    char buf[MAXARG_LEN], *p, cur, *args[MAXARG];
    p = buf;
    for (int i = 1; i < argc; i++)
    {
        args[i - 1] = (char *)malloc(strlen(argv[i]));
        strcpy(args[i - 1], argv[i]);
    }

    while (read(0, &cur, 1) == 1)
    {
        if (cur == '\n')
        {
            *p = 0;
            args[argc - 1] = (char *)malloc(strlen(buf));
            strcpy(args[argc - 1], buf);
            index++;
            p = buf;

            execute_program(argv[1], args);
            free(args[argc - 1]);
            continue;
        }
        *p = cur;
        p++;
        if (p == buf + MAXARG_LEN)
        {
            fprintf(2, "xargs: input too long\n");
            exit(0);
        }
    }
    return 0;
}
