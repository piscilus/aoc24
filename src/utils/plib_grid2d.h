/*
 * PLIB_GRID2D - v0.1.0 - File Line Reader.
 *
 * The GRID2D read contents from a file into memory and allows access to
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

#ifndef PLIB_GRID2D_H
#define PLIB_GRID2D_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef PBLIB_GRID2D_STATIC
#define PBLIB_GRID2D_DEF static
#else
#define PBLIB_GRID2D_DEF extern
#endif

typedef struct
{
    FILE* fp;
    size_t file_size;  /* w/out null terminator */
    char* file_buffer; /* complete file content null terminated */
    char** grid;
    size_t** flags;
    size_t max_x;
    size_t max_y;
} plib_grid2d_t;

PBLIB_GRID2D_DEF plib_grid2d_t*
plib_grid2d_init(char* file_path);

PBLIB_GRID2D_DEF void
plib_grid2d_print(plib_grid2d_t* g);

PBLIB_GRID2D_DEF void
plib_grid2d_free(plib_grid2d_t* g);

#endif /* PLIB_GRID2D_H */

#define PLIB_GRID2D_IMPLEMENTATION
#ifdef PLIB_GRID2D_IMPLEMENTATION

static plib_grid2d_t*
plib_grid2d_determine_file_size(plib_grid2d_t* g)
{
    assert(g != NULL);
    assert(g->fp != NULL);

    if (fseek(g->fp, 0, SEEK_END) != 0)
        return NULL;

    long file_size;
    if ((file_size = ftell(g->fp)) < 0)
        return NULL;

    if (fseek(g->fp, 0, SEEK_SET) != 0)
        return NULL;

    g->file_size = (size_t)file_size;

    return g;
}

static plib_grid2d_t*
plib_grid2d_read_file_content(plib_grid2d_t* g)
{
    assert(g != NULL);
    assert(g->fp != NULL);

    if (g->file_size == 0U)
        return NULL;

    g->file_buffer = (char*)malloc(g->file_size + 1U);
    if (g->file_buffer == NULL)
        return NULL;

    if (fread(g->file_buffer, 1, g->file_size, g->fp) != g->file_size)
    {
        free(g->file_buffer);
        return NULL;
    }

    g->file_buffer[g->file_size] = '\0';

    return g;
}

static plib_grid2d_t*
plib_grid2d_prepare_grid(plib_grid2d_t* g)
{
    assert(g != NULL);

    /* determine max y by counting EOL */
    g->max_y = 0U;
    for (size_t i = 0U; i < g->file_size; i++)
        if (g->file_buffer[i] == '\n')
            g->max_y++;

    if (g->file_buffer[g->file_size - 1] != '\n')
        g->max_y++;

    g->grid = malloc(g->max_y * sizeof(char*));
    if (g->grid == NULL)
        return NULL;

    char* p = g->file_buffer;
    g->max_x = 0U;
    while (*p != '\0' && *p != '\n')
    {
        p++;
        g->max_x++;
    }

    p = g->file_buffer;
    for (size_t i = 0U; i < g->max_y; i++)
    {
        g->grid[i] = p;
        size_t line_length = strcspn(p, "\n");
        if (line_length != g->max_x)
        {
            free(g->grid);
            return NULL;
        }
        g->grid[i][line_length] = '\0';
        p += line_length + 1U;
    }

    g->flags = calloc(g->max_y, sizeof(size_t*));
    if (g->grid == NULL)
    {
        free(g->grid);
        return NULL;
    }

    for (size_t y = 0; y < g->max_y; y++)
    {
        g->flags[y] = calloc(g->max_x, sizeof(size_t));
        if (g->flags[y] == NULL)
        {
            free(g->grid);
            free(g->flags);
            return NULL;
        }
    }

    return g;
}

PBLIB_GRID2D_DEF plib_grid2d_t*
plib_grid2d_init(char* file_path)
{
    if (file_path == NULL)
        return NULL;

    plib_grid2d_t* g = malloc(sizeof(plib_grid2d_t));
    if (g == NULL)
    {
        perror("Memory allocation error");
        return NULL;
    }

    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        perror("Could not open file");
        free(g);
        return NULL;
    }

    g->fp = fp;
    if (plib_grid2d_determine_file_size(g) == NULL)
    {
        perror("Could not determine file size");
        fclose(g->fp);
        free(g);
        return NULL;
    }

    if (plib_grid2d_read_file_content(g) == NULL)
    {
        perror("Could not read file to buffer");
        fclose(g->fp);
        free(g);
        return NULL;
    }

    if (plib_grid2d_prepare_grid(g) == NULL)
    {
        perror("Could not prepare grid");
        fclose(g->fp);
        free(g);
        return NULL;
    }

    fclose(g->fp);

    return g;
}

PBLIB_GRID2D_DEF void
plib_grid2d_print(plib_grid2d_t* g)
{
    if ((g == NULL) || (g->grid == NULL))
        return;

    printf("x=%zu, y=%zu:\n", g->max_x, g->max_y);
    for (size_t i = 0U; i < g->max_y; i++)
        printf("%s\n", g->grid[i]);
}

PBLIB_GRID2D_DEF void
plib_grid2d_free(plib_grid2d_t* g)
{
    free(g->file_buffer);
    for (size_t y = 0U; y < g->max_y; y++)
        free(g->flags[y]);
    free(g->flags);
    free(g->grid);
    free(g);

    return;
}
#endif /* PLIB_GRID2D_IMPLEMENTATION */

#ifndef PLIB_GRID2D_NO_NAMESPACE_GUARD_
#define PLIB_GRID2D_NO_NAMESPACE_GUARD_
#ifdef PLIB_GRID2D_NO_NAMESPACE
    #define grid2d_t plib_grid2d_t
    #define grid2d_init plib_grid2d_init
    #define grid2d_print plib_grid2d_print
    #define grid2d_free plib_grid2d_free
#endif /* PLIB_GRID2D_NO_NAMESPACE */
#endif /* PLIB_GRID2D_NO_NAMESPACE_GUARD_ */

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
