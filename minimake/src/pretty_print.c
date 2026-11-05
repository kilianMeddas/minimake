#include "include/pretty_print.h"

#include <stdio.h>
#include <stdlib.h>

void pretty_print(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        exit(2);
    }
}
