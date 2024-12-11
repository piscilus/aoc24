/*
 * Doubly linked list.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "dll.h"

#include <stdio.h>
#include <string.h>

dll_node_t*
dll_create_node(node_payload_t data)
{
    dll_node_t* new_node = malloc(sizeof(dll_node_t));
    if (new_node == NULL)
        return NULL;
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

void
dll_add_node(dll_node_t** head, node_payload_t data)
{
    dll_node_t* new_node = dll_create_node(data);

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }

    dll_node_t* current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = new_node;
    new_node->prev = current;
}

void
dll_add_node_after(dll_node_t* prev_node, node_payload_t data)
{
    if (prev_node == NULL)
        return;

    dll_node_t* new_node = dll_create_node(data);
    new_node->next = prev_node->next;
    new_node->prev = prev_node;
    if (prev_node->next != NULL)
        prev_node->next->prev = new_node;

    prev_node->next = new_node;
}

void
dll_print(dll_node_t* head)
{
    dll_node_t* current = head;
    int count = 0;
    while (current != NULL)
    {
        printf("%d: %llu\n", count, current->data);
        current = current->next;
        count++;
    }
    printf("entries = %d\n", count);
}

void
dll_free(dll_node_t* head)
{
    dll_node_t* current = head;
    dll_node_t* next_node;
    while (current != NULL)
    {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}
