/****************************************************************************************************
 * FILE:    uname.c
 * BRIEF:   uname Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define _GNU_SOURCE

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

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
    struct utsname uts;
    
    /*** Get Host System Information ***/
    if(uname(&uts) == -1)
        error_exitFailure(true, "Failed Getting Host System Information\n");
    
    /*** Print Host System Information ***/
    printf("Node Name: %s\n", uts.nodename);
    printf("System Name: %s\n", uts.sysname);
    printf("Release: %s\n", uts.release);
    printf("Version: %s\n", uts.version);
    printf("Machine: %s\n", uts.machine);
    printf("Domain Name: %s\n", uts.domainname);
    
    exit(EXIT_SUCCESS);
}
