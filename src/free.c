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
    //warnx("free");
    if(ptr == NULL)
    {
      //  warnx("free null");
        return;
    }
    if(is_alligned(ptr) == 0)
    {
        warnx(" Invalid free ptr ");
        return ;
    }
    struct metadata *block = addpointer(ptr , -METASIZE);
    //warnx("free of size %ld" , block->size);
    block->is_free = 1;
    add_to_free(freelist , block);
    //print_fl(freelist);
}
    __attribute__((visibility("default")))
void *calloc(size_t __attribute__((unused)) nmemb,
        size_t __attribute__((unused)) size)
{
    //warnx("calloc");
    size_t numbytes = nmemb * size;
    if( nmemb != 0 && numbytes/nmemb != size)
        warnx("overflow");
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
    //warnx("--Realloc--");
    if(ptr == NULL)
        return malloc(size);
    else if( size == 0)
        free(ptr);
    if(is_alligned(ptr) == 0)
        return NULL; 
    struct metadata *block = ptr;
    block = addpointer(block , - METASIZE);
    //warnx(" Realloc of %ld from %ld" , size , block->size);
    if(block->size > size)
        return block;
    /* Free block -- Check merge of block with prev and next -- If succesfull
        use for realloc -- 
    [ -- !!  call merge everytime you free with the block you free     !! -- ] 
    */
    free(addpointer(block,METASIZE));
    struct metadata *new_b = malloc(size);
    if(new_b == NULL)
        return NULL;
    new_b = mycopy(block , new_b , block->size);
    return new_b;
}
