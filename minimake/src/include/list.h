#ifndef LIST_H
#define LIST_H

#include "parser.h"

bool rule_insert(struct parser *p, const char *target);

bool dependency_insert(struct rule *rule, const char *dependency);

bool command_insert(struct rule *rule, const char *command);

void list_free(struct list *list);

void rules_free(struct rule *rules);

struct rule *rule_get(struct rule *rules, const char *target);

struct rule *first_standard_rule(struct rule *rules);

bool list_insert(struct list **target, const char *value);

#endif // LIST_H
