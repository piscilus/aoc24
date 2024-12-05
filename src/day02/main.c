/*
 * Main program for advent of code 2024 day 2.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_FLR_IMPLEMENTATION
#define PLIB_FLR_NO_NAMESPACE
#include "plib_flr.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 2: Red-Nosed Reports\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));

    size_t num_locations = flr_num_lines(f);
    if (num_locations == 0U)
    {
        fprintf(stderr, "File empty?!\n");
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    const char* p;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        int result = 1;
        int dir = 0;
        int prev = INT_MIN, current;
        char *token = strtok(p, " ");
        while(token)
        {
            current = atoi(token);
            token = strtok(NULL, " ");
            if (prev != INT_MIN)
            {
                if (prev == current || (abs(prev - current) > 3))
                {
                    result = 0;
                    break;
                }
                if (dir == 0)
                {
                    if (prev > current)
                        dir = -1;
                    else
                        dir = 1;
                }
                else
                {
                    if (dir == -1 && prev < current)
                    {
                        result = 0;
                        break;
                    }
                    else if (dir == 1 && prev > current)
                    {
                        result = 0;
                        break;
                    }
                }
            }
            prev = current;
        }
        if (result)
            count++;
    }

    printf("Part 1: Number of safe reports = %d\n", count);

    flr_free(f);

    return EXIT_SUCCESS;
}
