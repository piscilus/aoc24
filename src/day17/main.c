/*
 * Main program for advent of code 2024 day 17.
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
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    REG_A = 0,
    REG_B,
    REG_C,
    NUM_REGS
};

enum
{
    INST_ADV = 0,
    INST_BXL,
    INST_BST,
    INST_JNZ,
    INST_BXC,
    INST_OUT,
    INST_BDV,
    INST_CDV
};

static unsigned int registers[NUM_REGS] = {0};

static unsigned int
get_operand(unsigned int operand)
{
    switch (operand)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4:
            return registers[REG_A];
        case 5:
            return registers[REG_B];
        case 6:
            return registers[REG_C];
        default:
            assert(0);
            return 0;
    }
}

int
main(int argc, char** argv)
{
    int ret = EXIT_FAILURE;

    printf("Advent of Code 2024 - 17: Chronospatial Computer\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));
    const char* p;

    int i = 0;
    while (((p = flr_get_next_line(f, NULL)) != NULL) && (strlen(p) > 0U))
    {
        if (i >= NUM_REGS)
        {
            fprintf(stderr, "Invalid format: too many registers\n");
            goto ep;
        }
        char r;
        if (sscanf(p, "Register %c: %u", &r, &registers[i]) != 2)
        {
            fprintf(stderr, "Failed to parse line %d: '%s'\n", i, p);
            goto ep;
        }
        i++;
    }

    if ((p = flr_get_next_line(f, NULL)) == NULL)
    {
        fprintf(stderr, "Unexpected end of input\n");
        goto ep;
    }

    const char* prefix = "Program: ";
    size_t prefix_len = strlen(prefix);
    if (strncmp(p, prefix, prefix_len) != 0)
    {
        fprintf(stderr, "Expected program section but got '%s'\n", p);
        goto ep;
    }

    const char* prog = p + prefix_len;
    size_t prog_len = strlen(prog);

    char* parsebuf;
    if ((parsebuf = malloc(sizeof(char) * prog_len)) == NULL)
    {
        fprintf(stderr, "Buy more RAM\n");
        goto ep;
    }
    unsigned int* program;
    if ((program = malloc(sizeof(unsigned int) * prog_len)) == NULL)
    {
        fprintf(stderr, "Buy more RAM\n");
        goto ep2;
    }

    strncpy(parsebuf, prog, prog_len);
    parsebuf[prog_len] = '\0';

    size_t program_count = 0U;
    char* token = strtok(parsebuf, ",");
    while (token != NULL)
    {
        unsigned long num = strtoul(token, NULL, 10);
        if (num > UINT_MAX)
        {
            fprintf(stderr, "Number out of range\n");
            goto ep3;
        }
        program[program_count++] = (unsigned int)num;
        token = strtok(NULL, ",");
    }

    printf("Part 1: program output = ");
    size_t ip = 0U; /* instruction pointer */
    size_t oc = 0U; /* output counter */
    while (ip < program_count - 1U)
    {
        unsigned int op = program[ip + 1U]; /* operand */
        switch (program[ip]) /* opcode */
        {
            case INST_ADV:
                assert(get_operand(op) < 32);
                registers[REG_A] /= (1U << get_operand(op));
                break;
            case INST_BXL:
                registers[REG_B] ^= op;
                break;
            case INST_BST:
                registers[REG_B] = (get_operand(op) % 8U) & 0x7U;
                break;
            case INST_JNZ:
                if (registers[REG_A] != 0U)
                {
                    ip = op;
                    continue;
                }
                break;
            case INST_BXC:
                registers[REG_B] ^= registers[REG_C];
                break;
            case INST_OUT:
                if (oc > 0U)
                    putchar(',');
                printf("%u", get_operand(op) % 8U);
                oc++;
                break;
            case INST_BDV:
                assert(get_operand(op) < 32);
                registers[REG_B] = registers[REG_A] / (1U << get_operand(op));
                break;
            case INST_CDV:
                assert(get_operand(op) < 32);
                registers[REG_C] = registers[REG_A] / (1U << get_operand(op));
                break;
            default:
                assert(0);
                break;
        }
        ip += 2U;
    }
    putchar('\n');
    ret = EXIT_SUCCESS;

    ep3:
    free(program);

    ep2:
    free(parsebuf);

    ep:
    flr_free(f);

    return ret;
}
