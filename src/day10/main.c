/*
 * Main program for advent of code 2024 day 10.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_GRID2D_IMPLEMENTATION
#define PLIB_GRID2D_NO_NAMESPACE
#include "plib_grid2d.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static void
search_paths(grid2d_t* g, size_t y, size_t x, int height)
{
    if (height == '9')
    {
        g->flags[y][x]++;
        return;
    }
    int next_height = height + 1;
    if ((y > 0U) && (g->grid[y - 1U][x] == next_height))
        search_paths(g, y - 1U, x, next_height);
    if ((x > 0U) && (g->grid[y][x - 1U] == next_height))
        search_paths(g, y, x - 1U, next_height);
    if ((y < (g->max_y - 1U)) && (g->grid[y + 1U][x] == next_height))
        search_paths(g, y + 1U, x, next_height);
    if ((x < (g->max_x - 1U)) && (g->grid[y][x + 1U] == next_height))
        search_paths(g, y, x + 1U, next_height);
}

static void
count_and_clear(grid2d_t* g, int* p1, int* p2)
{
    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if (g->flags[y][x] != 0U)
            {
                (*p1)++;
                *p2 += g->flags[y][x];
                g->flags[y][x] = 0U;
            }
        }
    }
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 10: Day Hoof It\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* g = grid2d_init(*(argv + 1));
    if (g == NULL)
    {
        fprintf(stderr, "Input error.\n");
        exit(EXIT_FAILURE);
    }

    int p1 = 0;
    int p2 = 0;
    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if (g->grid[y][x] == '0')
            {
                search_paths(g, y, x, '0');
                count_and_clear(g, &p1, &p2);
            }
        }
    }

    printf("Part 1: %d\n", p1);
    printf("Part 2: %d\n", p2);

    grid2d_free(g);

    return EXIT_SUCCESS;
}
