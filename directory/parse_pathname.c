/****************************************************************************************************
 * FILE:    parse_pathname.c
 * BRIEF:   Parse Pathname Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include "libgen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char *pathname1, *pathname2;
    
    /*** Check Input ***/
    if(argc != 2)
        error_exitFailure(false, "Usage: %s [file_path]\n", argv[0]);
    
    /*** Copy Input ***/
    pathname1 = strdup(argv[1]);
    pathname2 = strdup(argv[1]);
    if((pathname1 == NULL) || (pathname2 == NULL))
        error_exitFailure(true, "Failed Allocating Memory For Pathname: %s\n", argv[1]);
    
    /*** Split Pathname ***/
    printf("%s -> %s + %s\n", argv[1], dirname(pathname1), basename(pathname2));
    
    /*** Clean Up ***/
    free(pathname1);
    free(pathname2);
    
    exit(EXIT_SUCCESS);
}
