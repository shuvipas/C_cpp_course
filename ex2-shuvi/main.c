#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 60
#define MAX_FIELD 20
#define MAX_DISTANCE 1000
#define MIN_DISTANCE 0
#define MAX_DURATION 100
#define MIN_DURATION 10
#define DECIMAL 10
#define CLI_ARG_NUM 2
#define CLI_ARG 1
#define INFO_ARG 3

#define BY_DURATION "by_duration"
#define BY_DISTANCE "by_distance"
#define BY_NAME "by_name"
#define TEST "test"
#define MIN_LINES 1

#define MSG_LINE_NUM "Enter number of lines. Then enter\n"
#define MSG_LINE_INFO "Enter line info. Then enter\n"
#define ERR_MSG_NAME "ERROR: bus name should contain only digits and small \
chars\n"
#define ERR_MSG_DURATION "ERROR: duration should be an integer \
between 10 and 100 (includes)\n"
#define ERR_MSG_DISTANCE "ERROR: distance should be an integer \
between 0 and 1000 (includes)\n"
#define ERR_MSG_MEM "ERROR: memory allocation failed"
#define ERR_MSG_INFO "ERROR: entering line info failed. insert: \
<name>,<distence>,<duration>.\n"
#define ERR_MSG_LINE_NUM "ERROR: the line number must be a positive \
integer bigger then 0\n"
#define CLI_ERR_NUM "USAGE: only 1 command line argument is allowed\n"
#define CLI_ERR_ARG "USAGE: not one of the supported commands\n"

#define TEST_PASSED "TEST %d PASSED: "
#define TEST_FAILED "TEST %d FAILED: "
#define EQUAL_TEST "tested if the bus lines did not change after sorting\n"
#define DISTANCE_TEST "tested if the buses got sorted by distance\n"
#define DURATION_TEST "tested if the buses got sorted by duration\n"
#define NAME_TEST "tested if the buses got sorted by name\n"
#define ERR_MSG_MEM "ERROR: memory allocation failed"


/**
 * This function tests whether a command string is valid. A valid command
 * string is one of the following: "test", "by_distance", "by_duration",
 * or "by_name".
 * If the command string is valid, the function returns `EXIT_SUCCESS`.
 * If the command string is not valid, the function prints an error message to
 * stdout and returns `EXIT_FAILURE`.
 * @param command The command string to be tested.
 * @return `EXIT_SUCCESS` if the command string is valid, `EXIT_FAILURE`
 * otherwise.
 */
int command_tester(char command[MAX_FIELD]);

/**
 * This function prompts the user to input the number of bus lines to
 * be entered and reads the input from the user.
 * It then dynamically allocates an array of BusLine structs of the requested
 * size and prompts the user to enter information for each bus line in the
 * format "<name>,<distance>,<duration>".
 * The entered information is stored in the array of BusLine structs.
 * @param n_ptr A pointer to an int to store the number of bus lines
 * entered by the user.
 * @return A pointer to the allocated array of BusLine structs, or NULL if an
 * error occurred during allocation or input.
 */
BusLine *user_input(int *n_ptr);

/**
 * This function tests the validity of the information of a bus line,
 * namely the bus line name, distance and duration. The bus line name must
 * be composed of lowercase letters and digits only. The distance and duration
 * must be within the valid range of values defined by the constants
 * MIN_DISTANCE and MAX_DISTANCE and MIN_DURATION and MAX_DURATION.
 * @param s A string representing the name of the bus line.
 * @param distance An integer representing the distance of the bus line.
 * @param duration An integer representing the duration of the bus line.
 * @return Returns EXIT_SUCCESS if the information is valid, and
 * EXIT_FAILURE otherwise.
 */
int info_tester(char s[], int distance, int duration);

/**
 * This function takes a user command and a range of bus lines,
 * and performs the appropriate action on the list of bus lines
 * according to the user command.
 * @param command A string that represents the user command to execute.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 * @return Returns EXIT_SUCCESS if the user command was executed successfully,
 * or EXIT_FAILURE if there was an error.
 */
int user_command(char command[MAX_FIELD], BusLine *start, BusLine *end);

/**
 * This function performs several tests on an array of BusLine structs to
 * ensure that the sorting functions in the program are working correctly.
 * It creates a temporary copy of the array and sorts it by distance,
 * duration, and name, and compares the results with the original array
 * to ensure that they match.
 * @param n The number of elements in the array.
 * @param bus_station A pointer to the array of BusLine structs to test.
 * @return Returns EXIT_SUCCESS if the memory allocation success,
 *  or EXIT_FAILURE if it fails.
 */
int tester(int n, BusLine *bus_station);

/**
 * This function sorts a temporary copy of the bus lines array by distance,
 * and then tests whether the sorting was done correctly and whether the
 * original array is still equal to the sorted array.
 * and for each test prints to stdout if it passed or failed.
 * @param n The number of bus lines in the array.
 * @param bus_station The original array of bus lines.
 * @param temp_station The temporary array used for testing.
 * @param i Pointer to the test number.
 */
void by_distance_tester(int n, BusLine *bus_station,
                        BusLine *temp_station, int *i);
/**
 * This function sorts a temporary copy of the bus lines array by duration,
 * and then tests whether the sorting was done correctly and whether the
 * original array is still equal to the sorted array,
 * and for each test prints to stdout if it passed or failed.
 * @param n The number of bus lines in the array.
 * @param bus_station The original array of bus lines.
 * @param temp_station The temporary array used for testing.
 * @param i Pointer to the test number.
 */
void by_duration_tester(int n, BusLine *bus_station,
                        BusLine *temp_station, int *i);
/**
 * This function sorts a temporary copy of the bus lines array by name,
 * and then tests whether the sorting was done correctly and whether the
 * original array is still equal to the sorted array,
 * and for each test prints to stdout if it passed or failed.
 * @param n The number of bus lines in the array.
 * @param bus_station The original array of bus lines.
 * @param temp_station The temporary array used for testing.
 * @param i Pointer to the test number.
 */
void by_name_tester(int n, BusLine *bus_station,
                    BusLine *temp_station, int *i);

/**
 * This function prints the name, distance, and duration of each bus line
 * in the array to stdout.
 * @param start A pointer to the first bus line.
 * @param end A pointer to the last bus line.
 */
void print_buses(BusLine *start, BusLine *end);

/**
 * This program sorts and displays bus lines information based on user input.
 * The program takes input from the user in the form of bus lines, which
 *  include the name of the bus line, the distance it travels, and the
 *  duration of the trip. The user can then sort and display the bus lines
 *  by name, distance, or duration, or run tests to ensure that the program
 *  is functioning correctly.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return Returns EXIT_SUCCESS if the program completed successfully
 *  or EXIT_FAILURE otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != CLI_ARG_NUM)
    {
        printf(CLI_ERR_NUM);
        return EXIT_FAILURE;
    }
    if(command_tester(argv[CLI_ARG]))
    {
        return EXIT_FAILURE;
    }
    int n = 0;
    BusLine *bus_station = user_input(&n);

    if (bus_station == NULL)
    {
        return EXIT_FAILURE;
    }

    int try = user_command(argv[CLI_ARG], bus_station, bus_station + n - 1);
    if (try)
    {
        return EXIT_FAILURE;
    }

    free(bus_station);
    return EXIT_SUCCESS;
}

BusLine *user_input(int *n_ptr)
{
    char buffer[MAX_LINE];
    printf(MSG_LINE_NUM);
    fgets(buffer, MAX_LINE, stdin);
    *n_ptr = strtol(buffer, NULL, DECIMAL);
    if (*n_ptr < MIN_LINES)
    {
        printf(ERR_MSG_LINE_NUM);
        return NULL;
    }
    BusLine *temp = malloc(sizeof(BusLine) * (*n_ptr));
    if (temp == NULL)
    {
        printf(ERR_MSG_MEM);
        return NULL;
    }
    BusLine *buses = temp;

    int i = 0;
    while (i < *n_ptr)
    {
        printf(MSG_LINE_INFO);
        fgets(buffer, MAX_LINE, stdin);

        int s = sscanf(buffer, "%[^,],%d,%d", buses[i].name,
                       &buses[i].distance, &buses[i].duration);
        if (s != INFO_ARG)
        {
            printf(ERR_MSG_INFO);
        }
        else if (!info_tester(buses[i].name, buses[i].distance,
                              buses[i].duration))
        {
            i++;
        }
    }
    return buses;
}

int info_tester(char s[], int distance, int duration)
{
    int n = strlen(s);
    for (int i = 0; i < n; i++)
    {
        if (!isdigit((int)s[i]) && !islower((int)s[i]))
        {
            printf(ERR_MSG_NAME);
            return EXIT_FAILURE;
        }
    }
    if (distance < MIN_DISTANCE || distance > MAX_DISTANCE)
    {
        printf(ERR_MSG_DISTANCE);
        return EXIT_FAILURE;
    }
    if (duration < MIN_DURATION || duration > MAX_DURATION)
    {
        printf(ERR_MSG_DURATION);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int command_tester(char command[MAX_FIELD])
{
    if (!strcmp(command, TEST)||!strcmp(command, BY_DISTANCE)||
        !strcmp(command, BY_DURATION)||!strcmp(command, BY_NAME))
    {
        return EXIT_SUCCESS;
    }
    else
    {
        printf(CLI_ERR_ARG);
        return EXIT_FAILURE;
    }
}

int user_command(char command[MAX_FIELD], BusLine *start, BusLine *end)
{
    if (!strcmp(command, TEST))
    {
        int n = end - start;
        if (tester(n, start))
        {
            printf(ERR_MSG_MEM);
            return EXIT_FAILURE;
        }
    }
    else if (!strcmp(command, BY_DISTANCE))
    {
        quick_sort(start, end, DISTANCE);
        print_buses(start, end);
    }
    else if (!strcmp(command, BY_DURATION))
    {
        quick_sort(start, end, DURATION);
        print_buses(start, end);
    }
    else if (!strcmp(command, BY_NAME))
    {
        bubble_sort(start, end);
        print_buses(start, end);
    }
    return EXIT_SUCCESS;
}
int tester(int n, BusLine *bus_station)
{
    BusLine *temp_station = malloc(sizeof(BusLine) * n);
    if (temp_station == NULL)
    {
        printf(ERR_MSG_MEM);
        return EXIT_FAILURE;
    }
    memcpy(temp_station, bus_station, sizeof(BusLine) * n);
    int i = 1; // test number
    by_distance_tester(n, bus_station, temp_station, &i);
    by_duration_tester(n, bus_station, temp_station, &i);
    by_name_tester(n, bus_station, temp_station, &i);
    free(temp_station);
    return EXIT_SUCCESS;
}

void by_distance_tester(int n, BusLine *bus_station,
                        BusLine *temp_station, int *i)
{
    quick_sort(temp_station, temp_station + n - 1, DISTANCE);
    if (is_sorted_by_distance(temp_station, temp_station + n - 1))
    {
        printf(TEST_PASSED, *i);
    }
    else
    {
        printf(TEST_FAILED, *i);
    }
    (*i)++;
    printf(DISTANCE_TEST);

    if (is_equal(temp_station, temp_station + n - 1, bus_station,
                 bus_station + n - 1))
    {
        printf(TEST_PASSED, *i);
    }
    else
    {
        memcpy(temp_station, bus_station, sizeof(BusLine) * n);
        printf(TEST_FAILED, *i);
    }
    (*i)++;
    printf(EQUAL_TEST);
}
void by_duration_tester(int n, BusLine *bus_station,
                        BusLine *temp_station, int *i)
{
    quick_sort(temp_station, temp_station + n - 1, DURATION);
    if (is_sorted_by_duration(temp_station, temp_station + n - 1))
    {
        printf(TEST_PASSED, *i);
    }
    else
    {
        printf(TEST_FAILED, *i);
    }
    (*i)++;
    printf(DURATION_TEST);
    if (is_equal(temp_station, temp_station + n - 1, bus_station,
                 bus_station + n - 1))
    {
        printf(TEST_PASSED, *i);
    }
    else
    {
        memcpy(temp_station, bus_station, sizeof(BusLine) * n);
        printf(TEST_FAILED, *i);
    }
    (*i)++;
    printf(EQUAL_TEST);
}

void by_name_tester(int n, BusLine *bus_station,
                    BusLine *temp_station, int *i)
{
    bubble_sort(temp_station, temp_station + n - 1);
    if (is_sorted_by_name(temp_station, temp_station + n - 1))
    {
        printf(TEST_PASSED, *i);
    }
    else
    {
        printf(TEST_FAILED, *i);
    }
    (*i)++;
    printf(NAME_TEST);
    if (is_equal(temp_station, temp_station + n - 1, bus_station,
                 bus_station + n - 1))
    {
        memcpy(temp_station, bus_station, sizeof(BusLine) * n);
        printf(TEST_PASSED, *i);
    }
    else
    {
        printf(TEST_FAILED, *i);
    }
    (*i)++;
    printf(EQUAL_TEST);
}

void print_buses(BusLine *start, BusLine *end)
{
    int n = end - start + 1;
    for (int i = 0; i < n; i++)
    {
        printf("%s,%d,%d\n", (start + i)->name, (start + i)->distance,
               (start + i)->duration);
    }
}