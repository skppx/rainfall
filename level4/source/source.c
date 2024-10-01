#include <stdio.h>
#include <stdlib.h>

int m = 0;

int p(char *ptr)
{
    return printf(ptr);
}

int n(void)
{
    int     result;
    char    buffer[520];

    fgets(buffer, 512, stdin);
    p(buffer);
    result = m;
    if (m == 16930116)
    {
        system("/bin/cat /home/user/level5/.pass");
        return (0);
    }
    return result;
}

int main(void)
{
    return n();
}
