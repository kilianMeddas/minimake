
#include "include/append.h"

#include <ctype.h>
#include <string.h>

#include "include/chained_list.h"
#include "include/helpers.h"
#include "include/list.h"
#include "include/resize.h"

void remove_spaces(char *str)
{
    if (str == NULL)
        return;

    char *equal_ptr = strchr(str, '=');
    if (equal_ptr == NULL)
        return;

    char *start = str;
    while (isblank(*start))
        start++;

    char *left_spaces = equal_ptr;
    while (left_spaces > start && isblank(*(left_spaces - 1)))
        left_spaces--;

    char *right_content = equal_ptr + 1;
    while (isblank(*right_content))
        right_content++;

    size_t name_len = left_spaces - start;

    memmove(str, start, name_len);
    str[name_len] = '=';
    memmove(str + name_len + 1, right_content, strlen(right_content) + 1);
}

static bool store_target(struct parser *p, const char *target, size_t len)
{
    size_t n = p->buffer_len + len + 4;
    if (!resize_buffer(p, n))
        return false;

    if (!rule_insert(p, target))
        return false;

    p->buffer[p->buffer_len++] = '(';
    memcpy(p->buffer + p->buffer_len, target, len);
    p->buffer_len += len;
    p->buffer[p->buffer_len++] = ')';
    p->buffer[p->buffer_len++] = ':';
    p->buffer[p->buffer_len] = '\0';

    return true;
}

bool append_target(struct parser *p)
{
    char *expanded = expand_variables(p->word, p->variables);
    if (!expanded)
        return false;

    size_t len = strlen(expanded);

    if (len == 0)
    {
        p->current_rule = NULL;
        p->isDoubleDot = true;
        p->word_len = 0;
        p->word[0] = '\0';
        free(expanded);
        return true;
    }

    bool result = store_target(p, expanded, len);
    free(expanded);
    if (!result)
        return false;

    p->isDoubleDot = true;
    p->word_len = 0;
    p->word[0] = '\0';

    return true;
}

static bool store_dependency(struct parser *p, const char *dependency)
{
    size_t len = strlen(dependency);
    size_t n = p->buffer_len + len + 4;

    if (!resize_buffer(p, n))
        return false;

    if (!dependency_insert(p->current_rule, dependency))
        return false;

    p->buffer[p->buffer_len++] = ' ';
    p->buffer[p->buffer_len++] = '[';
    memcpy(p->buffer + p->buffer_len, dependency, len);
    p->buffer_len += len;
    p->buffer[p->buffer_len++] = ']';
    p->buffer[p->buffer_len] = '\0';

    return true;
}

bool append_dependency(struct parser *p)
{
    if (p->word_len == 0)
        return true;

    if (p->current_rule == NULL)
    {
        p->word_len = 0;
        p->word[0] = '\0';
        return true;
    }

    char *expanded = expand_variables(p->word, p->variables);
    if (!expanded)
        return false;

    char *dependency = strtok(expanded, " \t\n");
    while (dependency)
    {
        if (!store_dependency(p, dependency))
        {
            free(expanded);
            return false;
        }

        dependency = strtok(NULL, " \t\n");
    }

    free(expanded);
    p->isDependency = true;
    p->word_len = 0;
    p->word[0] = '\0';

    return true;
}

bool append_command(struct parser *p, const char *line)
{
    if (p->current_rule == NULL)
        return true;

    char *command = str_duplicate(line);
    if (!command)
        return false;

    char *content = command + 1;

    while (isblank(*content))
        content++;

    size_t len = strlen(content);

    if (len != 0 && content[len - 1] == '\n')
        content[len - 1] = '\0';

    bool result = command_insert(p->current_rule, content);

    free(command);
    return result;
}

bool append_variables(struct parser *p, const char *line)
{
    char *line_parsable = str_duplicate(line);
    if (!line_parsable)
        return false;

    remove_spaces(line_parsable);

    char *separator = strchr(line_parsable, '=');
    if (!separator)
    {
        free(line_parsable);
        return false;
    }

    *separator = '\0';

    char *name = line_parsable;
    char *value = separator + 1;

    char *comment = strchr(value, '#');
    if (comment)
        *comment = '\0';

    size_t len = strlen(value);
    if (len != 0 && value[len - 1] == '\n')
        value[len - 1] = '\0';

    char *expanded = expand_variables(name, p->variables);
    if (!expanded)
    {
        free(line_parsable);
        return false;
    }

    bool result = false;
    if (expanded[0] != '\0')
        result = variable_set(&p->variables, expanded, value);

    free(expanded);
    free(line_parsable);
    return result;
}
