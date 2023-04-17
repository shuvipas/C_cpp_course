#include "test_bus_lines.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int is_sorted_by_distance(BusLine *start, BusLine *end)
{
    size_t n = end - start;
    for (size_t i = 0; i < n - 1; i++)
    {
        if ((start + i)->distance > (start + i + 1)->distance)
        {
            return 0;
        }
    }
    return 1;
}

int is_sorted_by_duration(BusLine *start, BusLine *end)
{
    size_t n = end - start;
    for (size_t i = 0; i < n - 1; i++)
    {
        if ((start + i)->duration > (start + i + 1)->duration)
        {
            return 0;
        }
    }
    return 1;
}

int is_sorted_by_name(BusLine *start, BusLine *end)
{
    size_t n = end - start;
    for (size_t i = 0; i < n - 1; i++)
    {
        if (strcmp((start + i)->name, (start + i + 1)->name) > 0)
        {
            return 0;
        }
    }
    return 1;
}

int is_equal(BusLine *start_sorted, BusLine *end_sorted,
             BusLine *start_original, BusLine *end_original)
{
    size_t n = end_sorted - start_sorted;
    if (n != (size_t)(end_original - start_original))
    {
        return 0;
    }

    int equal = 0;
    for (size_t i = 0; i < n; i++)
    {
        equal = 0;
        size_t j = 0;
        while (!equal)
        {
            if (!strcmp((start_sorted + i)->name, (start_original + j)->name))
            {
                equal = 1;
            }
            else if (j == n - 1)
            {
                return 0;
            }
            j++;
        }
    }
    return 1;
}
