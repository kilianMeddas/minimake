#include "include/append.h"

#include <string.h>

#include "include/resize.h"

bool append_target(struct parser *p)
{
    size_t n = p->buffer_len + p->word_len + 4;
    if (!resize_buffer(p, n))
        return false;

    p->buffer[p->buffer_len++] = '(';
    memcpy(p->buffer + p->buffer_len, p->word, p->word_len);
    p->buffer_len += p->word_len;
    p->buffer[p->buffer_len++] = ')';
    p->buffer[p->buffer_len++] = ':';
    p->buffer[p->buffer_len] = '\0';

    p->isDoubleDot = true;
    p->word_len = 0;
    p->word[0] = '\0';
    return true;
}

bool append_dependency(struct parser *p)
{
    size_t n = p->buffer_len + p->word_len + 4;
    if (!resize_buffer(p, n))
        return false;

    p->buffer[p->buffer_len++] = ' ';
    p->buffer[p->buffer_len++] = '[';
    memcpy(p->buffer + p->buffer_len, p->word, p->word_len);
    p->buffer_len += p->word_len;
    p->buffer[p->buffer_len++] = ']';
    p->buffer[p->buffer_len] = '\0';

    p->isDependency = true;
    p->word_len = 0;
    p->word[0] = '\0';
    return true;
}
