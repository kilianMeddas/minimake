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
        fprintf(stderr, "The syntax is: ./micromake Makefile\n");
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

    char *buffer = NULL;
    size_t buffer_size = 0;
    size_t buffer_len = 0;

    while ((len = getline(&line, &size, file)) != -1)
    {
        if (line[0] == '\t')
        {
            fprintf(stderr, "A tabulation was detected\n");
            free(buffer);
            free(line);
            fclose(file);
            return 2;
        }

        if (is_empty_line(line))
            continue;

        char *word = malloc(size);
        if (!word)
        {
            fprintf(stderr, "Memory allocation failed\n");
            free(buffer);
            free(line);
            fclose(file);
            return 2;
        }

        word[0] = '\0';

        bool isDoubleDot = false;
        bool isDependency = false;
        bool target_finished = false;
        size_t word_len = 0;

        for (ssize_t c = 0; c < len; c++)
        {
            if (line[c] == '=' || line[c] == '#')
            {
                fprintf(stderr, "Invalid character\n");
                free(word);
                free(buffer);
                free(line);
                fclose(file);
                return 2;
            }

            if (line[c] != ':' && line[c] != '\n' && !isblank(line[c]))
            {
                if (!isDoubleDot && target_finished)
                {
                    fprintf(stderr, "Invalid rule name\n");
                    free(word);
                    free(buffer);
                    free(line);
                    fclose(file);
                    return 2;
                }

                word[word_len] = line[c];
                word_len++;
                word[word_len] = '\0';
            }
            else if (line[c] == ':' && !isDoubleDot)
            {
                size_t needed = buffer_len + word_len + 4;

                if (needed > buffer_size)
                {
                    char *tmp = realloc(buffer, needed);
                    if (!tmp)
                    {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(word);
                        free(buffer);
                        free(line);
                        fclose(file);
                        return 2;
                    }
                    buffer = tmp;
                    buffer_size = needed;
                }

                buffer[buffer_len++] = '(';
                memcpy(buffer + buffer_len, word, word_len);
                buffer_len += word_len;
                buffer[buffer_len++] = ')';
                buffer[buffer_len++] = ':';
                buffer[buffer_len] = '\0';

                isDoubleDot = true;
                word_len = 0;
                word[0] = '\0';
            }
            else if (line[c] == ':' && isDoubleDot)
            {
                fprintf(stderr, "Double ':' detected\n");
                free(word);
                free(buffer);
                free(line);
                fclose(file);
                return 2;
            }
            else if ((isblank(line[c]) || line[c] == '\n') && word_len != 0)
            {
                if (isDoubleDot)
                {
                    size_t needed = buffer_len + word_len + 4;

                    if (needed > buffer_size)
                    {
                        char *tmp = realloc(buffer, needed);
                        if (!tmp)
                        {
                            fprintf(stderr, "Memory allocation failed\n");
                            free(word);
                            free(buffer);
                            free(line);
                            fclose(file);
                            return 2;
                        }
                        buffer = tmp;
                        buffer_size = needed;
                    }

                    buffer[buffer_len++] = ' ';
                    buffer[buffer_len++] = '[';
                    memcpy(buffer + buffer_len, word, word_len);
                    buffer_len += word_len;
                    buffer[buffer_len++] = ']';
                    buffer[buffer_len] = '\0';

                    isDependency = true;
                    word_len = 0;
                    word[0] = '\0';
                }
                else
                {
                    target_finished = true;
                }
            }
        }

        if (!isDoubleDot)
        {
            fprintf(stderr, "Missing ':' in rule\n");
            free(word);
            free(buffer);
            free(line);
            fclose(file);
            return 2;
        }

        if (word_len != 0 && isDoubleDot)
        {
            size_t needed = buffer_len + word_len + 4;

            if (needed > buffer_size)
            {
                char *tmp = realloc(buffer, needed);
                if (!tmp)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(word);
                    free(buffer);
                    free(line);
                    fclose(file);
                    return 2;
                }
                buffer = tmp;
                buffer_size = needed;
            }

            buffer[buffer_len++] = ' ';
            buffer[buffer_len++] = '[';
            memcpy(buffer + buffer_len, word, word_len);
            buffer_len += word_len;
            buffer[buffer_len++] = ']';
            buffer[buffer_len] = '\0';

            isDependency = true;
        }

        (void)isDependency;

        if (buffer_len + 2 > buffer_size)
        {
            char *tmp = realloc(buffer, buffer_len + 2);
            if (!tmp)
            {
                fprintf(stderr, "Memory allocation failed\n");
                free(word);
                free(buffer);
                free(line);
                fclose(file);
                return 2;
            }
            buffer = tmp;
            buffer_size = buffer_len + 2;
        }

        buffer[buffer_len++] = '\n';
        buffer[buffer_len] = '\0';

        free(word);
    }

    if (ferror(file))
    {
        fprintf(stderr, "Error reading file\n");
        free(buffer);
        free(line);
        fclose(file);
        return 2;
    }

    if (buffer != NULL)
        printf("%s", buffer);

    free(buffer);
    free(line);
    fclose(file);
    return 0;
}
