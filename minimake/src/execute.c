#include "include/execute.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/append.h"

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
