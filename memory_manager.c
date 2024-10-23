#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


char* memory_pool;
bool* allocated;
size_t pool_size;
size_t block_size;


void mem_init(size_t size) {
    // Allocate memory static 

    memory_pool = malloc(size);
    if (memory_pool == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    pool_size = total_size / block_size;

    allocated = (bool*)malloc(pool_size * sizeof(bool));
    if (allocated == NULL) {
        printf("Memory allocation failed\n");
        free(memory_pool);
        return;
    }

    memset(allocated, 0, pool_size * sizeof(bool));

    // for (int i = 0; i < size / block_size; i++) {
    //     allocated[i] = false;
    // }
}

void* mem_alloc(size_t size){
    //allocate in free space
    if (size > block_size) {
        return NULL; 
    }
    for (int i = 0; i < pool_size / block_size; i++) {
        if (!allocated[i]) {
            allocated[i] = true;
           return (void*)&memory_pool[i * block_size];
        }
    }
    return NULL;
}


void mem_free(void* block){

    if (block == NULL) {
        return;
    }
    
    if (block >= (void*)memory_pool && block < (void*)(memory_pool + pool_size)) {
        return; 
    }

    int index = ((char*)block - (char*)memory_pool) / block_size;

    if (index >= 0 && index < pool_size / block_size && allocated[index]) {
        allocated[index] = false;
    }
    else {
        return;
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
        allocated[i] = false; 
    }
}
