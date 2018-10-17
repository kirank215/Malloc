#include <stddef.h>

void *freelist = NULL;

void *find_block(size_t size)
{
    struct metadata *block;
    void *map;
    if(map == NULL) // first block
    {
        map = mmap(NULL , 4096 , PROT_READ | PROT_WRITE , MAP_PRIVATE | \
                            MAP_ANONYMUS , -1 , 0);
        if(map == MAP_FAILED)
            return NULL;
        block = map;
    }
    else
    {
        block = freelist;
        while(block->nxt_free != NULL && block->nxt_free->size > size) 
            block = block->nxt_free;
        if(block->nxt_free == NULL)
            block->nxt_free = mmap(NULL , 4096 , PROT_READ | PROT_WRITE , \
                                    MAP_PRIVATE | MAP_ANONYMUS , -1 , 0);
        void *tmp = block->nxt_free;
        block->nxt_free = block->nxt_free->nxt_free;
        block = tmp;
    }
    
            
}
__attribute__((visibility("default")))
void *malloc(size_t __attribute__((unused)) size)
{
	return NULL;
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
