#define _GNU_SOURCE
#include <stddef.h>
#include<err.h>
#include<sys/mman.h>
#include "malloc.h"
#define METASIZE allign(sizeof(struct metadata))
struct metadata *freelist = NULL;

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
    if(fl == block)
    {
        freelist = freelist->nxt_free;//use freelist directly as changing head
        return;
    }
    for(; fl->nxt_free != block; fl = fl->nxt_free);
    fl->nxt_free = fl->nxt_free->nxt_free; 
}
void add_to_free(struct metadata *fl , struct metadata *block)
{
    for(; fl->nxt_free != NULL; fl = fl->nxt_free);
    fl->nxt_free = block;
}
void print_fl(struct metadata *fl)
{
    warnx( " FREELIST " );
    for(; fl != NULL; fl = fl->nxt_free)
        warnx("\t %ld \t" , fl->size);
}
void *newpage(int size)
{
    struct metadata *block;
    size = ((size + METASIZE)/4096 + 1)*4096; // manage size if > 4096
    block = mmap(NULL , size , PROT_READ | PROT_WRITE , \
            MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0);
    if(block == MAP_FAILED)
        return NULL;
    set_block(block , size - METASIZE , 1 , NULL , NULL , NULL);
//    print_fl(freelist);
    return block;

}
struct metadata *find_block(size_t size)
{
    struct metadata *block;
    if(freelist == NULL) // first block
    {
        block = newpage(size);
        if(block == NULL)
            return NULL;
        freelist = block;     // freelist is now set to starting of the block
    } 
    else
    {
        block = freelist;
        if(block->size > size)
            return block;
        while(block->nxt_free != NULL && block->nxt_free->size < size) 
            block = block->nxt_free;
        if(block->nxt_free == NULL)
        {
            block->nxt_free = newpage(size);
            if(block->nxt_free == NULL)
                return NULL;
        }
        block = block->nxt_free;
    }
    return block;
}

void add_block(struct metadata *b1 , size_t s)
{
//    warnx(" Malloc of size %ld in block %ld" , s , b1->size);
    if((b1->size) > s + 2* METASIZE) //split- create a new block if you can
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
    b1->nxt_free = NULL;
}

    __attribute__((visibility("default")))
void *malloc(size_t __attribute__((unused)) size)
{
    struct metadata *block;
    size = allign(size);
    block = find_block(size);
    if(block == NULL)
        return NULL;
    add_block(block ,size);
//    print_fl(freelist);
    return block + 1;
}

