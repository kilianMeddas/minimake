#include "include/resize.h"

bool resize_buffer(struct parser *p, size_t n)
{
    if (n <= p->buffer_size)
        return true;

    char *tmp = realloc(p->buffer, n);
    if (!tmp)
        return false;

    p->buffer = tmp;
    p->buffer_size = n;
    return true;
}
