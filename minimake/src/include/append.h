#ifndef APPEND_H
#define APPEND_H

#include <stdbool.h>

#include "parser.h"

bool append_target(struct parser *p);
bool append_dependency(struct parser *p);
bool append_variables(struct parser *p, const char *line);
bool append_command(struct parser *p, const char *line);
char *expand_variables(const char *input, const struct variable *variables);
#endif // APPEND_H
