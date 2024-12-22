/*
 * Main program for advent of code 2024 day 22.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_FLR_IMPLEMENTATION
#define PLIB_FLR_NO_NAMESPACE
#include "plib_flr.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MASK       ((1U << 24U) - 1U)
#define ITERATIONS (2000)

static unsigned long
secret_algorithm(unsigned long secret)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        secret ^= secret << 6U;           /* *64 */
        secret ^= (secret & MASK) >> 5U;  /* /32 */
        secret ^= (secret & MASK) << 11U; /* *2048 */
        secret &= MASK;
    }

    return secret;
}

int
main(int argc, char** argv)
{
    int ret = EXIT_FAILURE;

    printf("Advent of Code 2024 - 22: Monkey Market\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));

    unsigned long result = 0U;
    const char* p;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        errno = 0;
        unsigned long v = strtoul(p, NULL, 10);
        if (errno == 0)
        {
            result += secret_algorithm(v);
        }
        else
        {
            fprintf(stderr, "Unexpected input: '%s'\n", p);
            break;
        }
    }

    printf("Part 1: %ld\n", result);

    flr_free(f);

    return ret;
}
