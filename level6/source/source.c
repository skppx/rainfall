#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void n(void)
{
    system("/bin/cat /home/user/level7/.pass");
}

void m(void)
{
    puts("Nope");
}

int main(int ac, char **av)
{
    int (**fptr)(void);
    char *buffer;

    buffer = malloc(64);
    fptr = (int (**)(void))malloc(4);
    *fptr = (void *)m;
    strcpy(buffer, av[1]);
    (**fptr)();
    return (0);
}
