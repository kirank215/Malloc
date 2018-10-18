/*#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<err.h>
#include "malloc.h"
#define METASIZE allign(sizeof(struct metadata))

extern struct metadata *freelist;
void *allign_pointer(void *p)
{
    p = addpointer(p , (size - 1));
    p = p & ~(size - 1);
    char *c = p;
    int size = sizeof(size_t);
    return (c + sizeof(size_t)) & (char *)~(size - 1);
}
void fill_zero(void *p , size_t size)
{
    char *c = p;
    for(size_t i = 0; i < size; i++)
    {
        *(c + i)  = 0;
    }
}
    __attribute__((visibility("default")))
void free(void __attribute__((unused)) *ptr)
{
  //  if(ptr != allign(ptr))
   //     warnx(" Invalid free ptr ");
    struct metadata *block = addpointer(ptr , -METASIZE);
    block->is_free = 1;
    add_to_free(freelist , block);
}
    __attribute__((visibility("default")))
void *calloc(size_t __attribute__((unused)) nmemb,
        size_t __attribute__((unused)) size)
{
    struct metadata *block = malloc(nmemb * size);
    if(block == NULL)
        return NULL;
    block -= 1;
    fill_zero(block , size);
    return block;
}
*/
