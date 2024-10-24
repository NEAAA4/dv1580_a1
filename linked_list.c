#include "memory_manager.h"
#include "common_defs.h"

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

typedef struct Node {
    uint16_t data;
    struct Node* next;
} Node; 

void list_init(Node** head, size_t size) {
    *head = NULL; 
    Node* node = *head;

    for (size_t i = 0; i < size; i++) {
        Node* node = (Node*)mem_alloc(sizeof(Node));
        if (node == NULL) {
            return;
        }
    }
    node->next = *head;
    *head = node;
}

void list_insert(Node** head, uint16_t data) {
    // insert node
    Node* node = (Node*)mem_alloc(sizeof(Node)); 
    if (node == NULL) {
        printf("Memory allocation failed \n"); 
        return;
    }

    node->data = data; 
    node->next = NULL; 

    if (*head == NULL){
        *head = node; 
    }
    else {
        Node* here = *head;
        while (here->next != NULL) {
            here = here->next; 
        }
        here->next = node; 
    }
}

void list_insert_after(Node* prev_node, uint16_t data){
    // insert node after prev_node
    if (prev_node == NULL) {
        return; 
    }

    Node* node = (Node*)mem_alloc(sizeof(Node)); 
    if (node == NULL){
        printf("Memory allocation failed \n");
        return;   
    }
    node->data = data; 
    node->next = prev_node->next; 
    prev_node->next = node; 
}

void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    // insert node before next_node
    Node* node = (Node*)mem_alloc(sizeof(Node)); 
    if (next_node == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    node->data = data; 

    if (*head == next_node) {
        node->next = *head;
        *head = node; 
        return;
    }

    Node* here = *head;  
    while (here->next != next_node && here->next != NULL) {
        here = here->next; 
    } 

    if (here->next == next_node) {
        node->next = next_node; 
        here->next = node; 
    }
}

void list_delete(Node** head, uint16_t data) {
    // delete list
    if (*head == NULL) {
        return; 
    }

    Node* here = *head; 
    Node* bfore = NULL; 

    if (here != NULL && here->data == data) {
        *head = here->next; 
        mem_free(here); 
        return;
    }

    while (here->data != data && here != NULL) {
        bfore = here; 
        here = here->next;
    }

    if (here == NULL) {
        return; 
    }

    bfore->next = here->next; 
    mem_free(here);
}

Node* list_search(Node** head, uint16_t data) {
    //Search after node with data
    Node* here = *head; 
    while (here != NULL) {
        if (here->data == data) {
            return here; 
        }
        here = here->next; 
    }
    return NULL;
}

void list_display(Node** head) {
    // display list
    Node* here = *head; 
    printf("[");
    while (here != NULL) {
        printf("%d", here->data); 
        if (here->next != NULL) {
            printf(", ");
        } 
        here = here->next;
    }
    printf("]\n");
}

void list_display_range(Node** head, Node* start_node, Node* end_node){
    Node* here = *head;

    if (start_node == NULL) {
        start_node = *head;
    }

    while (here != NULL && here != start_node) {
        here = here->next;
    }
    if (here == NULL) {
        printf("No start node \n");
        return;
    }

    while (here != NULL) {
        printf("%d", here->data);
        if (here == end_node) {
            break;
        }
        here = here->next;
    }

    printf("\n");
    
}

int list_count_nodes(Node** head) {
    // count nodes in list
    Node* here = *head;
    int count = 0;
    while (here != NULL) {
        count ++; 
        here = here->next;
    }
    return count; 
}

void list_cleanup(Node** head) {
    // clean up list
    Node* here = *head;
    Node* node;
    
    while (here != NULL) {
        node = here->next;
        mem_free(here);
        here = node;
    } 
    *head = NULL; 
}


