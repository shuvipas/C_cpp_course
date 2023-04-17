#include "sort_bus_lines.h"

void swap(BusLine *a, BusLine *b);

void quick_sort(BusLine *start, BusLine *end, SortType sort_type)
{
    if (start < end)
    {
        BusLine *pivot = partition(start, end, sort_type);
        quick_sort(start, pivot - 1, sort_type);
        quick_sort(pivot + 1, end, sort_type);
    }
}
BusLine *partition(BusLine *start, BusLine *end, SortType sort_type)
{
    BusLine *pivot = end;

    int n = end - start;
    int i = 0;
    for (int j = 0; j < n; j++)
    {
        int j_val = sort_type ? (start + j)->duration : (start + j)->distance;
        int p_val = sort_type ? pivot->duration : pivot->distance;
        if (j_val <= p_val)
        {
            swap((start + i), (start + j));
            i++;
        }
    }
    swap((start + i), end);
    return (start + i);
}

void swap(BusLine *a, BusLine *b)
{
    BusLine temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(BusLine *start, BusLine *end)
{
    int n = end - start;

    for (int i = 0; i < n; i++)
    {
        int swaped = 0;

        for (int j = 0; j < n - i; j++)
        {
            if (strcmp((start + j)->name, (start + j + 1)->name) > 0)
            {
                swap((start + j), (start + j + 1));
                swaped = 1;
            }
        }
        if (swaped == 0)
        {
            break;
        }
    }
}