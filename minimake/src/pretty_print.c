#include "include/pretty_print.h"

#include <stdio.h>

void pretty_print(struct parser *p)
{
    struct variable *current_variable = p->variables;
    fprintf(stdout, "# variables\n");
    while (current_variable != NULL)
    {
        fprintf(stdout, "'%s' = '%s'\n", current_variable->name,
                current_variable->value);
        current_variable = current_variable->next;
    }

    struct rule *current_rule = p->rules;
    fprintf(stdout, "# rules\n");

    while (current_rule != NULL)
    {
        fprintf(stdout, "(%s):", current_rule->target);
        struct list *dependencies = current_rule->dependencies;
        while (dependencies != NULL)
        {
            fprintf(stdout, " [%s]", dependencies->target);
            dependencies = dependencies->next;
        }

        fprintf(stdout, "\n");
        struct list *cmds = current_rule->commands;
        while (cmds != NULL)
        {
            fprintf(stdout, "\t'%s'\n", cmds->target);
            cmds = cmds->next;
        }
        current_rule = current_rule->next;
    }
}
