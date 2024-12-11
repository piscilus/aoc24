/*
 * Doubly linked list.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <stdlib.h>

typedef unsigned long long node_payload_t;

typedef struct node
{
    node_payload_t data;
    struct node* prev;
    struct node* next;
} dll_node_t;

dll_node_t*
dll_create_node(node_payload_t data);

void
dll_add_node(dll_node_t** head, node_payload_t data);

void
dll_add_node_after(dll_node_t* prev_node, node_payload_t data);

void
dll_print(dll_node_t* head);

void
dll_free(dll_node_t* head);
