#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef struct Memory {
    size_t size;          
    bool free;        
    struct Memory *next; 
} Memory;


static void *memory_pool = NULL;
static Memory *lista = NULL;

#define ALIGN sizeof(Memory)

void mem_init(size_t size) {
    if (memory_pool != NULL) {
        return;
    }
    
    memory_pool = malloc(size);
    if (!memory_pool) {
        return;
    }
    
    lista = (Memory *) memory_pool;
    lista->size = size - sizeof(Memory);  
    lista->free = true;                 
    lista->next = NULL;                     
}


void* mem_alloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    size = align_size(size);
    Memory *here = lista;
    Memory *bfore = NULL;

    while (here != NULL) {
        if (here->free && here->size >= size) {
            if (here->size >= size + sizeof(Memory) + ALIGN) {
                Memory *new = (Memory *)((char *)here + sizeof(Memory) + size);
                new->size = here->size - size - sizeof(Memory);
                new->free = true;
                new->next = here->next;

                here->size = size;
                here->free = false;
                here->next = new;

            } else {                
                here->free = false;
            }
            return (char *)here + sizeof(Memory);
        }

        bfore = here;
        here = here->next;
    }
    return NULL;
}


void mem_free(void* block) {
    if (block == NULL) {
        return; 
    }

    Memory *here = (Memory *)((char *)block - sizeof(Memory));
    if (!block_here->free) {
        block_here->free = true;

        Memory *block_next = block_here->next;
        if (block_next != NULL && block_next->free) {
            block_here->size += block_next->size + sizeof(Memory);
            block_here->next = block_next->next; 
        }

        Memory *block_bfore = lista;
        while (block_bfore != NULL && block_bfore->next != block_here) {
            block_bfore = block_bfore->next;
        }

        if (block_bfore != NULL && block_bfore->free) {
            block_bfore->size += block_here->size + sizeof(Memory);
            block_bfore->next = block_here->next; 
        }
    } else {
        return;
    }
}

void* mem_resize(void* block, size_t size) {
    if (block == NULL) {
        return mem_alloc(size);  
    }

    Memory *block_here = (Memory *)((char *)block - sizeof(Memory));
    if (block_here->free) {
        return NULL;
    }

    size = align_size(size);
    if (size <= block_here->size) {
        block_here->size = size;
        return block;
    }

    Memory *block_next = block_here->next;
    if (block_next != NULL && block_next->free && (block_here->size + sizeof(Memory) + block_next->size) >= size) {
        block_here->size += block_next->size + sizeof(Memory);
        block_here->next = next_block->next; 
        block_here->free = false; 
        return block;
    }

    void* new = mem_alloc(size);
    if (new == NULL) {
        return NULL; 
    }

    memcpy(new, block, block_here->size < size ? block_here->size : size);
    mem_free(block);
    return new; 
}

void mem_deinit() {
    if (memory_pool == NULL) {
        return;
    }

    Memory *here = lista;
    while (here != NULL) {
        if (!here->free) {
            printf("Freeing allocated block of size %zu at %p\n", current->size, (void *)current);
        }
        here = here->next;
    }
    free(memory_pool);
    memory_pool = NULL; 
    lista = NULL;   
}
