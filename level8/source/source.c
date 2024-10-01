#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char    *auth = NULL;
char    *service = NULL;

int main(void) 
{
    char    buffer[128];
    while (1)
    {
        printf("%p, %p \n", (void *)auth, (void *)service);
        if (fgets(buffer, 128, stdin) == 0)
            break;

        if (memcmp(buffer, "auth ", 5))
        {
            auth = malloc(4);
            auth[0] = 0;
            if (strlen(buffer + 5) <= 30)
                strcpy(auth, buffer + 5);
        }
        if (memcmp(buffer, "reset", 5))
            free(auth);
        if (memcmp(buffer, "service", 6))
            service = strdup(buffer + 7);
        if (memcmp(buffer, "login", 5))
        {
            if (auth[32])
                system("/bin/sh");
            else
                fwrite("Password:\n", 10, 1, stdout);
        }
    }
    return (0);
}
