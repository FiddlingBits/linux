/****************************************************************************************************
 * FILE:    pid_max.c
 * BRIEF:   pid_max Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define PID_MAX_MAX_LINE_LENGTH (100)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   main
 * BRIEF:   Main Entry
 * RETURN:  int: Success Or Failure Status
 * ARG:     argc: Number Of Arguments
 * ARG:     argv: Argument String List
 ****************************************************************************************************/
int main(int argc, const char *argv[])
{
    char line[PID_MAX_MAX_LINE_LENGTH];
    int fd;
    ssize_t n;
    
    /*** Check Input ***/
    if(argc != 2)
        error_exitFailure(false, "Usage: %s [new_pid_max]\n", argv[0]);
    
    /*** Open File ***/
    fd = open("/proc/sys/kernel/pid_max", O_RDWR);
    if(fd == -1)
        error_exitFailure(true, "Failed Opening File\n");
    
    /*** Read File ***/
    n = read(fd, line, PID_MAX_MAX_LINE_LENGTH);
    if(n == -1)
        error_exitFailure(true, "Failed Reading File\n");
    
    /*** Write File ***/
    if(lseek(fd, 0, SEEK_SET) == -1)
        error_exitFailure(true, "Failed Seeking File\n");
    if(write(fd, argv[1], strlen(argv[1])) != (ssize_t)strlen(argv[1]))
        error_exitFailure(true, "Failed Writing File\n");
    
    /*** Print Results ***/
    printf("pid_max:\n");
    printf("  Old: %.*s", n, line);
    printf("  New: ");
    fflush(stdout);
    system("cat /proc/sys/kernel/pid_max");
    
    /*** Close File ***/
    if(close(fd) == -1)
        error_exitFailure(true, "Failed Closing File\n");
    
    exit(EXIT_SUCCESS);
}
