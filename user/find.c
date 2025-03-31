#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define BUFSIZ 512
#define DIRSIZ 14

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void recurse_find(const char *path, const char *name)
{
    int fd;
    struct dirent de;
    struct stat st;
    char buf[BUFSIZ], *p;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (T_DIR != st.type)
    {
        return;
    }

    if (strlen(path) >= BUFSIZ)
    {
        fprintf(2, "find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(path);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
        {
            continue;
        }
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        // printf("boof %s\n", buf);
        if (stat(buf, &st) < 0)
        {
            continue;
        }
        switch (st.type)
        {
        case T_DEVICE:
        case T_FILE:
            if (strcmp(de.name, name) == 0)
            {
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            strcpy(p, de.name);
            p[strlen(de.name)] = 0;
            recurse_find(buf, name);
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find <directory> <name>\n");
        exit(-1);
    }
    recurse_find(argv[1], argv[2]);
    return 0;
}
