/*
 * Main program for advent of code 2024 day 6.
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
#include <stdio.h>
#include <stdlib.h>

#define DIR_UP    ('^')
#define DIR_DOWN  ('v')
#define DIR_LEFT  ('<')
#define DIR_RIGHT ('>')
#define OBSTACLE  ('#')

typedef struct
{
    size_t x;
    size_t y;
} coord_t;

typedef struct
{
    coord_t pos;
    char dir;
} guard_t;

static const char dir_chars[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

int
determine_start(grid2d_t* grid, guard_t* guard)
{
    for (size_t y = 0U; y < grid->max_y; y++)
    {
        for (size_t x = 0U; x < grid->max_x; x++)
        {
            for (size_t i = 0U; i < (sizeof(dir_chars)/sizeof(dir_chars[0])); i++)
            {
                if (grid->grid[y][x] == dir_chars[i])
                {
                    guard->pos.y = y;
                    guard->pos.x = x;
                    guard->dir = dir_chars[i];
                    return 1;
                }
            }
        }
    }
    return 0;
}

static void
turn(guard_t* guard)
{
    switch (guard->dir)
    {
        case DIR_UP:
            guard->dir = DIR_RIGHT;
            break;
        case DIR_DOWN:
            guard->dir = DIR_LEFT;
            break;
        case DIR_LEFT:
            guard->dir = DIR_UP;
            break;
        case DIR_RIGHT:
            guard->dir = DIR_DOWN;
            break;
        default:
            assert(0);
            break;
    }
}

static int
walk(grid2d_t* grid, guard_t* guard)
{
    grid->flags[guard->pos.y][guard->pos.x] = 1U;
    int walking = 1;
    while (walking)
    {
        switch (guard->dir)
        {
            case DIR_UP:
                if (guard->pos.y == 0U)
                    walking = 0;
                else
                    if (grid->grid[guard->pos.y - 1][guard->pos.x] == OBSTACLE)
                        turn(guard);
                    else
                        guard->pos.y -= 1U;
                break;
            case DIR_DOWN:
                if (guard->pos.y == grid->max_y - 1U)
                    walking = 0;
                else
                    if (grid->grid[guard->pos.y + 1U][guard->pos.x] == OBSTACLE)
                        turn(guard);
                    else
                        guard->pos.y += 1U;
                break;
            case DIR_LEFT:
                if (guard->pos.x == 0U)
                    walking = 0;
                else
                    if (grid->grid[guard->pos.y][guard->pos.x - 1U] == OBSTACLE)
                        turn(guard);
                    else
                        guard->pos.x -= 1U;
                break;
            case DIR_RIGHT:
                if (guard->pos.x == grid->max_x - 1U)
                    walking = 0;
                else
                    if (grid->grid[guard->pos.y][guard->pos.x + 1U] == OBSTACLE)
                        turn(guard);
                    else
                        guard->pos.x += 1U;
                break;
            default:
                assert(0);
                break;
        }
        grid->flags[guard->pos.y][guard->pos.x] = 1U;
    }

    int count = 0;
    for (size_t y = 0U; y < grid->max_y; y++)
        for (size_t x = 0U; x < grid->max_x; x++)
            if (grid->flags[y][x] > 0U)
                count++;

    return count;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 6: Guard Gallivant\n\n");

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

#if 0
    grid2d_print(g);
    printf("maxy %zu maxx %zu\n", g->max_y, g->max_x);
#endif

    guard_t guard = {0};
    int r = determine_start(g, &guard);
    assert(r);

    r = walk(g, &guard);
    printf("Part 1: %d\n", r);

    grid2d_free(g);

    return EXIT_SUCCESS;
}
