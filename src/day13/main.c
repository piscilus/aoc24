/*
 * Main program for advent of code 2024 day 13.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_FLR_IMPLEMENTATION
#define PLIB_FLR_NO_NAMESPACE
#include "plib_flr.h"

#include <stdio.h>

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 13: Claw Contraption\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));

    size_t i = 0U;
    long x[3];
    long y[3];
    long result = 0;
    const char* p;
    while((p = flr_get_next_line(f, NULL)) != NULL)
    {
        if (strlen(p) > 0U)
        {
            char c;
            if (i < 2U)
            {
                if (sscanf(p, "Button %c: X+%ld, Y+%ld", &c, &x[i], &y[i]) != 3)
                {
                    fprintf(stderr, "Unexpected input\n");
                    break;
                }
                (void)c;
            }
            else if (i == 2U)
            {
                if (sscanf(p, "Prize: X=%ld, Y=%ld", &x[i], &y[i]) != 2)
                {
                    fprintf(stderr, "Unexpected input\n");
                    break;
                }
                /* solve using elimination */
                long m = x[2] * y[0] - y[2] * x[0];
                long n = x[1] * y[0] - y[1] * x[0];
                if ((m % n) == 0)
                {
                    long b = m / n;
                    long k = x[2] - b * x[1];
                    if ((k % x[0]) == 0)
                        result += k / x[0] * 3 + b;
                }
            }
            else
            {
                fprintf(stderr, "Unexpected input\n");
                break;
            }
            i++;
        }
        else
        {
            i = 0U;
        }
    }

    printf("Part 1: fewest tokens = %ld\n", result);

    flr_free(f);

    return EXIT_SUCCESS;
}
