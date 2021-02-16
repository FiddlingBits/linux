/****************************************************************************************************
 * FILE:    copy.c
 * BRIEF:   Copy Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#ifndef COPY_BUFFER_SIZE
  #define COPY_BUFFER_SIZE (1024)
#endif

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
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
    char buffer[COPY_BUFFER_SIZE];
    int fdInput, fdOutput, flags;
    mode_t permissions;
    ssize_t numRead;
    
    /*** Check Input ***/
    if(argc != 3)
        error_exitFailure(false, "Usage: %s [old_file] [new_file]\n", argv[0]);
        
    /*** Open Files ***/
    /* Input File */
    fdInput = open(argv[1], O_RDONLY);
    if(fdInput == -1)
        error_exitFailure(true, "Failed Opening Input File: %s\n", argv[1]);
    
    /* Output File */
    flags = O_CREAT | O_WRONLY | O_TRUNC;
    permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // rw-rw-rw-
    fdOutput = open(argv[2], flags, permissions);
    if(fdOutput == -1)
        error_exitFailure(true, "Failed Opening Output File: %s\n", argv[2]);
    
    /*** Copy Contents Of Input File To Output File ***/
    while((numRead = read(fdInput, buffer, sizeof(buffer))) > 0)  // read Changes File Offset; Use pread To Not Modify File Offset
    {
        if(write(fdOutput, buffer, (size_t)numRead) != numRead) // write Changes File Offset; Use pwrite To Not Modify File Offset
            error_exitFailure(true, "Failed Writing Output File: %s\n", argv[2]);
            
    }
    if(numRead == -1)
        error_exitFailure(true, "Failed Reading Input File: %s\n", argv[1]);
    
    /*** Close Files ***/
    if(close(fdInput) == -1)
        error_exitFailure(true, "Failed Closing Input File: %s\n", argv[1]);
    if(close(fdOutput) == -1)
        error_exitFailure(true, "Failed Closing Output File: %s\n", argv[2]);
    
    exit(EXIT_SUCCESS);
}
