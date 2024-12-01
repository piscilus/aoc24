/*
 * plib_flr - v0.1.0 - File Line Reader.
 *
 * The FLR read contents from a file into memory and allows access to
 * null-terminated lines.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2024 "piscilus" Julian Kraemer
 *
 * SPDX-License-Identifier: MIT
 *
 *
 * Notes
 *
 * Only UNIX end-of-line (line-feed) supported!
 *
 *
 * Version History
 *
 * 0.1.0 (2024-11-11) First public release
 *
 */

#ifndef PLIB_FLR_H
#define PLIB_FLR_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef PBLIB_FLR_STATIC
#define PBLIB_FLR_DEF static
#else
#define PBLIB_FLR_DEF extern
#endif

typedef struct plib_flr plib_flr_t;

/* Initialize and load from a file */
PBLIB_FLR_DEF plib_flr_t*
flr_init(const char* file_path);

/* Get number of lines */
PBLIB_FLR_DEF size_t
plib_flr_num_lines(plib_flr_t* flr);

/* Get next line and its line number (optional) */
PBLIB_FLR_DEF const char*
plib_flr_get_next_line(plib_flr_t* flr, size_t* line);

/* Get specific line */
PBLIB_FLR_DEF const char*
plib_flr_get_line(plib_flr_t* flr, size_t line);

/* Reset iterator to first line */
PBLIB_FLR_DEF void
plib_flr_reset(plib_flr_t* flr);

/* Free memory */
PBLIB_FLR_DEF void
plib_flr_free(plib_flr_t* flr);
#elif

#endif /* PLIB_FLR_H */

#define PLIB_FLR_IMPLEMENTATION
#ifdef PLIB_FLR_IMPLEMENTATION
struct plib_flr
{
    FILE* fp;
    size_t file_size;  /* w/out null terminator */
    char* file_buffer; /* complete file content null terminated */
    char** lines;      /* list of pointers to line beginnings */
    size_t num_lines;
    size_t current_line;
};

static plib_flr_t*
plib_flr_determine_file_size(plib_flr_t* flr)
{
    assert(flr != NULL);
    assert(flr->fp != NULL);

    if (fseek(flr->fp, 0, SEEK_END) != 0)
        return NULL;

    long file_size;
    if ((file_size = ftell(flr->fp)) < 0)
        return NULL;

    if (fseek(flr->fp, 0, SEEK_SET) != 0)
        return NULL;

    flr->file_size = (size_t)file_size;

    return flr;
}

static plib_flr_t*
plib_flr_read_file(plib_flr_t* flr)
{
    assert(flr != NULL);
    assert(flr->fp != NULL);

    if (flr->file_size == 0U)
        return NULL;

    flr->file_buffer = malloc(flr->file_size + 1U);
    if (flr->file_buffer == NULL)
        return NULL;

    if (fread(flr->file_buffer, 1, flr->file_size, flr->fp) != flr->file_size)
    {
        free(flr->file_buffer);
        return NULL;
    }

    flr->file_buffer[flr->file_size] = '\0';

    return flr;
}

static plib_flr_t*
plib_flr_detect_lines(plib_flr_t* flr)
{
    assert(flr != NULL);
    assert(flr->file_buffer != NULL);
    assert(flr->file_size > 0U);

    flr->num_lines = 0U;
    for (size_t i = 0U; i < flr->file_size; i++)
        if (flr->file_buffer[i] == '\n')
            flr->num_lines++;

    if (flr->file_buffer[flr->file_size - 1U] != '\n')
        flr->num_lines++;

    flr->lines = (char**)malloc(flr->num_lines * sizeof(char*));

    if (flr->lines == NULL)
        return NULL;

    char* current_line = flr->file_buffer;
    for (size_t i = 0U; i < flr->num_lines; i++)
    {
        flr->lines[i] = current_line;
        char* next_line = strchr(current_line, '\n');
        if (next_line != NULL)
        {
            *next_line = '\0';
            current_line = next_line + 1;
        }
        else
        {
            break;
        }
    }

    return flr;
}

PBLIB_FLR_DEF plib_flr_t*
plib_flr_init(const char* file_path)
{
    if (file_path == NULL)
        return NULL;

    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        perror("Could not open file");
        return NULL;
    }

    plib_flr_t* flr = malloc(sizeof(plib_flr_t));
    if (flr == NULL)
        return NULL;

    flr->fp = fp;
    if (plib_flr_determine_file_size(flr) == NULL)
    {
        perror("Could not determine file size");
        fclose(flr->fp);
        free(flr);
        return NULL;
    }

    if (plib_flr_read_file(flr) == NULL)
    {
        perror("Could not read file to buffer");
        fclose(flr->fp);
        free(flr);
        return NULL;
    }

    fclose(flr->fp);

    if (plib_flr_detect_lines(flr) == NULL)
    {
        perror("Could not parse lines");
        free(flr->file_buffer);
        free(flr);
        return NULL;
    }
    flr->current_line = 0U;

    return flr;
}

PBLIB_FLR_DEF size_t
plib_flr_num_lines(plib_flr_t* flr)
{
    return (flr != NULL) ? flr->num_lines : 0U;
}

PBLIB_FLR_DEF const char*
plib_flr_get_next_line(plib_flr_t* flr, size_t* line)
{
    if ((flr == NULL) || (flr->lines == NULL))
        return NULL;
    if (flr->current_line >= flr->num_lines)
        return NULL;

    if (line != NULL)
        *line = flr->current_line + 1U;

    return flr->lines[flr->current_line++];
}

PBLIB_FLR_DEF const char*
plib_flr_get_line(plib_flr_t* flr, size_t n)
{
    if ((flr == NULL) || (n > (flr->num_lines - 1U)))
        return NULL;

    flr->current_line = n - 1U;

    return flr->lines[flr->current_line++];
}

PBLIB_FLR_DEF void
plib_flr_reset(plib_flr_t* flr)
{
    if (flr == NULL)
        return;

    flr->current_line = 0U;
}

PBLIB_FLR_DEF void
plib_flr_free(plib_flr_t* flr)
{
    if (flr == NULL)
        return;

    free(flr->file_buffer);
    free(flr->lines);
    free(flr);
    flr = NULL;
}
#endif /* PLIB_FLR_IMPLEMENTATION */

#ifndef PLIB_FLR_NO_NAMESPACE_GUARD_
#define PLIB_FLR_NO_NAMESPACE_GUARD_
#ifdef PLIB_FLR_NO_NAMESPACE
    #define flr_t plib_flr_t
    #define flr_init plib_flr_init
    #define flr_num_lines plib_flr_num_lines
    #define flr_get_next_line plib_flr_get_next_line
    #define flr_get_line plib_flr_get_line
    #define flr_reset plib_flr_reset
    #define flr_free plib_flr_free
#endif /* PLIB_FLR_NO_NAMESPACE */
#endif /* PLIB_FLR_NO_NAMESPACE_GUARD_ */

/*
 * MIT License
 *
 * Copyright (c) 2024 "piscilus" Julian Kraemer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
