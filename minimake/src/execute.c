#include "include/execute.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/append.h"
#include "include/list.h"
#include "include/parser.h"

int execute_rule(struct parser *p, struct rule *rule)
{
    if (!rule || !p)
        return 2;

    struct list *cmd = rule->commands;

    while (cmd)
    {
        const char *command = cmd->target;
        bool silent = command[0] == '@';

        if (silent)
            command++;

        char *expanded = expand_variables(command, p->variables);
        if (!expanded)
        {
            fprintf(stderr, "Error: variable expansion failed\n");
            return 2;
        }

        if (!silent)
        {
            printf("%s\n", expanded);
            fflush(stdout);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            fprintf(stderr, "Error: fork failed\n");
            free(expanded);
            return 2;
        }

        if (pid == 0)
        {
            execl("/bin/sh", "sh", "-c", expanded, NULL);
            fprintf(stderr, "Error: exec failed\n");
            _exit(2);
        }

        int wstatus;
        int result = waitpid(pid, &wstatus, 0);
        free(expanded);

        if (result == -1)
        {
            fprintf(stderr, "Error: waitpid failed\n");
            return 2;
        }

        if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0)
        {
            fprintf(stderr, "Error: command failed\n");
            return 2;
        }

        cmd = cmd->next;
    }

    return 0;
}

int build_rule(struct parser *p, struct rule *rule, bool *changed)
{
    if (!p || !rule)
        return 2;

    struct stat target_info;
    bool target_exists = stat(rule->target, &target_info) == 0;
    bool needs_build = !target_exists;

    for (struct list *dep = rule->dependencies; !rule->is_done && dep;
         dep = dep->next)
    {
        struct rule *dependency = rule_get(p->rules, dep->target);
        bool child_changed = false;

        if (dependency)
        {
            if (build_rule(p, dependency, &child_changed) != 0)
                return 2;
        }

        struct stat dependency_info;
        bool dependency_exists = stat(dep->target, &dependency_info) == 0;

        if (!dependency && !dependency_exists)
        {
            fprintf(stderr,
                    "minimake: *** No rule to make target '%s', "
                    "needed by '%s'. Stop.\n",
                    dep->target, rule->target);
            return 2;
        }

        if (child_changed)
        {
            rule->was_changed = true;
            needs_build = true;
        }

        if (target_exists && dependency_exists
            && dependency_info.st_mtime > target_info.st_mtime)
            needs_build = true;
    }

    bool rebuilt = !rule->is_done && needs_build && rule->commands;
    if (rebuilt && execute_rule(p, rule) != 0)
        return 2;

    rule->was_changed = rebuilt || rule->was_changed;

    if (changed)
        *changed = rule->was_changed;
    else if (rule->is_done || !rule->was_changed)
    {
        if (!rule->commands)
            printf("minimake: Nothing to be done for '%s'.\n", rule->target);
        else
            printf("minimake: '%s' is up to date.\n", rule->target);
    }

    rule->is_done = true;
    return 0;
}
