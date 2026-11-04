#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_empty_line(const char *line)
{
    for (size_t i = 0; line[i] != '\0'; i++)
    {
        if (!isblank(line[i]) && line[i] != '\n')
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "The syntaxe is: ./micromake Makefile\n");
        return 2;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        return 2;
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t len;

    char buffer[256] = "";

    while ((len = getline(&line, &size, file)) != -1)
    {
        if (line[0] == '\t')
        {
            fprintf(stderr, "A tabulation was detected\n");
            free(line);
            fclose(file);
            return 2;
        }
        if (is_empty_line(line))
            continue;

        char *word = malloc(sizeof(char) * size);
        if (!word)
        {
            fprintf(stderr,
                    "An error occured during the allocation of the memory for "
                    "a word");
            free(line);
            fclose(file);
            return 2;
        }
        word[0] = '\0';

        bool isDoubleDot = false;
        bool isDependency = false;

        for (ssize_t c = 0; c < len; c++)
        {
            if (line[c] == '=')
            {
                fprintf(stderr,
                        "You cannot defined variables for this exercice\n");
                free(word);
                free(line);
                fclose(file);
                return 2;
            }
            else if (line[c] == '#')
            {
                fprintf(stderr, "Character unknown for a Makefile");
                free(word);
                free(line);
                fclose(file);
                return 2;
            }

            if (line[c] != ':' && line[c] != '\n' && !isblank(line[c]))
            {
                size_t len_word = strlen(word);
                snprintf(word + len_word, size - len_word, "%c", line[c]);
            }
            else if (line[c] == ':' && !isDoubleDot)
            {
                if (strlen(word) == 0)
                {
                    fprintf(stderr, "Missing rule name\n");
                    free(word);
                    free(line);
                    fclose(file);
                    return 2;
                }
                snprintf(buffer + strlen(buffer),
                         sizeof(buffer) - strlen(buffer), "(%s):", word);
                isDoubleDot = true;
                word[0] = '\0';
            }
            else if (line[c] == ':' && isDoubleDot)
            {
                fprintf(stderr, "Double ':' detected)");
                free(line);
                fclose(file);
                free(word);
                return 2;
            }
            else if ((isblank(line[c]) || line[c] == '\n') && strlen(word) != 0)
            {
                if (isDoubleDot)
                {
                    snprintf(buffer + strlen(buffer),
                             sizeof(buffer) - strlen(buffer), " [%s]", word);

                    isDependency = true;
                    word[0] = '\0';
                }
            }
        }
        if (!isDoubleDot)
        {
            fprintf(stderr, "Missing ':' in rule\n");
            free(word);
            free(line);
            fclose(file);
            return 2;
        }
        if (strlen(word) != 0 && isDoubleDot)
        {
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
                     " [%s]", word);
        }

        (void)isDependency;

        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
                 "\n");

        free(word);
    }

    printf("%s", buffer);

    free(line);
    fclose(file);
    return 0;
}
