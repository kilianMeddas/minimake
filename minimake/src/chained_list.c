#include "include/chained_list.h"

#include <stdlib.h>
#include <string.h>

static char *str_duplicate(const char *str)
{
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;

    strcpy(copy, str);
    return copy;
}

bool variable_insert(struct variable **variables, const char *name,
                     const char *value)
{
    if (!variables || !name || !value)
        return false;

    struct variable *new = malloc(sizeof(struct variable));
    if (!new)
        return false;

    new->name = str_duplicate(name);
    new->value = str_duplicate(value);

    if (!new->name || !new->value)
    {
        free(new->name);
        free(new->value);
        free(new);
        return false;
    }

    new->next = *variables;
    *variables = new;

    return true;
}

const char *variable_get(const struct variable *variables, const char *name)
{
    if (!name)
        return NULL;

    const struct variable *current = variables;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
            return current->value;

        current = current->next;
    }
    return NULL;
}

bool variable_set(struct variable **variables, const char *name,
                  const char *value)
{
    if (!variables || !name || !value)
        return false;

    struct variable *current = *variables;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            char *new_value = str_duplicate(value);
            if (!new_value)
                return false;

            free(current->value);
            current->value = new_value;
            return true;
        }

        current = current->next;
    }

    return variable_insert(variables, name, value);
}

void variables_free(struct variable *variables)
{
    struct variable *current = variables;

    while (current)
    {
        struct variable *actual = current;
        current = current->next;

        free(actual->name);
        free(actual->value);
        free(actual);
    }
}
