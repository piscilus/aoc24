/*
 * Main program for advent of code 2024 day 1.
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
#include <stdlib.h>
#include <string.h>

static int
compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

static int*
find_first(int* arr, size_t n, int key)
{
    size_t low = 0U;
    size_t high = n;
    int* result = NULL;

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;
        if (arr[mid] < key)
        {
            low = mid + 1U;
        }
        else if (arr[mid] > key)
        {
            high = mid;
        }
        else
        {
            result = &arr[mid];
            high = mid;
        }
    }

    return result;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 1: Historian Hysteria\n\n");

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

    int* left = malloc(num_locations * sizeof(int));
    if (left == NULL)
    {
        fprintf(stderr, "Buy more RAM!\n");
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    int* right = malloc(num_locations * sizeof(int));
    if (right == NULL)
    {
        fprintf(stderr, "Buy more RAM!\n");
        free(left);
        flr_free(f);
        exit(EXIT_FAILURE);
    }

    const char* p;
    size_t i = 0U;
    while((p = flr_get_next_line(f, NULL)) != NULL)
    {
        if (sscanf(p, "%d %d", left + i, right + i) != 2)
        {
            fprintf(stderr, "Error in the data, line %zu: '%s'\n", i, p);
            free(left);
            free(right);
            flr_free(f);
            exit(EXIT_FAILURE);
        }
        i++;
    }

    flr_free(f);

    /* Part 1 */

    qsort(left, num_locations, sizeof(int), compare_ints);
    qsort(right, num_locations, sizeof(int), compare_ints);

    int total_distance = 0;
    for (size_t i = 0U; i < num_locations; i++)
    {
        int diff;
        diff = (left[i] > right[i]) ? left[i] - right[i] : right[i] - left[i];
        total_distance += diff;
    }
    printf("Part 1: Total distance = %d\n", total_distance);

    /* Part 2 */

    int similarity_score = 0;
    i = 0U;
    while (i < num_locations)
    {
        int current = left[i];
        int result = 0;
        int* v = find_first(right, num_locations, current);
        if (v != NULL)
        {
            int count = 0;
            size_t ci = v - right;
            while ((ci < num_locations) && (current == right[ci]))
            {
                count++;
                ci++;
            }
            result += current * count;
            while ((i < num_locations) && (current == left[i]))
            {
                similarity_score += result;
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    printf("Part 2: Similarity score = %d\n", similarity_score);

    free(left);
    free(right);

    return EXIT_SUCCESS;
}
