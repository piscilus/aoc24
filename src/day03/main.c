/*
 * Main program for advent of code 2024 day 3.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 3: Mull It Over\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    if (fseek(fp, 0, SEEK_END) != 0)
        exit(EXIT_FAILURE);

    long fs;
    if ((fs = ftell(fp)) < 0)
        exit(EXIT_FAILURE);

    size_t file_size = (size_t)fs;
    if (fseek(fp, 0, SEEK_SET) != 0)
        exit(EXIT_FAILURE);

    char* buffer = malloc(file_size + 1U);
    if (buffer == NULL)
        exit(EXIT_FAILURE);

    size_t read_size = fread(buffer, 1U, file_size, fp);
    buffer[read_size] = '\0';

    fclose(fp);

    long result_p1 = 0;
    long result_p2 = 0;
    int ena = 1;
    for (size_t i = 0U; i < file_size; i++)
    {
        if (0 == strncmp(buffer + i, "mul(", 4U))
        {
            int a = 0, b = 0;
            char end;

            if (   ((i + 4U) < file_size)
                && (3 == sscanf(buffer + i + 3,"(%d,%d%c", &a, &b, &end))
                && (end == ')'))
            {
                int mul = a * b;
                result_p1 += mul;
                if (ena)
                    result_p2 += mul;
            }
        }
        else if (0 == strncmp(buffer + i, "do()", 4U))
            ena = 1;
        else if (0 == strncmp(buffer + i, "don't()", 7U))
            ena = 0;
    }

    free(buffer);

    printf("Part 1: result = %ld\n", result_p1);
    printf("Part 2: result = %ld\n", result_p2);

    return EXIT_SUCCESS;
}
