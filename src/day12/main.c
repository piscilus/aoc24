/*
 * Main program for advent of code 2024 day 12.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_GRID2D_IMPLEMENTATION
#define PLIB_GRID2D_NO_NAMESPACE
#include "plib_grid2d.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int area;
    int perimeter;
} data_t;

static void
walk(grid2d_t* g, size_t y, size_t x, char c, data_t* d)
{
    if (g->flags[y][x] > 0U)
        return;

    g->flags[y][x]++;
    d->area++;

    if ((y > 0U) && (g->grid[y - 1U][x] == c))
        walk(g, y - 1U, x, c, d);
    else
        d->perimeter++;

    if ((x > 0U) && (g->grid[y][x - 1U] == c))
        walk(g, y, x - 1U, c, d);
    else
        d->perimeter++;

    if ((y < (g->max_y - 1U)) && (g->grid[y + 1U][x] == c))
        walk(g, y + 1U, x, c, d);
    else
        d->perimeter++;

    if ((x < (g->max_x - 1U)) && (g->grid[y][x + 1U] == c))
        walk(g, y, x + 1U, c, d);
    else
        d->perimeter++;

    return;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 12: Garden Groups\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* g = grid2d_init(argv[1]);

    int count = 0;
    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if (g->flags[y][x] == 0U)
            {
                data_t data = {0};
                walk(g, y, x, g->grid[y][x], &data);
                count += data.area * data.perimeter;
            }
        }
    }
    printf("Part 1: total price = %d\n", count);

    grid2d_free(g);

    return EXIT_SUCCESS;
}
