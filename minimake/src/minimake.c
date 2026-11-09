#include "include/argument.h"
#include "include/list.h"
#include "include/options.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        print_help();
        return 0;
    }

    struct options options;
    options.is_pretty = false;
    options.is_file_present = false;
    options.is_help = false;
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
