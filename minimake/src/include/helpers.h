#ifndef HELPERS_H
#define HELPERS_H

#include "parser.h"

bool empty_line(const char *line);
bool blank(struct parser *p);
bool finish_line(struct parser *p);
char *extract_file(char *arg);
#endif // HELPERS_H
