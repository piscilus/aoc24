/*
 * Main program for advent of code 2024 day 7.
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 7: Bridge Repair\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));
    if (flr_num_lines(f) == 0U)
    {
        fprintf(stderr, "File empty?!\n");
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    long long total_calibration_result = 0;
    const char* p;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        size_t count = 0U;
        long long* numbers = malloc(strlen(p) * sizeof(long long));
        if (numbers == NULL)
            break;
        char* line = strdup(p);
        assert(line != NULL);
        char *token = strtok(line, " ");
        while(token)
        {
            numbers[count] = atoll(token);
            token = strtok(NULL, " ");
            count++;
        }
        assert(count > 2U);
        size_t num_operations = count - 2U;
        size_t combinations = pow(2, num_operations);
        for (size_t i = 0U; i < combinations; i++)
        {
            long long result = numbers[1];
            for (size_t j = 0U; j < num_operations; j++)
            {
                if (((i >> (num_operations - j - 1U)) & 1U) == 0U)
                    result += numbers[j + 2];
                else
                    result *= numbers[j + 2];
            }
            if (result == numbers[0])
            {
                total_calibration_result += result;
                break;
            }
        }
        free(line);
        free(numbers);
    }

    printf("Part 1: %lld\n", total_calibration_result);
    flr_free(f);

    return EXIT_SUCCESS;
}
