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

    for (size_t t = 0U; t < SIMULATION_TIME; t++)
    {
        for (size_t r = 0U; r < num_robots; r++)
        {
            robots[r].pos.x = wrap(robots[r].pos.x + robots[r].vel.vx, BATHROOM_X);
            robots[r].pos.y = wrap(robots[r].pos.y + robots[r].vel.vy, BATHROOM_Y);
        }
    }

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

    free(robots);

    return EXIT_SUCCESS;
}
