#include <stdio.h>
#include <stdlib.h>

int m = 0;

int v(void)
{
    int     result;
    char    buffer[520];
    fgets(buffer, 512, stdin);
    printf(buffer);
    result = m;
    if (m == 64)
    {
        fwrite("Wait what?!\n", 1, 12, stdout);
        system("/bin/sh");
        return (0);
    }
    return result;
}

int main(void)
{
    return v();
}
