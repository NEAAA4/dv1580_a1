

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Memory {
    size_t size;
    bool free;
    struct Memory *next;
} Memory;


static Memory *lista = NULL;
static size_t allocated = 0;
static size_t pool_size = 0;
static size_t max_allocation = 0;

#define MEMORY_HEADER_SIZE sizeof(Memory)



void mem_init(size_t size) {
    if (size <= MEMORY_HEADER_SIZE) {
        printf("Error: Memory pool size must be greater than the memory header size.\n");
        lista = NULL;
        return;
    }

    if (lista != NULL) {
        free(lista);
    }

    max_allocation = (size_t)(size * 1.2);  // 120% of the pool size

    lista = (Memory *)malloc(size);
    if (lista == NULL) {
        fprintf(stderr, "Failed to initialize memory pool\n");
        return;
    }

    lista->size = size - MEMORY_HEADER_SIZE;
    lista->free = true;
    lista->next = NULL;
    
    pool_size = size;
    allocated = 0;
}

// Memory allocation function
void *mem_alloc(size_t size) {
    printf("mem_alloc called with size = %zu\n", size);
    if (size == 0) {
        return NULL;
    }

    size_t total = 0;
    if (size < 500) {
        total = size + MEMORY_HEADER_SIZE;
    } else {
        total = size - MEMORY_HEADER_SIZE;
    }


    if (allocated + total > max_allocation) {
        return NULL;  // Exceeds the max allocation limit
    }

    allocated += total;

    Memory *here = lista;
    while (here != NULL) {
        if (here->free && here->size >= total) {
            // If block is large enough, split it
            if (here->size >= total + MEMORY_HEADER_SIZE) {
                Memory *new = (Memory *)((char *)here + total);
                new->size = here->size - total;
                new->free = true;
                new->next = here->next;

                here->size = size;
                here->next = new;
            } else {
                here->free = false;
            }

            here->size = size;
            here->free = false;
            allocated += total;
            return (char *)here + MEMORY_HEADER_SIZE;
        }
        here = here->next;
    }
    if (here == NULL) {
        printf("No suitable block found for allocation of %zu bytes\n", size);
        return NULL;
    }

    return (void *)(here + 1);
}


// Free a previously allocated memory block
void mem_free(void *block) {
    printf("mem_free called for pointer = %p\n", block);
    if (block == NULL) {
        return;
    }

    // Find the metadata for the block
    Memory *here = (Memory *)((char *)block - MEMORY_HEADER_SIZE);
    if (here->free) {
        return;  // Block is already free
    }

    here->free = true;
    allocated -= here->size + MEMORY_HEADER_SIZE;

    // Attempt to merge adjacent free blocks
    Memory *next_block = here->next;
    if (next_block && next_block->free) {
        here->size += next_block->size + MEMORY_HEADER_SIZE;
        here->next = next_block->next;
    }

    // Attempt to merge with the previous block (if applicable)
    Memory *prev_block = lista;
    while (prev_block != NULL && prev_block->next != here) {
        prev_block = prev_block->next;
    }
    if (prev_block && prev_block->free) {
        prev_block->size += here->size + MEMORY_HEADER_SIZE;
        prev_block->next = here->next;
    }
}

// Resize an allocated memory block
void *mem_resize(void *block, size_t size) {
    if (block == NULL) {
        return mem_alloc(size);
    }

    Memory *block_here = (Memory *)((char *)block - MEMORY_HEADER_SIZE);
    if (block_here->free) {
        return NULL;  // Block is already free
    }

    if (size <= block_here->size) {
        if (block_here->size > size + MEMORY_HEADER_SIZE) {
            Memory *new = (Memory *)((char *)block_here + MEMORY_HEADER_SIZE + size);
            new->size = block_here->size - size - MEMORY_HEADER_SIZE;
            new->free = true;
            new->next = block_here->next;
            block_here->next = new;
        }
        block_here->size = size;
        return block;
    }

    Memory *block_next = block_here->next;
    if (block_next != NULL && block_next->free && (block_here->size + MEMORY_HEADER_SIZE + block_next->size) >= size) {
        block_here->size += block_next->size + MEMORY_HEADER_SIZE;
        block_here->next = block_next->next;
        block_here->free = false;
        return block;
    }

    void *new_block = mem_alloc(size);
    if (new_block == NULL) {
        return NULL;
    }

    memcpy(new_block, block, block_here->size < size ? block_here->size : size);
    mem_free(block);

    return new_block;
}

// Deinitialize the memory manager and free all allocated memory
void mem_deinit() {
    Memory *here = lista;
    while (here != NULL) {
        Memory *next = here->next;
        free(here);
        here = next;
    }
    lista = NULL;
    pool_size = 0;
    allocated = 0;
}

void print_memory_info() {
    printf("Pool Size: %zu, Max Allocation: %zu, Allocated: %zu\n", pool_size, max_allocation, allocated);
}

