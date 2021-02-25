/****************************************************************************************************
 * FILE:    process_time.c
 * BRIEF:   Process Time Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define PROCESS_TIME_DEFAULT_NUM_CALLS (10000000)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include "number.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void processTime_displayProcessTimes(const char *msg);

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
    long i, numCalls;
    
    /*** Check Arguments; Set Default If Necessary ***/
    if(argc > 1)
        numCalls = number_getLong(argv[1]);
    else
        numCalls = PROCESS_TIME_DEFAULT_NUM_CALLS;
    
    /*** Display Process Times ***/
    processTime_displayProcessTimes("Before Delay:");
    
    /*** Delay ***/
    printf("Calling getppid %ld Times...\n", numCalls);
    for(i = 0; i < numCalls; i++)
        (void)getppid();
    
    /*** Display Process Times ***/
    processTime_displayProcessTimes("After Delay:");
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   processTime_displayProcessTimes
 * BRIEF:   Display Process Times
 * RETURN:  void: Returns Nothing
 * ARG:     msg: Message To Print Before Displaying Process Times
 ****************************************************************************************************/
static void processTime_displayProcessTimes(const char *msg)
{
    static long clockTicks = 0;
    clock_t clockTime;
    struct tms t;
    
    /*** Set Up ***/
    /* Get Clock Ticks, If Necessary */
    if(clockTicks == 0)
    {
        clockTicks = sysconf(_SC_CLK_TCK);
        if(clockTicks == -1)
        {
            error_exitFailure(true, "Failed Getting Clock Ticks\n");
        }
        else
        {
            printf("Info:\n");
            printf("  CLOCKS_PER_SEC: %ld\n", CLOCKS_PER_SEC);
            printf("  sysconf(_SC_CLK_TCK): %ld\n", clockTicks);
        }
    }
    
    /*** Display Process Times ***/
    /* Display Message, If Necessary */
    if(msg != NULL)
        printf("%s\n", msg);
    
    /* Get Process Times */
    clockTime = clock();
    if(clockTime == -1)
        error_exitFailure(true, "Failed Getting Clock Time\n");
    if(times(&t) == -1)
        error_exitFailure(true, "Failed Getting Process Times\n");
    
    /* Display Process Times */
    printf("  clock(): %ld (%ld / %ld = %.3f Seconds)\n", clockTime, clockTime, CLOCKS_PER_SEC, (double)clockTime / CLOCKS_PER_SEC);
    printf("  times():\n");
    printf("    User = %ld (%ld / %ld = %.3f)\n", t.tms_utime, t.tms_utime, clockTicks, (double)t.tms_utime / clockTicks);
    printf("    System = %ld (%ld / %ld = %.3f)\n", t.tms_stime, t.tms_stime, clockTicks, (double)t.tms_stime / clockTicks);
}
