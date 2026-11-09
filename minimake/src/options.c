#include "include/options.h"

#include "include/execute.h"
#include "include/list.h"
#include "include/parser.h"
#include "include/pretty_print.h"

static void parser_free(struct parser *p)
{
    variables_free(p->variables);
    rules_free(p->rules);
    free(p->buffer);
}

static int select_targets(struct parser *p, struct options *options)
{
    if (!p || !options)
        return 2;

    struct list *current = options->targets;

    if (options->targets == NULL)
    {
        struct rule *rule = first_standard_rule(p->rules);

        if (!rule)
        {
            fprintf(stderr, "minimake: *** No targets. Stop.\n");
            return 2;
        }

        return build_rule(p, rule, NULL);
    }

    while (current != NULL)
    {
        struct rule *rule = rule_get(p->rules, current->target);
        if (!rule)
        {
            fprintf(stderr,
                    "minimake: *** No rule to make target '%s'. Stop.\n",
                    current->target);
            return 2;
        }

        int result = build_rule(p, rule, NULL);
        if (result != 0)
            return 2;

        current = current->next;
    }
    return 0;
}

void print_help(void)
{
    fprintf(stdout, "Usage: ./minimake [target] [options] ...\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout,
            "-f file, --file=file, --makefile=file      Name of the Makefile "
            "to use (Mandatory)\n");
    fprintf(stdout,
            "-h, --help                                 Print this message and "
            "exit.\n");
    fprintf(stdout,
            "-p, --print-data-base --pretty-print       Print make's internal "
            "database.\n");
}

int option_chose(struct options *options)
{
    int status = 0;
    struct parser p = { 0 };

    if (options->is_help)
    {
        print_help();
        return 0;
    }
    if (!options->file)
    {
        fprintf(stderr, "No file found");
        parser_free(&p);
        return 2;
    }
    int result = read_file(options->file, &p);
    if (result != 0)
    {
        fprintf(stderr,
                "An error occured while collecting the data from the file");
        parser_free(&p);
        return 2;
    }
    if (options->is_pretty && options->is_file_present)
    {
        pretty_print(&p);
        status = 0;
    }
    else if (options->is_file_present)
    {
        status = select_targets(&p, options);
    }

    parser_free(&p);
    return status;
}
