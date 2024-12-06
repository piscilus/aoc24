/*
 * Main program for advent of code 2024 day 5.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_FLR_IMPLEMENTATION
#define PLIB_FLR_NO_NAMESPACE
#include "plib_flr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int x;
    int y;
} rule_t;

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 4: Print Queue\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));

    size_t num_locations = flr_num_lines(f);
    if (num_locations == 0U)
    {
        fprintf(stderr, "File empty?!\n");
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    rule_t* rules = malloc(sizeof(rule_t) * num_locations);
    if (rules == NULL)
    {
        fprintf(stderr, "Buy more RAM!\n");
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    int num_rules = 0;
    const char* p;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        if (strlen(p) == 0U)
            break;
        else
        {
            sscanf(p, "%d|%d", &rules[num_rules].x, &rules[num_rules].y);
            num_rules++;
        }
    }
#if 0
    printf("%d\n", num_rules);
    for (int i = 0; i < num_rules; i++)
    {
        printf("%d %d %d\n", i, rules[i].x, rules[i].y);
    }
#endif
    int result = 0;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        size_t len = strlen(p);
        size_t num_pages = 0U;
        int* pages = malloc(len * sizeof(int));
        if (pages == NULL)
            break;
        char *token = strtok(p, ",");
        while(token)
        {
            pages[num_pages] = atoi(token);
            token = strtok(NULL, ",");
            num_pages++;
        }
        int correct = 1;
        for (size_t page = 0U; page < num_pages; page++)
        {
            for (size_t rule = 0U; rule < num_rules; rule++)
            {
                if (pages[page] == rules[rule].x)
                {
                    for (size_t page2 = page; page2-- > 0U; )
                    {
                        if (rules[rule].y == pages[page2])
                        {
                            correct = 0;
                            goto stop;
                        }
                    }
                }
                if (pages[page] == rules[rule].y)
                {
                    for (size_t page2 = page; page2 < num_pages; page2++ )
                    {
                        if (rules[rule].x == pages[page2])
                        {
                            correct = 0;
                            goto stop;
                        }
                    }
                }
            }
        }
        stop:
        if (correct)
            result += pages[num_pages / 2U];
        free(pages);
    }
    printf("Part 1: %d\n", result);

    flr_free(f);

    return EXIT_SUCCESS;
}
