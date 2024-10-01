#include <stdio.h>
#include <stdlib.h>

int run(void)
{
    fwrite("Good... Wait What?\n", 1, 19, stdout);
    return system("/bin/sh");
}

int main(int ac, char **av)
{
    int v4;

    gets(&v4);
    return (0);
}
