#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include "malloc.h"
#define METASIZE allign(sizeof(struct metadata))

extern struct metadata *freelist;

    __attribute__((visibility("hidden")))
int is_alligned(void *p)
{
    uintptr_t s = (sizeof(size_t)) - 1;
    void *p1 = (void *)((uintptr_t)(p) & ~(s));
    if (p1 == p)
        return 1;
    return 0;
}

    __attribute__((visibility("hidden")))
void *mycopy(void *a , void *b , size_t size)
{
    char *src = a;
    char *dest = b;
    for(size_t i = 0; i < size; i++)
    {
        *(dest + i) = *(src + i);
    }   
    return dest;
}

    __attribute__((visibility("default")))
void free(void __attribute__((unused)) *ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    if(is_alligned(ptr) == 0)
    {
        return ;
    }
    struct metadata *block = addpointer(ptr , -METASIZE);
    block->is_free = 1;
    add_to_free(freelist , block);  // add to free list if could not merge
}

    __attribute__((visibility("default")))
void *calloc(size_t __attribute__((unused)) nmemb,
        size_t __attribute__((unused)) size)
{
    size_t numbytes = nmemb * size;
    if( nmemb != 0 && numbytes/nmemb != size)
        return NULL;
    void *block = malloc(nmemb * size);
    if(block == NULL)
        return NULL;
    block = memset(block , 0 , nmemb * size);
    return block;
}

    __attribute__((visibility("default")))
void *realloc(void __attribute__((unused)) *ptr,
        size_t __attribute__((unused)) size)
{
    if(ptr == NULL)
        return malloc(size);
    else if( size == 0)
        free(ptr);
    if(is_alligned(ptr) == 0)
        return NULL; 
    struct metadata *block;
    block = addpointer(ptr , - METASIZE);
    if(block->size > size)
        return ptr;
    free(ptr);
    struct metadata *new_b = malloc(size);
    if(new_b == NULL)
        return NULL;
    new_b = mycopy(ptr , new_b , block->size);
    return new_b;
    return NULL;
}
