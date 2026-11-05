#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "chained_list.h"

struct parser
{
    char *word;
    size_t word_len;
    char *buffer;
    size_t buffer_size;
    size_t buffer_len;
    bool isDoubleDot;
    bool isDependency;
    bool target_finished;
    struct variable *variables;
};

int read_file(char *file, struct parser *p);

#endif // PARSER_H
