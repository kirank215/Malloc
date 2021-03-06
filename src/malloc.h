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
void *addpointer(void *b , int a);
int allign(size_t s);
void set_block(struct metadata *b , size_t s , int free , \
                struct metadata *n ,struct metadata *p ,struct metadata *nf);
void remove_from_free(struct metadata *fl , struct metadata *block);
void add_to_free(struct metadata *fl , struct metadata *block);
void print_fl(struct metadata *fl);
void *newpage(int size);
struct metadata *find_block(size_t size);
void add_block(struct metadata *b1 , size_t s);
#endif
