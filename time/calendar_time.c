/****************************************************************************************************
 * FILE:    calendar_time.c
 * BRIEF:   Calendar Time Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define CALENDAR_TIME_SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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
    time_t t;
    struct timeval tv;
    struct tm gm, *gmp, loc, *locp;
    
    /*** Get Time ***/
    t = time(NULL);
    if(gettimeofday(&tv, NULL) == -1)
        error_exitFailure(true, "Failed Getting Time Of Day\n");
    gmp = gmtime(&t);
    if(gmp == NULL)
        error_exitFailure(true, "Failed Getting GM Time\n");
    gm = *gmp; // Save Copy; Returned Structure May Be Modified In Calls To Other Time Functions
    locp = localtime(&t);
    if(locp == NULL)
        error_exitFailure(true, "Failed Getting Local Time\n");
    loc = *locp; // Save Copy; Returned Structure May Be Modified In Calls To Other Time Functions
    
    /*** Print Time ***/    
    printf("%ld (%ld.%06ld) Seconds Since The Epoch (January 1, 1970); ", t, tv.tv_sec, tv.tv_usec);
    printf("This Is About %0.3f Years\n", t / CALENDAR_TIME_SECONDS_IN_TROPICAL_YEAR);
    printf("GM (Greenwich Mean) Time (Time Since January 1, 1900):\n");
    printf("  Year: %d\n", gm.tm_year);
    printf("  Month: %d (January = 0, February = 1, etc)\n", gm.tm_mon);
    printf("  Day: %d\n", gm.tm_mday);
    printf("  Hours: %d\n", gm.tm_hour);
    printf("  Minutes: %d\n", gm.tm_min);
    printf("  Seconds: %d\n", gm.tm_sec);
    printf("  Day Of Week: %d (Sunday = 0, Monday = 1, etc)\n", gm.tm_wday);
    printf("  Day Of Year: %d\n", gm.tm_yday);
    printf("  Is DST: %s\n", gm.tm_isdst ? "TRUE" : "FALSE");
    printf("Local Time (Time Since January 1, 1900):\n");
    printf("  Year: %d\n", loc.tm_year);
    printf("  Month: %d (January = 0, February = 1, etc)\n", loc.tm_mon);
    printf("  Day: %d\n", loc.tm_mday);
    printf("  Hours: %d\n", loc.tm_hour);
    printf("  Minutes: %d\n", loc.tm_min);
    printf("  Seconds: %d\n", loc.tm_sec);
    printf("  Day Of Week: %d (Sunday = 0, Monday = 1, etc)\n", loc.tm_wday);
    printf("  Day Of Year: %d\n", loc.tm_yday);
    printf("  Is DST: %s\n", loc.tm_isdst ? "TRUE" : "FALSE");
    
    /*** Format/Convert And Print Time ***/
    printf("ctime() Formats time() Value As: %s", ctime(&t));
    printf("asctime() Formats gmtime() Value As: %s", asctime(&gm));
    printf("asctime() Formats localtime() Value As: %s", asctime(&loc));
    printf("mktime() Converts gmtime() Value As: %ld\n", mktime(&gm));
    printf("mktime() Converts localtime() Value As: %ld\n", mktime(&loc));
    
    exit(EXIT_SUCCESS);
}
