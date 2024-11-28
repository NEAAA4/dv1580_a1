#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to initialize a linked list
void list_init(Node** head, size_t size) {
    if (head == NULL) {
        printf("Error: Invalid head pointer \n");
        return;
    }
    // Initialize the head to NULL
    *head = NULL;

    // Initialize the memory pool
    mem_init(size);  // Call mem_init to initialize the memory pool
}

// Function to insert a node at the end of the list
void list_insert(Node** head, uint16_t data) {
    Node* new = (Node*) mem_alloc(sizeof(Node));
    printf("New Node :: %p \n", new);
    if (new == NULL) {
        printf("Error: Memory allocation failed\n");
        return;
    }

    new->data = data;
    new->next = NULL;


    if (*head == NULL) {
        *head = new;
        printf("Inserted first node with data: %u at %p\n", data, (void*)new);
    } else {

        Node* current = *head;

        while (current->next != NULL) {
            printf("Current:: %p, Data: %u, Next: %p\n", (void*)current, current->data, (void*)current->next);
            current = current->next;
        }
        current->next = new;
        printf("Inserted node with data: %u at %p\n", data, (void*)new);
    }
}

void list_insert_after(Node* prev_node, uint16_t data) {
    if (prev_node == NULL) {
        printf("Error: prev_node is NULL\n");
        return;
    }


    Node* new = (Node*) mem_alloc(sizeof(Node));
    if (new == NULL) {
        printf("Error: Memory allocation failed\n");
        return;
    }

    new->data = data;
    new->next = prev_node->next;
    
    prev_node->next = new;
}


void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    if (next_node == *head) {
        Node* new = (Node*) mem_alloc(sizeof(Node));  
        new->data = data;
        new->next = *head;
        *head = new;
    } else {
        Node* prev_node = *head;
        while (prev_node != NULL && prev_node->next != next_node) {
            prev_node = prev_node->next;
        }
        
        if (prev_node == NULL) {
            printf("Next_node not in list \n");
            return;
        }

        Node* new = (Node*) mem_alloc(sizeof(Node)); 
        new->data = data;
        new->next = next_node;
        prev_node->next = new;
    }
}


void list_delete(Node** head, uint16_t data) {
    if (*head == NULL) {
        printf("Error: List is empty\n");
        return; 
    }

    if ((*head)->data == data) {
        Node* temp = *head; 
        *head = (*head)->next;  
        free(temp);
        return;
    }

    Node* current = *head;
    while (current->next != NULL) {
        if (current->next->data == data) {
            Node* temp = current->next;
            current->next = current->next->next; 
            free(temp); 
            return;  
        }
        current = current->next;  
    }

    printf("Error: Data not found in the list\n");
}

Node* list_search(Node** head, uint16_t data) {
    if (*head == NULL) {
        return NULL;
    }

    Node* current = *head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


void list_display(Node** head) {
    Node* current_node = *head;
    printf("[");
    while (current_node) {
        printf("%u", current_node->data);
        if (current_node->next) {
            printf(", ");
        }
        current_node = current_node->next;
    }
    printf("]\n");
}


void list_display_range(Node** head, Node* start_node, Node* end_node) {
    if (start_node == NULL) {
        start_node = *head;
    }
    if (end_node == NULL) {
        end_node = *head;
        while (end_node->next) {
            end_node = end_node->next;
        }
    }
    printf("[");
    Node* current_node = start_node;
    while (current_node != end_node->next) {
        printf("%d", current_node->data);
        if (current_node->next != end_node->next) {
            printf(", ");
        }
        current_node = current_node->next;
    }
    printf("]\n");
}

int list_count_nodes(Node** head) {
    int count = 0;
    Node* current_node = *head;
    while (current_node) {
        count++;
        current_node = current_node->next;
    }
    return count;
}

void list_cleanup(Node** head) {
    Node* current = *head;
    while (current != NULL) {

        Node* next = current->next; 
        mem_free(current);          
        current = next;             
    }
    *head = NULL;
}
