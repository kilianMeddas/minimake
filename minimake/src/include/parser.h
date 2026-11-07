#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    bool isCommand;
    bool target_finished;

    struct variable *variables;
    struct rule *rules;
    struct rule *current_rule;
};

struct list
{
    char *target;
    struct list *next;
};

struct rule
{
    char *target;
    struct list *dependencies;
    struct list *commands;
    struct rule *next;
};

int read_file(char *file, struct parser *p);

#endif // PARSER_H
