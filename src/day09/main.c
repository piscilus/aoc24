/*
 * Main program for advent of code 2024 day 9.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - Day 9: Disk Fragmenter\n\n");

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
    if ((fs % 2) != 0)
        file_size++;

    char* buffer = malloc(file_size);
    if (buffer == NULL)
        exit(EXIT_FAILURE);

    size_t read_size = fread(buffer, 1U, file_size, fp);
    if ((fs % 2) != 0)
        buffer[read_size] = '0';

    fclose(fp);

    /* determine disk size */
    size_t disksize = 0U;
    for (size_t i = 0U; i < read_size; i += 2U)
    {
        assert(buffer[i] >= 0x30 && buffer[i] <= 0x39);
        assert(buffer[i + 1U] >= 0x30 && buffer[i + 1U] <= 0x39);
        disksize += (buffer[i] - 0x30) + (buffer[i + 1U] - 0x30);
    }

    int* disk = malloc(disksize * sizeof(int));
    if (disk == NULL)
    {
        fprintf(stderr, "Buy more RAM!\n");
        exit(EXIT_FAILURE);
    }

    /* build disk */
    size_t diskhead = 0U;
    int id = 0U;
    for (size_t i = 0U; i < read_size; i += 2U)
    {
        for (int j = 0; j < (buffer[i] - 0x30); j++)
        {
            disk[diskhead] = id; /* allocated by file id */
            diskhead++;
        }
        for (int j = 0; j < (buffer[i + 1] - 0x30); j++)
        {
            disk[diskhead] = -1; /* free */
            diskhead++;
        }
        id++;
    }

    /* defragmentation */
    diskhead = 0U;
    for (size_t i = disksize; i-- > 0U; )
    {
        if (disk[i] != -1)
        {
            for (; diskhead < disksize; diskhead++)
            {
                if ((diskhead < i) && (disk[diskhead] == -1))
                {
                    disk[diskhead] = disk[i];
                    disk[i] = -1;
                    break;
                }
            }
        }
    }

    uint64_t checksum = 0U;
    for (size_t i = 0U; i < disksize; i++)
        if (disk[i] != -1)
            checksum += (uint64_t)i * (uint64_t)disk[i];

    free(buffer);
    free(disk);

    printf("Part 1: checksum = %" PRIu64 "\n", checksum);

    return EXIT_SUCCESS;
}
