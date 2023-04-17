#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"


/**
 * This function Checks whether a range of bus lines is sorted by distance.
 * The function returns 1 if the bus lines are sorted, and 0 otherwise.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 * @return 1 if the bus lines are sorted by distance, 0 otherwise.
 */
int is_sorted_by_distance(BusLine *start, BusLine *end);

/**
 * This function Checks whether a range of bus lines is sorted by duration.
 * The function returns 1 if the bus lines are sorted, and 0 otherwise.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 * @return 1 if the bus lines are sorted by duration, 0 otherwise.
 */
int is_sorted_by_duration(BusLine *start, BusLine *end);

/**
 * This function Checks whether a range of bus lines is sorted by name.
 * The function returns 1 if the bus lines are sorted, and 0 otherwise.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 * @return 1 if the bus lines are sorted by name, 0 otherwise.
 */
int is_sorted_by_name(BusLine *start, BusLine *end);

/**
 * This function Checks whether two arrays of bus lines are equal.
 * This function checks whether the bus lines in the sorted array
 * is equal to the bus lines in the original array.The function
 * returns 1 if the ranges are equal, and 0 otherwise.
 * @param start_sorted A pointer to the first bus line in the sorted array.
 * @param end_sorted A pointer to the last bus line in the sorted array.
 * @param start_original A pointer to the first bus line in the original array.
 * @param end_original A pointer to the last bus line in the original array.
 * @return 1 if they are equal, 0 otherwise.
 */
int is_equal(BusLine *start_sorted,
             BusLine *end_sorted, BusLine *start_original,
             BusLine *end_original);

// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#endif // EX2_REPO_TESTBUSLINES_H
