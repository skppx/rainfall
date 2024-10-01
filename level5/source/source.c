#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av, char **envp);

int o(int ac, char **av, char **envp)
{
    system("/bin/sh");
    _exit(1);
    return main(ac, av, envp);
}

int n(int ac, char **av, char **envp)
{
    char    buffer[520];

    fgets(buffer, 512, stdin);
    printf(buffer);
    exit(1);
    return o(ac, av, envp);
}

int main(int ac, char **av, char **envp)
{
    return n(ac, av, envp);
}
