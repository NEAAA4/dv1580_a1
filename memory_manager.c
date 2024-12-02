#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur för minnesblock
typedef struct Memory {
    size_t size;
    size_t starting;
    struct Memory* next;
    int freeing;
} Memory;

// Globala variabler
static char* memory_pool = NULL;
static Memory* lista = NULL;
static size_t pool_size = 0;

void mem_init(size_t size) {
    memory_pool = (char*)malloc(size);
    if (memory_pool == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    pool_size = size;
    lista = (Memory*)malloc(sizeof(Memory)); // metadata
    if (lista == NULL) {
        printf("Allocation of block list failed\n");
        free(memory_pool);
        memory_pool = NULL;
        return;
    }

    lista->starting = 0; 
    lista->size = size; 
    lista->freeing = 1;  // Blocket är fritt
    lista->next = NULL; 
}

void* mem_alloc(size_t size) {
    Memory* here = lista;
    while (here != NULL) {
        if (here->freeing && here->size >= size) {
            if (here->size > size) { // Dela block
                Memory* new = (Memory*)malloc(sizeof(Memory)); // nytt block
                if (new == NULL) {
                    printf("Allocation for new block failed \n");
                    return NULL;  
                }
                
                new->starting = here->starting + size; 
                new->size = here->size - size;
                new->freeing = 1;  
                new->next = here->next;

                here->size = size;
                here->freeing = 0; // inte fri
                here->next = new;
            } else {
                here->freeing = 0;
            }
            return memory_pool + here->starting; 
        }

        here = here->next;
    }
    return NULL; 
}

void mem_free(void* block) {
    if (!block) {
        return;
    }

    size_t starting = (char*)block - memory_pool;
    Memory* here = lista;
    Memory* bfore = NULL;

    while (here) {
        if (here->starting == starting) {
            if (here->freeing) {
                printf("Block already freed\n");
                return; 
            }
            here->freeing = 1;

            if (here->next && here->next->freeing) { // slå ihop med nästa
                Memory* bnext = here->next;
                here->size += bnext->size;
                here->next = bnext->next;
                free(bnext);
            }

            if (bfore && bfore->freeing) { // slå ihop med föregående
                bfore->size += here->size;
                bfore->next = here->next;
                free(here);
            }

            return;
        }
        bfore = here; 
        here = here->next;
    }
}

void* mem_resize(void* block, size_t size) {
    if (block == NULL) {
        return mem_alloc(size);
    }
    if (size == 0) {
        mem_free(block);
        return NULL;
    }

    Memory* here = lista;
    while (here != NULL) {
        if (memory_pool + here->starting == block) {
            break;
        }
        here = here->next;
    }
    if (here == NULL) {
        printf("Block not found in list\n");
        return NULL;
    }

    size_t shere = here->size;
    if (shere >= size) { 
        return block;
    } else {
        void* new = mem_alloc(size); // allokera nytt block
        if (new == NULL) {
            printf("Allocation failed for new block \n");
            return NULL;  
        }

        memcpy(new, block, shere); // kopiera data
        mem_free(block);

        return new;
    }
}

void mem_deinit() {
    if (memory_pool) { 
        free(memory_pool); // Frigör poolen direkt
        memory_pool = NULL;
        pool_size = 0;
    }

    Memory* here = lista;
    while (here) {
        Memory* next = here->next;
        free(here); // Frigör metadata
        here = next;
    }
    lista = NULL;
}
