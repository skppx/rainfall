#include <stdio.h>
#include <unistd.h>
#include <string.h>

int p(void)
{
    char            buffer[64];
    const void      *printaddr;
    unsigned int    retaddr;

    fflush(stdout);
    gets(buffer);
    printaddr = (const void *)retaddr;
    if ((retaddr & 0xB0000000) == -1342177280)
    {
        printf("(%p)\n", printaddr);
        _exit(1);
    }
    puts(buffer);
    strdup(buffer);
    return (0);
}

int main(void)
{
    return p();
}
