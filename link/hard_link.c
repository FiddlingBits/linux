/****************************************************************************************************
 * FILE:    hard_link.c
 * BRIEF:   Hard Link Source File
 ****************************************************************************************************/

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
    char string[] = "Write To Hard Link\n";
    int fd;
    
    /*** Check Input ***/
    if(argc != 3)
        error_exitFailure(false, "Usage: %s [existing_file] [hard_link]\n  Example: cat ../build/existing_file.txt; ../build/hard_link.exe ../build/existing_file.txt ../build/hard_link.txt; cat ../build/existing_file.txt; cat ../build/hard_link.txt;\n", argv[0]);
    
    /*** Create Hard Link ***/
    if(link(argv[1], argv[2]) == -1)
        error_exitFailure(true, "Failed To Create Hard Link (%s) From File (%s)\n", argv[2], argv[1]);
    printf("Created Hard Link: %s\n", argv[2]);
    
    /*** Open Hard Link ***/
    fd = open(argv[2], O_WRONLY | O_APPEND);
    if(fd == -1)
        error_exitFailure(true, "Failed Opening Hard Link: %s\n", argv[2]);
    printf("Opened Hard Link: %s\n", argv[2]);
    
    /*** Delete Hard Link (Hard Link Not Actually Deleted Until File Descriptor Closed) ***/
    if(unlink(argv[2]) == -1)
        error_exitFailure(true, "Failed To Delete (Unlink) Hard Link (%s)\n");
    printf("Deleted (Unlinked) Hard Link: %s\n", argv[2]);
    
    /*** Write To Hard Link ***/
    if(write(fd, string, strlen(string)) != (ssize_t)strlen(string))
        error_exitFailure(true, "Failed Writing To Hard Link: %s\n", argv[2]);
    printf("Write To Hard Link: %s\n", argv[2]);
    
    /*** Close Hard Link ***/
    if(close(fd) == -1)
        error_exitFailure(true, "Failed Closing Hard Link: %s\n", argv[2]);
    
    exit(EXIT_SUCCESS);
}
