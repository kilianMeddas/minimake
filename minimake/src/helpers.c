#include "include/helpers.h"

#include <ctype.h>
#include <string.h>

#include "include/append.h"
#include "include/resize.h"

char *str_duplicate(const char *str)
{
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;

    strcpy(copy, str);
    return copy;
}

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

static bool append_text(char **result, size_t *len, size_t *capacity,
                        const char *text, size_t count)
{
    size_t n = *len + count + 1;

    if (n > *capacity)
    {
        size_t new_capacity = n * 2;
        char *new_result = realloc(*result, new_capacity);
        if (!new_result)
            return false;

        *result = new_result;
        *capacity = new_capacity;
    }

    memcpy(*result + *len, text, count);
    *len += count;
    (*result)[*len] = '\0';

    return true;
}

static char *extract_variable_name(const char **c)
{
    const char *start = *c + 1;
    const char *end = NULL;
    char closing = '\0';

    if (*start == '(' || *start == '{')
    {
        closing = *start == '(' ? ')' : '}';
        start++;
        end = strchr(start, closing);
        if (!end)
        {
            fprintf(stderr, "Missing '%c' in variable\n", closing);
            return NULL;
        }
    }
    else
        end = start + 1;

    size_t len = end - start;
    char *name = malloc(len + 1);
    if (!name)
        return NULL;

    memcpy(name, start, len);
    name[len] = '\0';

    *c = closing ? end + 1 : end;
    return name;
}

static bool expand_reference(char **result, size_t *len, size_t *capacity,
                             const char **c, const struct variable *variables)
{
    char *name = extract_variable_name(c);
    if (!name)
        return false;

    const char *value = variable_get(variables, name);
    free(name);

    if (!value)
        value = "";

    return append_text(result, len, capacity, value, strlen(value));
}

char *expand_variables(const char *input, const struct variable *variables)
{
    if (!input)
        return NULL;

    size_t len = 0;
    size_t capacity = 1;
    char *result = malloc(capacity);
    if (!result)
        return NULL;

    result[0] = '\0';
    const char *c = input;

    while (*c)
    {
        bool success = false;

        if (*c == '$' && c[1] == '$')
        {
            success = append_text(&result, &len, &capacity, "$", 1);
            c += 2;
        }
        else if (*c == '$' && c[1] != '\0')
        {
            success = expand_reference(&result, &len, &capacity, &c, variables);
        }
        else
        {
            success = append_text(&result, &len, &capacity, c, 1);
            c++;
        }

        if (!success)
        {
            free(result);
            return NULL;
        }
    }

    return result;
}
