/*
 * Main program for advent of code 2024 day 4c.
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
count_horizontal(grid2d_t* g, const char* p)
{
    int c = 0;
    size_t p_size = strlen(p);

    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x - p_size + 1; x++)
        {
            size_t cfwd = 0U;
            size_t cbwd = 0U;
            for (size_t i = 0U; i < p_size; i++)
            {
                if (g->grid[y][x + i] == p[p_size - i - 1U])
                    cfwd++;
                if (g->grid[y][x + i] == p[i])
                    cbwd++;
            }
            if (cfwd == p_size)
                c++;
            if (cbwd == p_size)
                c++;
        }
    }

    return c;
}

static int
count_vertical(grid2d_t* g, const char* p)
{
    int c = 0;
    size_t p_size = strlen(p);

    for (size_t x = 0U; x < g->max_x; x++)
    {
        for (size_t y = 0U; y < g->max_y - p_size + 1U; y++)
        {
            size_t cfwd = 0U;
            size_t cbwd = 0U;
            for (size_t i = 0U; i < p_size; i++)
            {
                if (g->grid[y + i][x] == p[p_size - i - 1U])
                    cfwd++;
                if (g->grid[y + i][x] == p[i])
                    cbwd++;
            }
            if (cfwd == p_size)
                c++;
            if (cbwd == p_size)
                c++;
        }
    }

    return c;
}

static int
count_diagonal_backslash(grid2d_t* g, const char* p)
{
    int c = 0;
    size_t p_size = strlen(p);

    for (size_t y = 0U; y < g->max_y - p_size + 1U; y++)
    {
        for (size_t x = 0U; x < g->max_x - p_size + 1U; x++)
        {
            size_t cfwd = 0U;
            size_t cbwd = 0U;
            for (size_t i = 0; i < p_size; i++)
            {
                if (g->grid[y + i][x + i] == p[p_size - i - 1U])
                    cfwd++;
                if (g->grid[y + i][x + i] == p[i])
                    cbwd++;
            }
            if (cfwd == p_size)
                c++;
            if (cbwd == p_size)
                c++;
        }
    }

    return c;
}

static int
count_diagonal_slash(grid2d_t* g, const char* p)
{
    int c = 0;
    size_t p_size = strlen(p);

    for (size_t y = 0U; y < g->max_y - p_size + 1U; y++)
    {
        for (size_t x = p_size - 1U; x < g->max_x; x++)
        {
            size_t cfwd = 0U;
            size_t cbwd = 0U;
            for (size_t i = 0U; i < p_size; i++)
            {
                if (g->grid[y + i][x - i] == p[p_size - i - 1])
                    cfwd++;
                if (g->grid[y + i][x - i] == p[i])
                    cbwd++;
            }
            if (cfwd == p_size)
                c++;
            if (cbwd == p_size)
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

    const char* p = "XMAS";
    int c = count_horizontal(g, p);        /* - */
    c += count_vertical(g, p);             /* | */
    c += count_diagonal_backslash(g, p);   /* \ */
    c += count_diagonal_slash(g, p);       /* / */

    printf("Part 1: XMAS count = %d\n", c);

    grid2d_free(g);

    return EXIT_SUCCESS;
}
