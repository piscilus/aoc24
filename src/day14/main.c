/*
 * Main program for advent of code 2024 day 14.
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

#define BATHROOM_X (101)
#define BATHROOM_Y (103)
#define PIXEL_DENSITY_WINDOW (5)
#define SIMULATION_TIME (100U)

typedef struct
{
    struct
    {
        int x;
        int y;
    } pos;
    struct
    {
        int vx;
        int vy;
    } vel;
} robot_t;

static int
wrap(int value, int max_value)
{
    int range = max_value;
    int result = value % range;
    if (result < 0)
        result += max_value;
    return result;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 14: Restroom Redoubt\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));

    size_t num_robots = flr_num_lines(f);
    if (num_robots == 0U)
        exit(EXIT_FAILURE);

    robot_t* robots = malloc(num_robots * sizeof(robot_t));
    if (robots == NULL)
    {
        fprintf(stderr, "Buy more RAM!\n");
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    const char* p;
    size_t i = 0U;
    while((p = flr_get_next_line(f, NULL)) != NULL)
    {
        if (sscanf( p,
                    "p=%d,%d v=%d,%d",
                    &robots[i].pos.x,
                    &robots[i].pos.y,
                    &robots[i].vel.vx,
                    &robots[i].vel.vy) != 4)
        {
            free(robots);
            flr_free(f);
            exit(EXIT_FAILURE);
        }
        i++;
    }
    flr_free(f);

    assert(num_robots == i);

    int** grid = calloc(BATHROOM_Y, sizeof(int*));
    if (grid == NULL)
        exit(EXIT_FAILURE);
    for (size_t y = 0U; y < BATHROOM_Y; y++)
    {
        grid[y] = calloc(BATHROOM_X, sizeof(int));
        if (grid[y] == NULL)
            exit(EXIT_FAILURE);
    }

    size_t tiles = BATHROOM_Y * BATHROOM_X;
    printf("Calculations for part 2:\n", num_robots);
    printf(" Num robots                    = %zu\n", num_robots);
    printf(" Num tiles                     = %zu\n", tiles);
    printf(" Num empty tiles               = %zu\n", tiles - num_robots);
    size_t num_regions = tiles / PIXEL_DENSITY_WINDOW / PIXEL_DENSITY_WINDOW;
    printf(" Num density regions (%dx%x)     = %zu\n", PIXEL_DENSITY_WINDOW, PIXEL_DENSITY_WINDOW, num_regions);
    size_t percent_robots = (num_robots * 100 / tiles) + 1;
    printf(" Percentage of tiles allocated = %zu %%\n", percent_robots);
    size_t regions_almost_free = num_regions - (num_regions * percent_robots * 2 / 100);
    printf(" Num regions almost free       = %zu\n\n", regions_almost_free);

    size_t time = 0U;
    int run = 1;
    while (run)
    {
        for (size_t y = 0U; y < BATHROOM_Y; y++)
            for (size_t x = 0U; x < BATHROOM_X; x++)
                grid[y][x] = 0;
        for (size_t r = 0U; r < num_robots; r++)
        {
            robots[r].pos.x = wrap(robots[r].pos.x + robots[r].vel.vx, BATHROOM_X);
            robots[r].pos.y = wrap(robots[r].pos.y + robots[r].vel.vy, BATHROOM_Y);
            grid[robots[r].pos.y][robots[r].pos.x]++;
        }
        time++;
        if (time == SIMULATION_TIME)
        {
            long q[4] = {0};
            for (size_t r = 0U; r < num_robots; r++)
            {
                if (robots[r].pos.x < BATHROOM_X / 2)
                {
                    if (robots[r].pos.y < BATHROOM_Y / 2)
                        q[0]++;
                    else if (robots[r].pos.y > BATHROOM_Y / 2)
                        q[1]++;
                }
                else if (robots[r].pos.x > BATHROOM_X / 2)
                {
                    if (robots[r].pos.y < BATHROOM_Y / 2)
                        q[2]++;
                    else if (robots[r].pos.y > BATHROOM_Y / 2)
                        q[3]++;
                }
            }
            printf("Part 1: Safety factor = %ld\n", q[0] * q[1] * q[2] * q[3]);
        }
        size_t almost_empty = 0U;
        for (size_t y = 0U; y < BATHROOM_Y; y += PIXEL_DENSITY_WINDOW)
        {
            for (size_t x = 0U; x < BATHROOM_X; x += PIXEL_DENSITY_WINDOW)
            {
                int robots_in_region = 0;
                for (size_t yy = y; yy < y + PIXEL_DENSITY_WINDOW; yy++)
                    for (size_t xx = x; xx < x + PIXEL_DENSITY_WINDOW; xx++)
                        if (yy < BATHROOM_Y && xx < BATHROOM_X)
                            robots_in_region += grid[yy][xx];
                if (robots_in_region < 2) /* region max one robot */
                    almost_empty++;
            }
        }
        if (almost_empty > regions_almost_free)
        {
            printf("Part 2: Tree after = %zu seconds\n", time);
#if 0
            for (size_t y = 0U; y < BATHROOM_Y; y++)
            {
                for (size_t x = 0U; x < BATHROOM_X; x++)
                {
                    printf("%d", grid[y][x]);
                }
                putchar('\n');
            }
            putchar('\n');
#endif
            run = 0;
        }
    }

    for (size_t y = 0U; y < BATHROOM_Y; y++)
        free(grid[y]);
    free(grid);
    free(robots);

    return EXIT_SUCCESS;
}
