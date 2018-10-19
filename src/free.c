#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<err.h>
#include<string.h>
#include "malloc.h"
#define METASIZE allign(sizeof(struct metadata))

extern struct metadata *freelist;
int is_alligned(void *p)
{
    uintptr_t s = (sizeof(size_t)) - 1;
    void *p1 = (void *)((uintptr_t)(p) & ~(s));
    if (p1 == p)
        return 1;
    return 0;
}
    __attribute__((visibility("default")))
void free(void __attribute__((unused)) *ptr)
{
    if(ptr == NULL)
    {
 //       warnx("free null");
        return;
    }
    if(is_alligned(ptr) == 0)
    {
        warnx(" Invalid free ptr ");
        return ;
    }
    struct metadata *block = addpointer(ptr , -METASIZE);
//    warnx("free of size %ld" , block->size);
    block->is_free = 1;
    add_to_free(freelist , block);
   // print_fl(freelist);
}
    __attribute__((visibility("default")))
void *calloc(size_t __attribute__((unused)) nmemb,
        size_t __attribute__((unused)) size)
{
    void *block = malloc(nmemb * size);
    if(block == NULL)
        return NULL;
    block = memset(block , 0 , nmemb * size);
    block = addpointer(block , - METASIZE);
    return block;
}
