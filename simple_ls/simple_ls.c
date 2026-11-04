#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *path = ".";

    if (argc == 2)
    {
        path = argv[1];
    }
    else if (argc > 2)
    {
        fprintf(stderr, "Please provide at most one path");
        return 1;
    }

    DIR *directory = opendir(path);
    if (!directory)
    {
        fprintf(stderr, "simple_ls: %s: No such file or directory\n", path);
        return 1;
    }
    struct dirent *entry;

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        printf("%s\n", entry->d_name);
    }

    if (closedir(directory) == -1)
    {
        fprintf(stderr, "simple_ls: %s: error %d\n", path, errno);
        return 1;
    }

    return 0;
}
