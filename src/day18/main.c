/*
 * Main program for advent of code 2024 day 18.
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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define STEPS       (1024) /* example = (12) */
#define TARGET_X    (70) /* example = (6) */
#define TARGET_Y    TARGET_X
#define GRID_SIZE_X (TARGET_X + 1)
#define GRID_SIZE_Y (TARGET_Y + 1)

typedef struct
{
    int x;
    int y;
} coords_t;

                /* up   down  left  right */
const int dy[] = { -1,  1,     0,   0     };
const int dx[] = {  0,  0,    -1,   1     };

typedef struct
{
    coords_t coords;
    int dist;
} pos_t;

typedef struct
{
    pos_t pos[GRID_SIZE_Y * GRID_SIZE_X];
    int front;
    int rear;
} queue_t;

static void
queue_init(queue_t* q)
{
    q->front = 0;
    q->rear = 0;
}

static bool
queue_is_empty(queue_t* q)
{
    return (q->front == q->rear);
}

static void
queue_enqueue(queue_t* q, pos_t p)
{
    q->pos[q->rear++] = p;
}

static pos_t
queue_dequeue(queue_t* q)
{
    return q->pos[q->front++];
}

static bool
can_enter_field(coords_t c, int g[GRID_SIZE_Y][GRID_SIZE_X], bool v[GRID_SIZE_Y][GRID_SIZE_X])
{
    return (   (c.y >= 0)
            && (c.y < GRID_SIZE_Y)
            && (c.x >= 0)
            && (c.x < GRID_SIZE_X)
            && (g[c.y][c.x] == 0)
            && !v[c.y][c.x]);
}

static int
shortest_path(int grid[GRID_SIZE_Y][GRID_SIZE_X], pos_t start, pos_t end)
{
    bool visited[GRID_SIZE_Y][GRID_SIZE_X] = {false};

    queue_t q;
    queue_init(&q);

    queue_enqueue(&q, start);
    visited[start.coords.y][start.coords.x] = true;

    while (!queue_is_empty(&q))
    {
        pos_t pos = queue_dequeue(&q);

        if (   pos.coords.y == end.coords.y
            && pos.coords.x == end.coords.x)
            return pos.dist;

        for (int i = 0; i < 4; i++)
        {
            coords_t n =
            {
                .y = pos.coords.y + dy[i],
                .x = pos.coords.x + dx[i]
            };

            if (can_enter_field(n, grid, visited))
            {
                visited[n.y][n.x] = true;
                queue_enqueue(&q, (pos_t){.coords = n, .dist = pos.dist + 1});
            }
        }
    }

    return -1;
}

int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 18: RAM Run\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    flr_t* f = flr_init(*(argv + 1));

    pos_t start = {.coords.x = 0, .coords.y = 0, .dist = 0};
    pos_t end = {.coords.x = TARGET_X, .coords.y = TARGET_Y, .dist = 0};

    int grid[GRID_SIZE_Y][GRID_SIZE_X] = {0};

    const char* p;
    int i = 0;
    while ((p = flr_get_next_line(f, NULL)) != NULL)
    {
        int y, x;
        if (sscanf(p, "%d,%d", &x, &y) != 2)
        {
            fprintf(stderr, "Failed to parse line %d: '%s'\n", i, p);
            break;
        }
        assert(y < GRID_SIZE_Y);
        assert(x < GRID_SIZE_X);
        grid[y][x] = 1;
        i++;
        if (i == STEPS)
            break;
    }

#if 0
    for (int y = 0; y < GRID_SIZE_Y; y++)
    {
        for (int x = 0; x < GRID_SIZE_X; x++)
        {
            printf("%d", grid[y][x]);
        }
        putchar('\n');
    }
#endif

    int result = shortest_path(grid, start, end);

    printf("Part 1: minimum number of steps = %d\n", result);


    flr_free(f);

    return EXIT_SUCCESS;
}
