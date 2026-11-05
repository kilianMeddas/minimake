#include "include/helpers.h"

#include <ctype.h>
#include <string.h>

#include "include/append.h"
#include "include/resize.h"

bool empty_line(const char *line)
{
    for (size_t i = 0; line[i] != '\0'; i++)
    {
        if (!isblank(line[i]) && line[i] != '\n')
            return false;
    }
    return true;
}

bool blank(struct parser *p)
{
    if (p->isDoubleDot)
        return append_dependency(p);

    p->target_finished = true;
    return true;
}

bool finish_line(struct parser *p)
{
    if (!p->isDoubleDot)
    {
        fprintf(stderr, "Missing ':' in rule\n");
        return false;
    }
    if (p->word_len != 0 && p->isDoubleDot)
    {
        if (!append_dependency(p))
            return false;
    }
    if (!resize_buffer(p, p->buffer_len + 2))
        return false;

    p->buffer[p->buffer_len++] = '\n';
    p->buffer[p->buffer_len] = '\0';
    return true;
}

char *extract_file(char *arg)
{
    if (!arg)
        return NULL;

    while (*arg != '=')
        arg++;

    arg++;
    if (strcmp(arg, "") == 0)
    {
        fprintf(stderr, "Please provide a file\n");
        exit(2);
    }
    return arg;
}
