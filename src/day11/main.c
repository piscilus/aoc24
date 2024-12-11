/*
 * Main program for advent of code 2024 day 11.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "dll.h"

#include <stdio.h>
#include <stdlib.h>

#define BLINKS (25U)

static node_payload_t
count_digits(node_payload_t n)
{
    if (n == 0)
        return 1;

    node_payload_t count = 0;
    while (n != 0)
    {
        n = n / 10;
        count++;
    }

    return count;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 11: Plutonian Pebbles\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    dll_node_t* head = NULL;
    node_payload_t value = 0;
    while(fscanf(fp, "%llu", &value) == 1)
        dll_add_node(&head, value);

    fclose(fp);

    for (size_t i = 0U; i < BLINKS; i++)
    {
        dll_node_t* current = head;
        while (current != NULL)
        {
            dll_node_t* next = current->next;
            if (current->data == 0U)
            {
                current->data = 1U;
            }
            else
            {
                node_payload_t digits = count_digits(current->data);
                if ((digits % 2U) == 0U)
                {
                    node_payload_t new_data = current->data;
                    node_payload_t split = 10U;
                    node_payload_t hd = digits / 2U;
                    while (hd > 1U)
                    {
                        split *= 10U;
                        hd--;
                    }
                    current->data /= split;
                    new_data %= split;
                    dll_add_node_after(current, new_data);
                }
                else
                {
                    current->data *= 2024U;
                }
            }
            current = next;
        }
    }

    dll_node_t* current = head;
    size_t count = 0U;
    while (current != NULL)
    {
        current = current->next;
        count++;
    }
    printf("Part 1: Stones after %d blinks = %zu\n", BLINKS, count);

    dll_free(head);

    return EXIT_SUCCESS;
}
