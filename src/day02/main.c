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
validate(int numbers[], size_t count)
{
    int check = 1;
    int dir = 0;
    for (size_t i = 1U; i < count; i++)
    {
        if (   (numbers[i - 1U] == numbers[i])
            || (abs(numbers[i - 1U] - numbers[i]) > 3))
        {
            check = 0;
            break;
        }
        if (dir == 0)
        {
            if (numbers[i - 1U] > numbers[i])
                dir = -1;
            else
                dir = 1;
        }
        else
        {
            if (dir == -1 && numbers[i - 1U] < numbers[i])
            {
                check = 0;
                break;
            }
            else if (dir == 1 && numbers[i - 1U] > numbers[i])
            {
                check = 0;
                break;
            }
        }
    }
    return check;
}

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

    int result_p1 = 0;
    int result_p2 = 0;
    const char* p;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        size_t count = 0U;
        int* numbers = malloc(strlen(p) * sizeof(int));
        if (numbers == NULL)
            break;
        char* line = strdup(p);
        assert(line != NULL);
        char *token = strtok(line, " ");
        while(token)
        {
            numbers[count] = atoi(token);
            token = strtok(NULL, " ");
            count++;
        }
        assert(count > 1U);
        if (validate(numbers, count))
        {
            result_p1++;
        }
        else
        {
            int* numbers_reduced = malloc((count - 1U) * sizeof(int));
            assert(numbers_reduced != NULL);
            for (size_t j = 0U; j < count; j++)
            {
                int n = 0;
                for (size_t i = 0U; i < count; i++)
                {
                    if (i == j)
                        continue;
                    else
                        numbers_reduced[n++] = numbers[i];
                }
                if (validate(numbers_reduced, count - 1U))
                {
                    result_p2++;
                    break;
                }
            }
            free(numbers_reduced);
        }
        free(line);
        free(numbers);
    }

    printf("Part 1: Number of safe reports = %d\n", result_p1);
    printf("Part 2: Number of safe reports = %d\n", result_p1 + result_p2);

    flr_free(f);

    return EXIT_SUCCESS;
}
