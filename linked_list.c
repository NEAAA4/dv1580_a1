#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void list_init(Node** head, size_t size) {
    *head = NULL;
    mem_init(size); 
}


void list_insert(Node** head, uint16_t data) {
    Node* new = (Node*) mem_alloc(sizeof(Node));
    if (new == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    new->data = data;
    new->next = NULL;

    if (*head == NULL) {
        *head = new; 
    } else {
        Node* here = *head;
        while (here->next != NULL) { 
            here = here->next;
        }
        here->next = new;
    }
}


void list_insert_after(Node* prev_node, uint16_t data) {
    if (prev_node == NULL) {
        printf("Prev_node is NULL\n");
        return;
    }

    Node* new = (Node*) mem_alloc(sizeof(Node));
    if (new == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    new->data = data;
    new->next = prev_node->next;
    prev_node->next = new;
}


void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    if (next_node == *head) {
        Node* new = (Node*) mem_alloc(sizeof(Node));
        if (new == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        new->data = data;
        new->next = *head;
        *head = new;
    } else {
        Node* prev_node = *head;
        while (prev_node != NULL && prev_node->next != next_node) {
            prev_node = prev_node->next;
        }

        if (prev_node == NULL) { 
            printf("Next_node not in list\n");
            return;
        }

        Node* new = (Node*) mem_alloc(sizeof(Node));
        if (new == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        new->data = data;
        new->next = next_node;
        prev_node->next = new;
    }
}


void list_delete(Node** head, uint16_t data) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* temp = NULL;

    if ((*head)->data == data) { 
        temp = *head;
        *head = (*head)->next;
        mem_free(temp);
        return;
    }

    Node* here = *head;
    while (here->next != NULL) { 
        if (here->next->data == data) { 
            temp = here->next;
            here->next = here->next->next;
            mem_free(temp);
            return;
        }
        here = here->next;
    }
    printf("Data not found in list\n");
}


Node* list_search(Node** head, uint16_t data) {
    if (*head == NULL) {
        return NULL;
    }

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
    Node* here = *head;
    printf("[");
    while (here) {
        printf("%u", here->data);
        if (here->next) {
            printf(", ");
        }
        here = here->next;
    }
    printf("]\n");
}


void list_display_range(Node** head, Node* start_node, Node* end_node) {
    if (*head == NULL) {
        printf("[]");
        return;
    }
    if (start_node == NULL) {
        start_node = *head;
    }

    Node* here = start_node;
    printf("[");
    int start = 1;

    while (here != NULL) {
        if (!start) {
            printf(", ");
        }
        printf("%u", here->data);
        if (here == end_node) {
            break;
        }
        here = here->next;
        start = 0;
    }
    printf("]");
}


int list_count_nodes(Node** head) {
    int count = 0;
    Node* here = *head;
    while (here != NULL) {
        count++;
        here = here->next;
    }
    return count;
}


void list_cleanup(Node** head) {
    Node* here = *head;
    while (here != NULL) {
        Node* next = here->next;
        mem_free(here);
        here = next;
    }
    *head = NULL;
}

