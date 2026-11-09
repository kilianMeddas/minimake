#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "options.h"

int file_argument(int argc, char *argv[], int *i, struct options *options);

int argument(int argc, char *argv[], int *i, struct options *options);

int parse_arguments(int argc, char *argv[], struct options *options);

#endif // ARGUMENT_H
