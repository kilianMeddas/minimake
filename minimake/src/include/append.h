#ifndef APPEND_H
#define APPEND_H

#include "parser.h"
#include <stdbool.h>

bool append_target(struct parser *p);
bool append_dependency(struct parser *p);
bool append_variables(struct parser *p);

#endif // APPEND_H

