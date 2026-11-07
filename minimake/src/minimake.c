#include <string.h>

#include "include/execute.h"
#include "include/helpers.h"
#include "include/list.h"
#include "include/parser.h"
#include "include/pretty_print.h"

struct options
{
    bool isPretty;
    bool isFilePresent;
    bool isHelp;
    char *file;
    struct list *targets;
};

static int file_argument(int argc, char *argv[], int *i,
                         struct options *options)
{
    if (strcmp("-f", argv[*i]) == 0 && (*i + 1 < argc))
    {
        options->isFilePresent = true;
        options->file = argv[*i + 1];
        (*i)++;
    }

    else if (strcmp("-f", argv[*i]) == 0 && (*i + 1 >= argc))
    {
        fprintf(stderr,
                "Please provide a file (./Minimakefile -h for more "
                "informations)\n");
        return 2;
    }
    else if (strstr(argv[*i], "--file=") != NULL
             || strstr(argv[*i], "--makefile=") != NULL)
    {
        options->isFilePresent = true;
        options->file = extract_file(argv[*i]);
        if (!options->file)
        {
            fprintf(stderr,
                    "An error occured while collection the name of the file in "
                    ": %s\n",
                    argv[*i]);
            return 2;
        }
    }
    return 0;
}

static int argument(int argc, char *argv[], int *i, struct options *options)
{
    if (strcmp("-h", argv[*i]) == 0 || strcmp("--help", argv[*i]) == 0)
    {
        options->isHelp = true;
    }
    else if ((strcmp("-f", argv[*i]) == 0 && (*i + 1 < argc))
             || strstr(argv[*i], "--file=") != NULL
             || strstr(argv[*i], "--makefile=") != NULL)
    {
        return file_argument(argc, argv, i, options);
    }
    else if (strcmp("-f", argv[*i]) == 0 && (*i + 1 >= argc))
    {
        fprintf(stderr,
                "Please provide a file (./Minimakefile -h for more "
                "informations)\n");
        return 2;
    }
    else if (strcmp("-p", argv[*i]) == 0
             || strcmp("--print-data-base", argv[*i]) == 0
             || strcmp("--pretty-print", argv[*i]) == 0)
    {
        options->isPretty = true;
    }
    else if (strcmp("", argv[*i]) == 0)
    {
        fprintf(stderr,
                "minimake: *** empty string invalid as argument. Stop.\n");
        return 2;
    }
    else if (argv[*i][0] == '-')
    {
        fprintf(stderr, "Available option: %s\n", argv[*i]);
        return 2;
    }

    else
    {
        if (list_insert(&options->targets, argv[*i]) == false)
            return 2;
    }
    return 0;
}

int parse_arguments(int argc, char *argv[], struct options *options)
{
    for (int i = 1; i < argc; i++)
    {
        int status = argument(argc, argv, &i, options);
        if (status != 0)
            return 2;
    }
    return 0;
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

        return execute_rule(p, rule);
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

        int result = execute_rule(p, rule);
        if (result != 0)
            return 2;

        current = current->next;
    }
    return 0;
}

static void parser_free(struct parser *p)
{
    variables_free(p->variables);
    rules_free(p->rules);
    free(p->buffer);
}

static void print_help(void)
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

static int option_chose(struct options *options)
{
    int status = 0;
    struct parser p = { 0 };

    if (options->isHelp)
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
    if (options->isPretty && options->isFilePresent)
    {
        pretty_print(&p);
        status = 0;
    }

    else if (options->isFilePresent)
    {
        status = select_targets(&p, options);
    }

    parser_free(&p);
    return status;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        print_help();
        return 0;
    }

    struct options options;
    options.isPretty = false;
    options.isFilePresent = false;
    options.isHelp = false;
    options.file = NULL;
    options.targets = NULL;

    int status = parse_arguments(argc, argv, &options);
    if (status != 0)
    {
        list_free(options.targets);
        return status;
    }
    int result = option_chose(&options);
    list_free(options.targets);

    return result;
}
