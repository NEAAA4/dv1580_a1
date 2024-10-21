#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#define pool_size 1024 // storlek på memory pool
#define block_size 16 // storlek på memory block

char memory_pool[pool_size]; 
bool allocated[pool_size/block_size];
size_t num_blocks;


void mem_init(size_t size) {
    // Allocate memory static 
    for (int i = 0; i < pool_size / block_size; i++) {
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
            return (void*)&memory_pool[i * block_size]; 

        } 
    }
    return NULL;
}


void mem_free(void* block){
    if (block >= (void*)memory_pool && block < (void*)(memory_pool + pool_size)) {
        return; 
    } 
    size_t offset = (unsigned char*)block - (unsigned char*)memory_pool;
    size_t index = offset / block_size; 

    if (index < num_blocks) {
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

