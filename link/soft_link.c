/****************************************************************************************************
 * FILE:    soft_link.c
 * BRIEF:   Soft Link Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
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
    char buf[PATH_MAX];
    ssize_t numBytes;
    struct stat statbuf;
    
    /*** Check Input ***/
    if(argc != 2)
        error_exitFailure(false, "Usage: %s [soft_link]\n  Example: ln -s ../build/existing_file.txt ../build/soft_link.txt; ../build/soft_link.exe ../build/soft_link.txt;\n", argv[0]);
    
    /*** Verify Argument Is A Soft Link ***/
    if(lstat(argv[1], &statbuf) == -1)
        error_exitFailure(true, "lstat Failed On File: %s\n", argv[1]);
    if(!S_ISLNK(statbuf.st_mode))
        error_exitFailure(false, "File Is Not Soft Link: %s\n", argv[1]);
    
    /*** Read Link ***/
    numBytes = readlink(argv[1], buf, PATH_MAX - 1);
    if(numBytes == -1)
        error_exitFailure(true, "readlink Failed On File: %s\n", argv[1]);
    buf[numBytes] = '\0';
    
    /*** Print Results ***/
    printf("readlink: %s -> %s\n", argv[1], buf);
    
    /*** Get Real Path ***/
    if(realpath(argv[1], buf) == NULL)
        error_exitFailure(true, "realpath Failed On File: %s\n", argv[1]);
    
    /*** Print Results ***/
    printf("realpath: %s -> %s\n", argv[1], buf);
    
    exit(EXIT_SUCCESS);
}
