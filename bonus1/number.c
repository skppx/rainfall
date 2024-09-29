#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int nb = atoi(argv[1]);

    printf("int : %i\n", nb);
    printf("uint : %u\n", (unsigned int)nb);
    return 0;
}
