#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int ac, char **av)
{
    int     uid;
    int     gid;
    char    *args[2];

    if (atoi(av[1]) == 423)
    {
        args[0] = strdup("/bin/sh");
        args[1] = 0;

        uid = geteuid();
        gid = getegid();
        setresgid(gid, gid, gid);
        setresuid(uid, uid, uid);
        execv("/bin/sh", args);
    }
    else
    {
        fwrite("No !\n", 1, 5, stderr);
    }
    return (0);
}
