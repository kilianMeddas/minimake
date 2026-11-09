#include "include/list.h"

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static char *str_duplicate(const char *str)
{
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;

    strcpy(copy, str);
    return copy;
}

bool rule_insert(struct parser *p, const char *target)
{
    if (!p || !target)
        return false;

    struct rule *new = malloc(sizeof(struct rule));
    if (!new)
        return false;

    new->target = str_duplicate(target);
    if (!new->target)
    {
        free(new);
        return false;
    }

    new->dependencies = NULL;
    new->commands = NULL;
    new->next = NULL;
    new->is_done = false;
    new->was_changed = false;
    if (!p->rules)
    {
        p->rules = new;
    }
    else
    {
        struct rule *current = p->rules;

        while (current->next)
            current = current->next;

        current->next = new;
    }

    p->current_rule = new;
    return true;
}

bool list_insert(struct list **target, const char *value)
{
    if (!target || !value)
        return false;

    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return false;

    new->target = str_duplicate(value);
    if (!new->target)
    {
        free(new);
        return false;
    }

    new->next = NULL;

    if (!*target)
    {
        *target = new;
        return true;
    }

    struct list *current = *target;

    while (current->next)
        current = current->next;

    current->next = new;

    return true;
}

bool dependency_insert(struct rule *rule, const char *dependency)
{
    if (!rule || !dependency)
        return false;

    return list_insert(&rule->dependencies, dependency);
}

bool command_insert(struct rule *rule, const char *command)
{
    if (!rule || !command)
        return false;

    return list_insert(&rule->commands, command);
}

void list_free(struct list *list)
{
    struct list *current = list;

    while (current)
    {
        struct list *next = current->next;

        free(current->target);
        free(current);

        current = next;
    }
}

void rules_free(struct rule *rules)
{
    struct rule *current = rules;

    while (current)
    {
        struct rule *next = current->next;

        list_free(current->dependencies);
        list_free(current->commands);
        free(current->target);
        free(current);

        current = next;
    }
}

struct rule *rule_get(struct rule *rules, const char *target)
{
    if (!rules || !target)
        return NULL;

    struct rule *current = rules;

    while (current)
    {
        if (strcmp(current->target, target) == 0)
            return current;

        current = current->next;
    }
    return NULL;
}

struct rule *first_standard_rule(struct rule *rules)
{
    if (!rules)
        return NULL;

    struct rule *current = rules;
    while (current)
    {
        if (strchr(current->target, '%') == NULL)
            return current;

        current = current->next;
    }

    return NULL;
}
