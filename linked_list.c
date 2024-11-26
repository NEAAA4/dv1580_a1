// linked_list.c
#include "linked_list.h"
#include <stdio.h>

void list_init(Node** head, size_t size) {
    *head = NULL;
    printf("SIZE:: %ld\n", size);
    mem_init(size); // Initialize memory management with the given size
}

void list_insert(Node** head, uint16_t data) {
    printf("Hello \n");
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    printf("New node: %p \n", new_node);
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;  // First node becomes the head
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
    printf("Inserted node with data = %u\n", data);
}



void list_insert_after(Node* prev_node, uint16_t data) {
    if (prev_node == NULL) {
        fprintf(stderr, "Previous node cannot be NULL\n");
        return;
    }
    Node* new_node = (Node*) mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    Node* new_node = (Node*) mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    new_node->data = data;

    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    Node* temp = *head;
    while (temp != NULL && temp->next != next_node) {
        temp = temp->next;
    }

    if (temp == NULL) {
        fprintf(stderr, "The given next node is not present in the list\n");
        mem_free(new_node);
        return;
    }

    new_node->next = next_node;
    temp->next = new_node;
}

void list_delete(Node** head, uint16_t data) {
    Node* temp = *head;
    Node* prev = NULL;

    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        fprintf(stderr, "Data not found in the list\n");
        return;
    }

    if (prev == NULL) {
        *head = temp->next;
    } else {
        prev->next = temp->next;
    }

    mem_free(temp);
}

Node* list_search(Node** head, uint16_t data) {
    Node* temp = *head;
    while (temp != NULL) {
        if (temp->data == data) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void list_display(Node** head) {
    Node* temp = *head;
    printf("[");
    while (temp != NULL) {
        printf("%u", temp->data);
        temp = temp->next;
        if (temp != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
}

void list_display_range(Node** head, Node* start_node, Node* end_node) {
    Node* temp = start_node ? start_node : *head;
    printf("[");
    while (temp != NULL) {
        printf("%u", temp->data);
        if (temp == end_node) {
            break;
        }
        temp = temp->next;
        if (temp != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
}

int list_count_nodes(Node** head) {
    Node* temp = *head;
    int count = 0;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

void list_cleanup(Node** head) {
    Node* temp = *head;
    while (temp != NULL) {
        Node* next = temp->next;
        mem_free(temp);  // Use mem_free to return memory to the pool
        temp = next;
    }
    *head = NULL;
}

