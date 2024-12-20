/*
 * Main program for advent of code 2024 day 15.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_FLR_IMPLEMENTATION
#define PLIB_FLR_NO_NAMESPACE
#include "plib_flr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    size_t x;
    size_t y;
} coords_t;

char** grid;
coords_t max = {0};

static int
move_box(char dir, coords_t* pos, size_t moves)
{
    switch (dir)
    {
        case '<':
            if (grid[pos->y][pos->x - 1U - moves] == '.')
            {
                grid[pos->y][pos->x - 1U - moves] = 'O';
                return 1;
            }
            else if (grid[pos->y][pos->x - 1U - moves] == 'O')
                return move_box(dir, pos, moves + 1U);
            else
                return 0U;
            break;
        case '^':
            if (grid[pos->y - 1U - moves][pos->x] == '.')
            {
                grid[pos->y - 1U - moves][pos->x] = 'O';
                return 1;
            }
            else if (grid[pos->y - 1U - moves][pos->x] == 'O')
                return move_box(dir, pos, moves + 1U);
            else
                return 0U;
            break;
        case '>':
            if (grid[pos->y][pos->x + 1U + moves] == '.')
            {
                grid[pos->y][pos->x + 1U + moves] = 'O';
                return 1;
            }
            else if (pos->x + 1U + moves < max.x && grid[pos->y][pos->x + 1U + moves] == 'O')
                return move_box(dir, pos, moves + 1U);
            else
                return 0U;
            break;
        case 'v':
            if (grid[pos->y + 1U + moves][pos->x] == '.')
            {
                grid[pos->y + 1U + moves][pos->x] = 'O';
                return 1;
            }
            else if (pos->y + 1U + moves < max.y && grid[pos->y + 1U + moves][pos->x] == 'O')
                return move_box(dir, pos, moves + 1U);
            else
                return 0U;
            break;
        default:
            assert(0);
            break;
    }
}

static void
move(char dir, coords_t* pos)
{
    assert(pos->x > 0U);
    assert(pos->y > 0U);
    assert(pos->x < max.x - 1U);
    assert(pos->y < max.y - 1U);
    switch (dir)
    {
        case '<':
            if (grid[pos->y][pos->x - 1U] == '.')
                pos->x -= 1U;
            else if (grid[pos->y][pos->x - 1U] == 'O' && move_box(dir, pos, 1U))
            {
                grid[pos->y][pos->x - 1U] = '.';
                pos->x -= 1U;
            }
            break;
        case '^':
            if (grid[pos->y - 1U][pos->x] == '.')
                pos->y -= 1U;
            else if (grid[pos->y - 1U][pos->x] == 'O' && move_box(dir, pos, 1U))
            {
                grid[pos->y - 1U][pos->x] = '.';
                pos->y -= 1U;
            }
            break;
        case '>':
            if (grid[pos->y][pos->x + 1U] == '.')
                pos->x += 1U;
            else if (grid[pos->y][pos->x + 1U] == 'O' && move_box(dir, pos, 1U))
            {
                grid[pos->y][pos->x + 1U] = '.';
                pos->x += 1U;
            }
            break;
        case 'v':
            if (grid[pos->y + 1U][pos->x] == '.')
                pos->y += 1U;
            else if (grid[pos->y + 1U][pos->x] == 'O' && move_box(dir, pos, 1U))
            {
                grid[pos->y + 1U][pos->x] = '.';
                pos->y += 1U;
            }
            break;
        default:
            assert(0);
            break;
    }
}

#if 0
static void
print_grid(coords_t* pos)
{
    for (size_t y = 0U; y < max.y; y++)
    {
        for (size_t x = 0U; x < max.x; x++)
        {
            if (x == pos->x && y == pos->y)
                putchar('@');
            else
                putchar(grid[y][x]);
        }
        putchar('\n');
    }
}
#endif

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 15: Warehouse Woes\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));
    const char* p;
    size_t i = 0U;
    size_t max_x;
    while (     ((p = flr_get_next_line(f, NULL)) != NULL)
             && ((max_x = strlen(p)) > 0U))
    {
        max.y++;
        max.x = max_x;
    }

    grid = malloc(max.y * sizeof(char*));
    if (grid == NULL)
    {
        free(grid);
        exit(EXIT_FAILURE);
    }

    for (size_t y = 0U; y < max.y; y++)
    {
        grid[y] = malloc(max.x * sizeof(char));
        if (grid[y] == NULL)
        {
            free(grid);
            exit(EXIT_FAILURE);
        }
    }

    flr_reset(f);

    while (    ((p = flr_get_next_line(f, &i)) != NULL)
            && (strlen(p) > 0U))
    {
        assert(i > 0U);
        memcpy(grid[i - 1U], p, max.x);
    }

    coords_t pos = {0};
    for (size_t y = 0U; y < max.y; y++)
    {
        for (size_t x = 0U; x < max.x; x++)
        {
            if (grid[y][x] == '@')
            {
                pos.x = x;
                pos.y = y;
                grid[y][x] = '.';
            }
        }
    }

    assert((pos.x > 0U) && (pos.y > 0U));

    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        const char* c = p;
        while (0 != *c)
        {
            move(*c, &pos);
            c++;
        }
    }

    size_t result = 0U;
    for (size_t y = 0U; y < max.y; y++)
        for (size_t x = 0U; x < max.x; x++)
            if (grid[y][x] == 'O')
                result += 100U * y + x;
    printf("Part 1: Sum of GPS coordinates = %zu\n", result);

    for (size_t y = 0U; y < max.y; y++)
        free(grid[y]);
    free(grid);

    flr_free(f);

    return EXIT_SUCCESS;
}
