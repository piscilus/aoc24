/*
 * Main program for advent of code 2024 day 25.
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

#define WIDTH  (5)
#define HEIGHT (7)

typedef struct
{
    unsigned int bitting[WIDTH];
} keylock_t;

int
main(int argc, char** argv)
{
    int ret = EXIT_FAILURE;

    printf("Advent of Code 2024 - 25: Code Chronicle\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));
    if (f == NULL)
    {
        fprintf(stderr, "Input corrupted\n");
        goto ep2;
    }

    keylock_t* keys = NULL;
    keylock_t* locks = NULL;
    int num_keys = 0;
    int num_locks = 0;

    keylock_t* current;

    int i = 0;
    const char* p;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        if (strlen(p) == 0U)
        {
            i = 0;
            continue;
        }
        if (i == 0) /* new lock or key */
        {
            if (p[0] == '#') /* lock */
            {
                locks = realloc(locks, (num_locks + 1) * sizeof(keylock_t));
                if (locks == NULL)
                {
                    fprintf(stderr, "Buy more RAM\n");
                    goto ep;
                }
                current = &locks[num_locks];
                memset(current, 0, sizeof(keylock_t));
                num_locks++;
            }
            else /* '.'  = key */
            {
                keys = realloc(keys, (num_keys + 1) * sizeof(keylock_t));
                if (keys == NULL)
                {
                    fprintf(stderr, "Buy more RAM\n");
                    goto ep;
                }
                current = &keys[num_keys];
                memset(current, 0, sizeof(keylock_t));
                num_keys++;
            }
        }
        else
        {
            assert(strlen(p) == WIDTH);
            for (size_t j = 0U; j < WIDTH; j++)
            if (p[j] == '#')
                current->bitting[j]++;
        }
        i++;
    }

    int count = 0;
    for (int n = 0; n < num_keys; n++)
    {
        for (int m = 0; m < num_locks; m++)
        {
            int valid = 0;
            for (int k = 0; k < WIDTH; k++)
                if (locks[m].bitting[k] + keys[n].bitting[k] <= HEIGHT - 1)
                    valid++;
            if (valid == WIDTH)
                count++;
        }
    }

    printf("Part 1: %d\n", count);

    ret = EXIT_SUCCESS;

    ep:

    if (keys != NULL)
        free(keys);
    if (locks != NULL)
        free(locks);
    flr_free(f);

    ep2:

    return ret;
}
