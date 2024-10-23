#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef struct Memory {
    uint16_t size;          
    bool free;        
    struct Memory *next; 
} Memory;


void* memory_pool = NULL;
Memory* lista = NULL;
size_t allocated = 0;
size_t pool_size = 0;


// void mem_init(size_t size) {
//     if (memory_pool != NULL) {
//         return;
//     }

//     memory_pool = malloc(size);

//     if (memory_pool == NULL) {
//         return;
//     }

//     lista = (Memory*)memory_pool;
//     lista->size = size - sizeof(Memory);
//     lista->free = true;
//     lista->next = NULL; 

//     allocated = 0;  
//     pool_size = size;   
// }

void mem_init(size_t size) {
    if (memory_pool == NULL) {
        memory_pool = malloc(size);
        if (!memory_pool) {
            printf("Failed to allocate %zu \n", size);
            return;
        }
        pool_size = size; 
        allocated = 0; 

        lista = (Memory *)memory_pool;
        lista->size = size - sizeof(Memory);
        lista->free = true;
        lista->next = NULL;

    }
}


void* mem_alloc(size_t size) {
    if (size == 0) {
        return NULL; 
    }

    size_t total = size - sizeof(Memory);

    if (allocated + total > pool_size) {
        printf("%ld \n", total);
        printf("%ld, %ld \n", allocated, pool_size);
        printf("Not enough memory \n");
        return NULL; 
    }

    Memory *here = lista;

    while (here != NULL) {
        if (here->free && here->size >= total) {
            if (here->size >= total + sizeof(Memory)) {
                Memory *new = (Memory *)((char *)here + total);
                new->size = here->size - total;
                new->free = true;
                new->next = here->next;

                here->size = size;
                here->next = new;                
            }
            else {
                here->free = false;
            }

            allocated += total; 
            here->free = false;
            return (char *)here + sizeof(Memory);
        }
        here = here->next;
    }
    printf("no suitable block found for allocation of %zu bytes \n", size);
    return NULL;
}


void mem_free(void* block) {
    if (block == NULL) {
        return; 
    }

    Memory* here = (Memory*)((char*)block - sizeof(Memory));
    if (here->free) {
        return; 
    }

    here->free = true;
    allocated -= here->size + sizeof(Memory);
}


void* mem_resize(void* block, size_t size) {
    if (block == NULL) {
        return mem_alloc(size); 
    }

    Memory *block_here = (Memory *)((char *)block - sizeof(Memory));
    if (block_here->free) {
        return NULL; 
    }

    if (size <= block_here->size) {
        if (block_here->size > size + sizeof(Memory)) {
            Memory *new_block = (Memory*)((char*)block_here + sizeof(Memory) + size);
            new_block->size = block_here->size - size - sizeof(Memory);
            new_block->free = true;
            new_block->next = block_here->next;
            block_here->next = new_block;
        }
        block_here->size = size; 
        return block; 
    }

    Memory *block_next = block_here->next;
    if (block_next != NULL && block_next->free && (block_here->size + sizeof(Memory) + block_next->size) >= size) {
        block_here->size += block_next->size + sizeof(Memory);
        block_here->next = block_next->next; 
        block_here->free = false; 
        return block; 
    }

    void* new_block = mem_alloc(size);
    if (new_block == NULL) {
        return NULL; 
    }

    memcpy(new_block, block, block_here->size < size ? block_here->size : size);
    mem_free(block);
    
    return new_block; 
}


void mem_deinit() {
    if (memory_pool != NULL) {
        free(memory_pool);
        memory_pool = NULL;
        lista = NULL;
        pool_size = 0;
        allocated = 0;
    }
}
