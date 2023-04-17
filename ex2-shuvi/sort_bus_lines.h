#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
 //EX2_REPO_SORTBUSLINES_H
#include <string.h>
#define NAME_LEN 21
/**
 * BusLine struct representing a bus line 
 * with a name, distance, and duration.
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;
} BusLine;
typedef enum SortType
{
    DISTANCE,
    DURATION
} SortType;

/**
 * This function sorts an array of bus lines using the bubble sort algorithm,
 * which repeatedly iterates over the array and swaps adjacent elements if
 *  they are out of order. 
 * The function modifies the order of the elements in place.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * This function sorts the bus lines in the array using the quick 
 * sort algorithm, which divides the range into two subranges based on
 *  a pivot element, and recursively sorts each subrange. 
 * The function modifies the order of the elements in place.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 * @param sort_type The sorting criteria to use, either SORT_BY_DISTANCE
 *  or SORT_BY_DURATION.
 */
void quick_sort (BusLine *start, BusLine *end, SortType sort_type);

/**
 * This function partitions the bus lines in the array around a pivot element
 * using the given sorting criteria, such that all elements before the pivot 
 * are less than or equal to the pivot, and all elements after the pivot are
 * greater than the pivot. 
 * The function returns a pointer to the pivot BusLine.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 * @param sort_type The sorting criteria to use, either SORT_BY_DISTANCE
 *  or SORT_BY_DURATION.
 * @return A pointer to the pivot BusLine.
 */
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type);


// write only between #define EX2_REPO_SORTBUSLINES_H and #endif 
//EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
