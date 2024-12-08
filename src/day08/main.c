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
#define MARKER_FREQUENCY  (1U << 0U)
#define MARKER_PART1      (1U << 1U)
#define MARKER_PART2      (1U << 2U)

typedef struct
{
    int x;
    int y;
} coord_t;

static int
place_antinode(grid2d_t* g, int y, int x, size_t marker)
{
    if (   (y >= 0)
        && (y < (int)g->max_y)
        && (x >= 0)
        && (x < (int)g->max_x))
    {
        g->flags[y][x] |= marker;
        return 1;
    }
    return 0;
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
                g->flags[y][x] |= MARKER_FREQUENCY;
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
            place_antinode(g, coords[i].y + dist_y, coords[i].x + dist_x, MARKER_PART1);
            place_antinode(g, coords[j].y - dist_y, coords[j].x - dist_x, MARKER_PART1);
            int yy = coords[i].y;
            int xx = coords[i].x;
            g->flags[yy][xx] |= MARKER_PART2;
            do
            {
                yy = yy + dist_y;
                xx = xx + dist_x;
            }
            while (place_antinode(g, yy, xx, MARKER_PART2));
            yy = coords[j].y;
            xx = coords[j].x;
            g->flags[yy][xx] |= MARKER_PART2;
            do
            {
                yy = yy - dist_y;
                xx = xx - dist_x;
            }
            while (place_antinode(g, yy, xx, MARKER_PART2));
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
            if (   (g->grid[y][x] != '.')
                && ((g->flags[y][x] & MARKER_FREQUENCY) == 0U))
                handle_frequency(g, y, x);

    int result_p1 = 0;
    int result_p2 = 0;
    for (size_t y = 0U; y < g->max_y; y++)
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if ((g->flags[y][x] & MARKER_PART1) != 0U)
                result_p1++;
            if ((g->flags[y][x] & MARKER_PART2) != 0U)
                result_p2++;
        }

    printf("Part 1: Number of unique antinodes = %d\n", result_p1);
    printf("Part 2: Number of unique antinodes = %d\n", result_p2);
    grid2d_free(g);

    return EXIT_SUCCESS;
}
