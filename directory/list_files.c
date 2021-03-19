/****************************************************************************************************
 * FILE:    list_files.c
 * BRIEF:   List Files Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <dirent.h>
#include <errno.h>
#include "error.h"
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
    DIR *dirp;
    struct dirent *dp;
    
    /*** Check Input ***/
    if(argc != 2)
        error_exitFailure(false, "Usage: %s [directory]\n", argv[0]);
    
    /*** Open Directory ***/
    dirp = opendir(argv[1]);
    if(dirp == NULL)
        error_exitFailure(true, "Failed Opening Directory: %s\n", argv[1]);
    
    /*** List Files ***/
    printf("%s\n", argv[1]);
    while(1)
    {
        /* Set Up */
        errno = 0;
        
        /* Read From Directory */
        dp = readdir(dirp);
        
        /* Error Check */
        if(dp == NULL)
            break;
        
        /* Print File */
        if((strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0))
            printf("  %s\n", dp->d_name);
    }
    
    /*** Error Check ***/
    if(errno != 0)
        error_exitFailure(true, "Failed Reading Directory: %s\n", argv[1]);
    
    /*** Close Directory ***/
    if(closedir(dirp) == -1)
        error_exitFailure(true, "Failed Closing Directory: %s\n", argv[1]);
    
    exit(EXIT_SUCCESS);
}
