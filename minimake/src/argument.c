#include "include/argument.h"

#include <string.h>

#include "include/helpers.h"
#include "include/list.h"
#include "include/options.h"

int file_argument(int argc, char *argv[], int *i, struct options *options)
{
    if (strcmp("-f", argv[*i]) == 0 && (*i + 1 < argc))
    {
        options->is_file_present = true;
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
        options->is_file_present = true;
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

int argument(int argc, char *argv[], int *i, struct options *options)
{
    if (strcmp("-h", argv[*i]) == 0 || strcmp("--help", argv[*i]) == 0)
    {
        options->is_help = true;
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
        options->is_pretty = true;
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
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
            i++;
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            options->is_help = true;
            return 0;
        }
    }

    for (int i = 1; i < argc; i++)
    {
        int status = argument(argc, argv, &i, options);
        if (status != 0)
            return 2;
    }
    return 0;
}
