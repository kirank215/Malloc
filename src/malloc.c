#define _GNU_SOURCE
#include <stddef.h>
#include<err.h>
#include<sys/mman.h>
#include "malloc.h"
#define METASIZE allign(sizeof(struct metadata))
void *freelist = NULL;

void *addpointer(void *b , int a)
{
    char *s = b;
    s += a;
    return s;
}

int allign( size_t s)
{
    int size = sizeof(size_t);
    s = (s + (size - 1)) & ~(size - 1);
    return s;
}
void set_block(struct metadata *b ,size_t s , int free ,\
                struct metadata *n ,struct metadata *p , struct metadata *nf )
{
    b->size = s;
    b->is_free = free;
    b->next = n;
    b->prev = p;
    b->nxt_free= nf;
}

void remove_from_free(struct metadata *fl , struct metadata *block)
{
    for(; fl->nxt_free != block; fl = fl->nxt_free);
    fl->nxt_free = fl->nxt_free->nxt_free; 
}
void add_to_free(struct metadata *fl , struct metadata *block)
{
    for(; fl->nxt_free != NULL; fl = fl->nxt_free);
    fl->nxt_free = block;
}
struct metadata *find_block(size_t size)
{
    warnx("%d" , METASIZE);
    struct metadata *block;
    void *map;
    if(freelist == NULL) // first block
    {
        size = ((size + METASIZE)/4096 + 1)*4096; // manage size if > 4096
        map = mmap(NULL , size , PROT_READ | PROT_WRITE , \
                                MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0);
        if(map == MAP_FAILED)
            return NULL;
        block = map;
        freelist = block;     // freelist is now set to starting of the block
        set_block(block , size - METASIZE , 1 , NULL , NULL , NULL);
    } 
    else
    {
        block = freelist;
        while(block->nxt_free != NULL && block->nxt_free->size > size) 
            block = block->nxt_free;
        if(block->nxt_free == NULL)
            block->nxt_free = mmap(NULL , 4096 , PROT_READ | PROT_WRITE , \
                                    MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0);
        block = block->nxt_free;
        
    }
    return block;
}

void add_block(struct metadata *b1 , size_t s)
{
    if((b1->size) > s + METASIZE + METASIZE) //split- create a new block if you can
    {
        int prev_size = b1->size;
        struct metadata *b2 = addpointer(b1 + 1 , s);    // b1 + 1 is data
        set_block(b2 , prev_size - s - METASIZE , 1 , b1->next , b1 , NULL);
        set_block(b1 , s , 0 , b2 , b1->prev , b1->nxt_free);
        add_to_free(freelist , b2);
    }
    else
        set_block(b1 , s , 0 , b1->next , b1->prev , b1->nxt_free);
    remove_from_free(freelist , b1);
}

__attribute__((visibility("default")))
void *malloc(size_t __attribute__((unused)) size)
{
    struct metadata *block;
    size = allign(size);
    block = find_block(size);
    add_block(block ,size);
	return block + 1;
}

__attribute__((visibility("default")))
void free(void __attribute__((unused)) *ptr)
{
}

__attribute__((visibility("default")))
void *realloc(void __attribute__((unused)) *ptr,
             size_t __attribute__((unused)) size)
{
	return NULL;
}

__attribute__((visibility("default")))
void *calloc(size_t __attribute__((unused)) nmemb,
             size_t __attribute__((unused)) size)
{
	return NULL;
}
