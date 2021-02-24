/****************************************************************************************************
 * FILE:    convert_time_string.c
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define _XOPEN_SOURCE
#define CONVERT_TIME_STRING_STRING_BUFFER_SIZE (1000)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
int main(int argc, char *argv[])
{
    char *outputFormat, string[CONVERT_TIME_STRING_STRING_BUFFER_SIZE];
    struct tm tm;
    
    /*** Check Arguments ***/
    if(argc < 3)
    {
        printf("Usage: %s [input_date_time] [input_date_format] ...\n", argv[0]);
        printf("  [output_date_format]\n");
        printf("  Example:\n");
        printf("    %s \"9:39:46pm 1 Feb 2011\" \"%%I:%%M:%%Spm %%d %%b %%Y\"\n", argv[0]);
        printf("    %s \"9:39:46pm 1 Feb 2011\" \"%%I:%%M:%%Spm %%d %%b %%Y\" \"%%F %%T\"\n", argv[0]);
        error_exitFailure(false, "");
    }
    
    /*** Set Up ***/
    /* Initialize Variables */
    if(argc > 3)
        outputFormat = argv[3];
    else
        outputFormat = "%H:%M:%S %A, %d %B %Y %Z";
    memset(&tm, 0, sizeof(tm));
    
    /* Set Locale */
    if(setlocale(LC_ALL, "") == NULL)
        error_exitFailure(true, "Failed Setting Locale\n");
    
    /*** Convert Input Date Time (strptime) ***/
    if(strptime(argv[1], argv[2], &tm) == NULL)
        error_exitFailure(true, "Failed Converting Input Date Time (strptime)\n");
    tm.tm_isdst = -1; // Not Set By strptime; If tm_isdst < 0, mktime Won't Use This Field
    printf("strptime: %ld Seconds Since The Epoch (January 1, 1970)\n", mktime(&tm));
    
    /*** Convert Input Date Time (strftime) ***/
    if(strftime(string, CONVERT_TIME_STRING_STRING_BUFFER_SIZE, outputFormat, &tm) == 0)
        error_exitFailure(true, "Failed Converting Input Date Time (strftime)\n");
    printf("strftime: \"%s\" Converted To \"%s\"\n", argv[1], string);
    
    exit(EXIT_SUCCESS);
}
