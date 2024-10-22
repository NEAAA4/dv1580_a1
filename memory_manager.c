#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef struct Memory {
    size_t size;
    struct Memory* next;
    int free;
} Memory;

static void* memory_pool = NULL;
static size_t pool_size = 0;
static Memory* allocated = NULL;

void mem_init(size_t size) {
    memory_pool = malloc(size);
    if (memory_pool == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    pool_size = size;
    allocated = (Memory*)memory_pool;
    allocated->size = size - sizeof(Memory*);
    allocated->next = NULL;
    allocated->free = 1;

}

void* mem_alloc(size_t size) {
    Memory* here = allocated;
    Memory* bfore = NULL;

    while (here != NULL) {
        if (here->free == 1 && here->size >= size) {
            if (here->size >= size + sizeof(Memory)) {
                Memory* block = (Memory*)((uintptr_t)here + sizeof(Memory)+ size);
                block->size = here->size - sizeof(Memory) - size;
                block->next = here->next;
                block->free = 1;

                here->size = 0;
                here->next = block;
            }

            here->free = 0;
            return (void*)((uintrp_t)here + sizeof(Memory));
    }
    bfore = here;
    here = here->next;
}


void mem_free(void* block){
    if (block == NULL) {
        return;
    }

    Memory* head = (Memory*)((uintptr_t)block - sizeof(Memory));
    head->free = 1;

    Memory* here = free;
    while (here != NULL) {
        if (here->free && here->next != NULl && here->next->free) {
            here->size += sizeof(Memory) + here->next->size;
            here->next = here->next->next;
        }
        here = here->next;
    }
}

void* mem_resize(void* block, size_t size) {
    void* new = mem_alloc(size);
    if (new == NULL) {
        return NULL;
    }
    if (block != NULL) {
        mem_free(block);
    }
    return new;
}


void mem_deinit() {
    // Deallocate memory static
    for (int i = 0; i < pool_size / block_size; i++) {
        allocated[i] = false; // sätter allt som tomt
    }
}
