#define _POSIX_C_SOURCE 200809L

#include "include/parser.h"

#include <ctype.h>
#include <string.h>

#include "include/append.h"
#include "include/helpers.h"

static bool character(struct parser *p, char c)
{
    if (c == '#')
    {
        return true;
    }

    if (c != ':' && c != '\n' && !isblank(c))
    {
        if (!p->is_double_dot && p->target_finished)
        {
            fprintf(stderr, "Invalid rule name\n");
            return false;
        }
        p->word[p->word_len++] = c;
        p->word[p->word_len] = '\0';
    }
    else if (c == ':' && !p->is_double_dot)
        return append_target(p);
    else if (c == ':' && p->is_double_dot)
    {
        fprintf(stderr, "Double ':' detected\n");
        return false;
    }
    else if ((isblank(c) || c == '\n') && p->word_len != 0)
        return blank(p);

    return true;
}

static bool parse_line(struct parser *p, const char *line, ssize_t len)
{
    if (line[0] == '#')
        return true;

    if (line[0] == '\t' && p->current_rule == NULL)
    {
        fprintf(stderr, "A tabulation was detected without rule before\n");
        return false;
    }
    else if (line[0] == '\t' && p->current_rule != NULL)
    {
        return append_command(p, line);
    }
    if (empty_line(line))
        return true;

    char *equal = strchr(line, '=');
    char *colon = strchr(line, ':');

    if (equal && (!colon || equal < colon))
    {
        p->current_rule = NULL;
        return append_variables(p, line);
    }

    p->word = malloc(len + 1);
    if (!p->word)
        return false;
    p->word[0] = '\0';
    p->word_len = 0;
    p->is_double_dot = false;
    p->is_dependency = false;
    p->target_finished = false;

    bool valid = true;
    for (ssize_t c = 0; c < len && valid; c++)
    {
        if (line[c] == '#')
        {
            break;
        }
        valid = character(p, line[c]);
    }
    if (valid)
        valid = finish_line(p);
    p->is_command = false;
    free(p->word);
    return valid;
}

int read_file(char *file, struct parser *p)
{
    FILE *f = fopen(file, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        return 2;
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t len;
    int status = 0;

    while ((len = getline(&line, &size, f)) != -1)
    {
        if (!parse_line(p, line, len))
        {
            status = 2;
            break;
        }
    }
    if (ferror(f))
    {
        fprintf(stderr, "Error reading file\n");
        status = 2;
    }
    free(line);
    fclose(f);

    return status;
}
