#ifndef HELPERS_H
#define HELPERS_H

#include "parser.h"

bool empty_line(const char *line);
bool blank(struct parser *p);
bool finish_line(struct parser *p);
char *extract_file(char *arg);
char *str_duplicate(const char *str);
char *extract_variable(const char *line, char *start);
#endif // HELPERS_H
