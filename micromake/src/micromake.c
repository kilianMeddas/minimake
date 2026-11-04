#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
};

bool is_empty_line(const char *line)
{
    for (size_t i = 0; line[i] != '\0'; i++)
    {
        if (!isblank(line[i]) && line[i] != '\n')
            return false;
    }
    return true;
}

bool resize_buffer(struct parser *p, size_t needed)
{
    if (needed <= p->buffer_size)
        return true;

    char *tmp = realloc(p->buffer, needed);
    if (!tmp)
        return false;

    p->buffer = tmp;
    p->buffer_size = needed;
    return true;
}

bool append_target(struct parser *p)
{
    size_t needed = p->buffer_len + p->word_len + 4;
    if (!resize_buffer(p, needed))
        return false;

    p->buffer[p->buffer_len++] = '(';
    memcpy(p->buffer + p->buffer_len, p->word, p->word_len);
    p->buffer_len += p->word_len;
    p->buffer[p->buffer_len++] = ')';
    p->buffer[p->buffer_len++] = ':';
    p->buffer[p->buffer_len] = '\0';

    p->isDoubleDot = true;
    p->word_len = 0;
    p->word[0] = '\0';
    return true;
}

bool append_dependency(struct parser *p)
{
    size_t needed = p->buffer_len + p->word_len + 4;
    if (!resize_buffer(p, needed))
        return false;

    p->buffer[p->buffer_len++] = ' ';
    p->buffer[p->buffer_len++] = '[';
    memcpy(p->buffer + p->buffer_len, p->word, p->word_len);
    p->buffer_len += p->word_len;
    p->buffer[p->buffer_len++] = ']';
    p->buffer[p->buffer_len] = '\0';

    p->isDependency = true;
    p->word_len = 0;
    p->word[0] = '\0';
    return true;
}

bool handle_blank(struct parser *p)
{
    if (p->isDoubleDot)
        return append_dependency(p);

    p->target_finished = true;
    return true;
}
bool handle_character(struct parser *p, char c)
{
    if (c == '=' || c == '#')
    {
        fprintf(stderr, "Invalid character\n");
        return false;
    }

    if (c != ':' && c != '\n' && !isblank(c))
    {
        if (!p->isDoubleDot && p->target_finished)
        {
            fprintf(stderr, "Invalid rule name\n");
            return false;
        }
        p->word[p->word_len++] = c;
        p->word[p->word_len] = '\0';
    }
    else if (c == ':' && !p->isDoubleDot)
        return append_target(p);
    else if (c == ':' && p->isDoubleDot)
    {
        fprintf(stderr, "Double ':' detected\n");
        return false;
    }
    else if ((isblank(c) || c == '\n') && p->word_len != 0)
        return handle_blank(p);
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

bool parse_line(struct parser *p, const char *line, ssize_t len)
{
    if (line[0] == '\t')
    {
        fprintf(stderr, "A tabulation was detected\n");
        return false;
    }
    if (is_empty_line(line))
        return true;

    p->word = malloc(len + 1);
    if (!p->word)
        return false;
    p->word[0] = '\0';
    p->word_len = 0;
    p->isDoubleDot = false;
    p->isDependency = false;
    p->target_finished = false;

    bool valid = true;
    for (ssize_t c = 0; c < len && valid; c++)
        valid = handle_character(p, line[c]);
    if (valid)
        valid = finish_line(p);
    free(p->word);
    return valid;
}

int read_file(FILE *file, struct parser *p)
{
    char *line = NULL;
    size_t size = 0;
    ssize_t len;
    int status = 0;

    while ((len = getline(&line, &size, file)) != -1)
    {
        if (!parse_line(p, line, len))
        {
            status = 2;
            break;
        }
    }
    if (ferror(file))
    {
        fprintf(stderr, "Error reading file\n");
        status = 2;
    }
    free(line);
    return status;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "The syntax is: ./micromake Makefile\n");
        return 2;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        return 2;
    }
    struct parser p = { 0 };
    int status = read_file(file, &p);
    if (status == 0 && p.buffer != NULL)
        printf("%s", p.buffer);
    free(p.buffer);
    fclose(file);
    return status;
}
