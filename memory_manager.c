#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// define poolsize and blocksize
#define pool_size 1024
#define block_size 16

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
    //free in allocated space
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
    // resize 
    if (block == NULL) {
        return mem_alloc(size);
    }

    if (size == 0) {
        mem_free(block);
        return NULL;
    }

    size_t offset = (unsigned char*)block - memory_pool;

    size_t sizeing = 0;
    for (size_t i = offset; i<pool_size && allocated[i]; i++) {
        sizeing++;
    }

    if (sizeing >= size) {
        return block;
    }

    for (size_t i = 0; i < sizeing; i++) {
        ((unsigned char*)blocking[i] = (unsigned char*)block)[i];
    }

    mem_free(block);

    return blocking;
}


void mem_deinit() {
    // Delocate memory
    for (int i = 0; i < pool_size / block_size; i++) {
        allocated[i] = false;
    }
}
