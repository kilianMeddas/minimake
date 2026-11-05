#include <string.h>

#include "include/helpers.h"
// #include "include/pretty_print.h"

struct options
{
    bool isPretty;
    bool isFilePresent;
    bool isHelp;
    char *file;
};

static void print_help()
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
        exit(2);
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
            exit(2);
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
        exit(2);
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
        exit(2);
    }
    else
    {
        fprintf(stderr, "Invalid argument: %s\n", argv[*i]);
        exit(2);
    }
    return 0;
}

static int parse_arguments(int argc, char *argv[], struct options *options)
{
    for (int i = 1; i < argc; i++)
    {
        int status = argument(argc, argv, &i, options);
        if (status != 0)
            exit(2);
    }
    return 0;
}

static int option_chose(struct options *options)
{
    if (options->isHelp)
    {
        print_help();
        return 0;
    }
    else if (options->isPretty && options->isFilePresent)
    {
        fprintf(stdout, "Not implemented yet but the file is : %s",
                options->file);
        // pretty_print(options->file);

        return 0;
    }
    else if (options->isFilePresent)
    {
        fprintf(stdout, "Not implemented yet but the file is : %s",
                options->file);

        // struct parser *p = {0};
        return 0;
        //        return read_file(options->file, p);
    }

    if (!options->isFilePresent)
    {
        fprintf(stderr, "Provide a file with the -f flag\n");
        exit(2);
    }

    if (!options->isHelp && options->isFilePresent && !options->isPretty)
    {
        fprintf(stderr, "Please check the -h\n");
        exit(2);
    }
    fprintf(stderr, "No correct flag(s) provided");
    exit(2);
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

    int status = parse_arguments(argc, argv, &options);
    if (status != 0)
        return status;

    return option_chose(&options);
}
