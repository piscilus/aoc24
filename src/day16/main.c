/*
 * Main program for advent of code 2024 day 16.
 *
 * (C) Copyright 2024 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#define PLIB_GRID2D_IMPLEMENTATION
#define PLIB_GRID2D_NO_NAMESPACE
#include "plib_grid2d.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCORE_FWD  (1)
#define SCORE_TURN (1000)

#define COSTS_INDEX(y, x, d) (((y) * g->max_x + (x)) * 4U + (d))

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
    int y;
    int x;
    int direction;
    int cost;
} node_t;

typedef struct
{
    node_t* nodes;
    size_t size;
    size_t capacity;
} pqueue_t;

static bool
pqueue_init(pqueue_t* pq, size_t max_size)
{
    pq->nodes = malloc(max_size * sizeof(node_t));
    if (pq->nodes == NULL)
    {
        fprintf(stderr, "Buy more RAM\n");
        return false;
    }
    pq->size = 0U;
    pq->capacity = max_size;
    return true;
}

static bool
pqueue_is_empty(pqueue_t* pq)
{
    return (pq->size == 0U);
}

static void
pqueue_push(pqueue_t* pq, node_t node)
{
    assert(pq->size < pq->capacity);
    pq->nodes[pq->size++] = node;
    for (size_t i = pq->size - 1U; i-- > 0U; )
    {
        if (pq->nodes[i].cost < pq->nodes[i - 1U].cost)
        {
            node_t temp = pq->nodes[i];
            pq->nodes[i] = pq->nodes[i - 1U];
            pq->nodes[i - 1U] = temp;
        }
        else
        {
            break;
        }
    }
}

static node_t
pqueue_pop(pqueue_t* pq)
{
    assert(pq->size > 0U);
    return pq->nodes[--pq->size];
}


static void
pqueue_free(pqueue_t* pq)
{
    free(pq->nodes);
    pq->nodes = NULL;
    pq->size = 0U;
    pq->capacity = 0U;
}

static int
can_enter_field(int y, int x, grid2d_t* g)
{
    return (   (y >= 0)
            && (y < (int)g->max_y)
            && (x >= 0)
            && (x < (int)g->max_x)
            && (g->grid[y][x] == '.'));
}

static int
shortest_path(grid2d_t* g, coords_t* start, int start_dir, coords_t* end)
{
    int* costs = malloc(g->max_x * g->max_y * 4U * sizeof(int));
    if (costs == NULL)
    {
        fprintf(stderr, "Buy more RAM\n");
        return -1;
    }

    for (size_t i = 0U; i < (g->max_x * g->max_y * 4U); i++)
        costs[i] = INT_MAX;

    pqueue_t pq;
    if (!pqueue_init(&pq, g->max_x * g->max_y * 4U))
        return -1;

    pqueue_push(&pq, (node_t){start->y, start->x, start_dir, 0});
    costs[COSTS_INDEX(start->y, start->x, start_dir)] = 0;

    while (!pqueue_is_empty(&pq))
    {
        node_t current = pqueue_pop(&pq);
        int y = current.y;
        int x = current.x;
        int dir = current.direction;
        int cost = current.cost;

        if (cost > costs[COSTS_INDEX(y, x, dir)])
            continue;

        int ny = y + dy[dir];
        int nx = x + dx[dir];
        int nc = cost + SCORE_FWD;

        if (   can_enter_field(ny, nx, g)
            && (nc < costs[COSTS_INDEX(ny, nx, dir)]))
        {
            costs[COSTS_INDEX(ny, nx, dir)] = nc;
            g->flags[ny][nx]++;
            pqueue_push(&pq, (node_t){ny, nx, dir, nc});
        }

        for (int d = 0; d < 4; d++)
        {
            if (d != dir)
            {
                int turncost = cost + SCORE_TURN;
                if (turncost < costs[COSTS_INDEX(y, x, d)])
                {
                    costs[COSTS_INDEX(y, x, d)] = turncost;
                    pqueue_push(&pq, (node_t){y, x, d, turncost});
                }
            }
        }
    }

    int cost_min = INT_MAX;
    for (int d = 0; d < 4; d++)
        if (costs[COSTS_INDEX(end->y, end->x, d)] < cost_min)
            cost_min = costs[COSTS_INDEX(end->y, end->x, d)];

    free(costs);
    pqueue_free(&pq);

    return (cost_min == INT_MAX) ? -1 : cost_min;
}


int
main(int argc, char** argv)
{
    printf("Advent of Code 2024 - 16: Reindeer Maze\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* g = grid2d_init(*(argv + 1));
    if (g == NULL)
    {
        fprintf(stderr, "Input error.\n");
        exit(EXIT_FAILURE);
    }

    coords_t start = {.y = INT_MIN};
    coords_t end = {.y = INT_MIN};
    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if (g->grid[y][x] == 'S')
            {
                start.x = x;
                start.y = y;
                g->grid[y][x] = '.';
            }
            else if (g->grid[y][x] == 'E')
            {
                end.x = x;
                end.y = y;
                g->grid[y][x] = '.';
            }
        }
    }
    assert(start.y > 0);
    assert(end.y > 0);

    int result = shortest_path(g, &start, 3, &end);

    printf("Part 1: %d\n", result);

    grid2d_free(g);

    return EXIT_SUCCESS;
}
