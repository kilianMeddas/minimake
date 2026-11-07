#ifndef CHAINED_LIST_H
#define CHAINED_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct variable
{
    char *name;
    char *value;
    struct variable *next;
};

bool variable_insert(struct variable **variables, const char *name,
                     const char *value);

const char *variable_get(const struct variable *variables, const char *name);

bool variable_set(struct variable **variables, const char *name,
                  const char *value);

void variables_free(struct variable *variables);

#endif // CHAINED_LIST_H
