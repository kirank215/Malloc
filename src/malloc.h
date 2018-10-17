#ifndef MALLOC_H
#define MALLOC_H

struct metadata
{
    size_t size;
    int is_free;
    struct metadata *next;
    struct metadata *prev;
    struct metadata *nxt_free;
};
void *find_block(size_t size);
