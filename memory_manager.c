#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define pool_size 5000 // storlek på memory pool
#define block_size 16 // storlek på memory block


// char memory_pool[pool_size]; 
// bool allocated[pool_size/block_size];
size_t num_blocks;
size_t size;


void* memory_pool = NULL;
bool* allocated = NULL;


void mem_init(size_t size) {
    // Allocate memory static 

    memory_pool = malloc(size);
    if (memory_pool == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    allocated = malloc(size / block_size * sizeof(bool));
    if (allocated == NULL) {
        printf("Memory allocation failed\n");
        free(memory_pool);
        return;
    }

    for (int i = 0; i < size / block_size; i++) {
        allocated[i] = false;
    }
}

void* mem_alloc(size_t size){
    //allocate in free space
    if (size > block_size) {
        return NULL; 
    }
    for (int i = 0; i < pool_size / block_size; i++) {
        if (!allocated[i]) {
            allocated[i] = true;
           return (void*)((char*)memory_pool + i * block_size); 
        }
    }
    return NULL;
}


void mem_free(void* block){
    if (block >= (void*)memory_pool && block < (void*)(memory_pool + pool_size)) {
        return; // kollar så block är inom räckvidd
    } 
    int index = ((char*)block - (char*)memory_pool) / block_size;

    if (index >= 0 && index < pool_size / block_size) {
        allocated[index] = false; 
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
