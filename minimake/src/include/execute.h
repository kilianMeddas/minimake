#ifndef EXECUTE_H
#define EXECUTE_H

#include "parser.h"

int execute_rule(struct parser *p, struct rule *rules);
int build_rule(struct parser *p, struct rule *rule, bool *changed);

#endif // EXECUTE_H
