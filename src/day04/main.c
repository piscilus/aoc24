/*
 * Main program for advent of code 2024 day 4.
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
#include <string.h>

static int
count_xmas(grid2d_t* g, const char* p)
{
    int c = 0;
    size_t p_size = strlen(p);

    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            size_t count[8] = {0};
            for (size_t i = 0U; i < p_size; i++)
            {
                if (x < g->max_x - p_size + 1U)
                {
                    if (g->grid[y][x + i] == p[p_size - i - 1U])
                        count[0]++;
                    if (g->grid[y][x + i] == p[i])
                        count[1]++;
                }
                if (y < g->max_y - p_size + 1U)
                {
                    if (g->grid[y + i][x] == p[p_size - i - 1U])
                        count[2]++;
                    if (g->grid[y + i][x] == p[i])
                        count[3]++;
                }
                if ((x < g->max_x - p_size + 1U) && (y < g->max_y - p_size + 1U))
                {
                    if (g->grid[y + i][x + i] == p[p_size - i - 1U])
                        count[4]++;
                    if (g->grid[y + i][x + i] == p[i])
                        count[5]++;
                }
                if ((x >= p_size - 1U) && (y < g->max_y - p_size + 1U))
                {
                    if (g->grid[y + i][x - i] == p[p_size - i - 1U])
                        count[6]++;
                    if (g->grid[y + i][x - i] == p[i])
                        count[7]++;
                }
            }
            for (size_t i = 0U; i < (sizeof(count) / sizeof(count[0])); i++)
                if (count[i] == p_size)
                    c++;
        }
    }

    return c;
}


static int
count_x_mas(grid2d_t* g)
{
    int c = 0;

    for (size_t y = 1U; y < g->max_y - 1U; y++)
    {
        for (size_t x = 1U; x < g->max_x - 1U; x++)
        {
            if (   (g->grid[y][x] == 'A')
                && (   (   (g->grid[y - 1U][x - 1U] == 'S')
                        && (g->grid[y + 1U][x + 1U] == 'M'))
                    || (   (g->grid[y - 1U][x - 1U] == 'M')
                        && (g->grid[y + 1U][x + 1U] == 'S')))

                && (   (   (g->grid[y - 1U][x + 1U] == 'S')
                        && (g->grid[y + 1U][x - 1U] == 'M'))
                    || (   (g->grid[y - 1U][x + 1U] == 'M')
                        && (g->grid[y + 1U][x - 1U] == 'S'))))
                c++;
        }
    }

    return c;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 4: Ceres Search\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* g = grid2d_init(argv[1]);
#if 0
    grid2d_print(g);
    printf("maxy %zu maxx %zu\n", g->max_y, g->max_x);
#endif

    int cp1 = count_xmas(g, "XMAS");
    printf("Part 1: XMAS count = %d\n", cp1);

    int cp2 = count_x_mas(g);
    printf("Part 2: X-MAS count = %d\n", cp2);

    grid2d_free(g);

    return EXIT_SUCCESS;
}
