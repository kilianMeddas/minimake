#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <string.h>

struct options
{
    bool is_pretty;
    bool is_file_present;
    bool is_help;
    char *file;
    struct list *targets;
};

void print_help(void);
int option_chose(struct options *options);

#endif // OPTIONS_H
