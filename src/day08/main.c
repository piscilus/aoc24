/*
 * Main program for advent of code 2024 day 8.
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUGP(...) /*printf(__VA_ARGS__)*/
#define NUM_PER_FREQUENCY (10)

typedef struct
{
    int x;
    int y;
} coord_t;

static void
place_antinode(grid2d_t* g, int y, int x)
{
    if (   (y >= 0)
        && (y < (int)g->max_y)
        && (x >= 0)
        && (x < (int)g->max_x))
    {
        g->flags[y][x] |= 2U;
    }
}

static void
handle_frequency(grid2d_t* g, size_t sy, size_t sx)
{
    coord_t coords[NUM_PER_FREQUENCY] = {0};
    int count = 0;
    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if (g->grid[y][x] == g->grid[sy][sx])
            {
                g->flags[y][x] |= 1U;
                assert(count < NUM_PER_FREQUENCY);
                coords[count].y = (int)y;
                coords[count].x = (int)x;
                count++;
            }
        }
    }
    assert(count > 0); /* increase NUM_PER_FREQUENCY */
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            int dist_y = (coords[i].y - coords[j].y);
            int dist_x = (coords[i].x - coords[j].x);
            place_antinode(g, coords[i].y + dist_y, coords[i].x + dist_x);
            place_antinode(g, coords[j].y - dist_y, coords[j].x - dist_x);
        }
    }
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 8: Resonant Collinearity\n\n");

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

    for (size_t y = 0U; y < g->max_y; y++)
        for (size_t x = 0U; x < g->max_x; x++)
            if ((g->grid[y][x] != '.') && ((g->flags[y][x] & 1U) == 0U))
                handle_frequency(g, y, x);

    int result = 0;
    for (size_t y = 0U; y < g->max_y; y++)
        for (size_t x = 0U; x < g->max_x; x++)
            if ((g->flags[y][x] & 2) != 0)
                result++;
    printf("Part 1: Number of unique antinodes = %d\n", result);
    grid2d_free(g);

    return EXIT_SUCCESS;
}