/****************************************************************************************************
 * FILE:    seek.c
 * BRIEF:   Seek Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <ctype.h>
#include "error.h"
#include <fcntl.h>
#include "number.h"
#include <stdint.h>
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
    char option;
    uint8_t *buffer;
    const char *argument;
    int fd, flags;
    mode_t permissions;
    off_t offset = 0;
    size_t length;
    ssize_t numRead, numWritten;
    
    /*** Check Input ***/
    if(argc < 3)
        error_exitFailure(false, "Usage: %s [file] ...\n  [r<length>]\n  [R<length>]\n  [w<string>]\n  [s<offset>]\n", argv[0]);
    
    /*** Open File ***/
    flags = O_CREAT | O_RDWR;
    permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // rw-rw-rw-
    fd = open(argv[1], flags, permissions);
    if(fd == -1)
        error_exitFailure(true, "Failed Opening File: %s\n", argv[1]);
    
    /*** Handle Arguments ***/
    for(int i = 2; i < argc; i++)
    {
        /* Parse Argument */
        option = argv[i][0];
        argument = &argv[i][1];
        
        /* Handle Argument */
        switch(option)
        {
            case 'r': // Display Bytes At Current Offset As Text
            case 'R': // Display Bytes At Current Offset As Hex
                /* Get Length */
                length = (size_t)number_getLong(argument);
                
                /* Allocate Memory */
                buffer = malloc(length);
                if(buffer == NULL)
                    error_exitFailure(true, "Failed Allocating Memory: %d Bytes\n", length);
                
                /* Read Bytes */
                numRead = read(fd, buffer, length);
                if(numRead == -1)
                    error_exitFailure(true, "Failed Reading File: %s\n", argv[1]);
                if(numRead == 0)
                {
                    printf("End-Of-File\n");
                }
                else
                {
                    printf("Read (%ld): ", offset);
                    for(int j = 0; j < numRead; j++)
                    {
                        if(option == 'r')
                            printf("%c", isprint(buffer[j]) ? buffer[j] : '?');
                        else
                            printf("%02X ", buffer[j]);
                    }
                    printf("\n");
                }
                
                /* Clean Up */
                free(buffer);
                break;
            case 's': // Change File Offset
                /* Change File Offset */
                offset = number_getLong(argument);
                if(lseek(fd, offset, SEEK_SET) == -1)
                    error_exitFailure(true, "Failed Seeking File: %s\n", argv[1]);
                printf("Seek (%ld)\n", offset);
                break;
            case 'w': // Write Bytes At Current Offset
                /* Write Bytes */
                numWritten = write(fd, argument, strlen(argument));
                if(numWritten == -1)
                    error_exitFailure(true, "Failed Writing File: %s\n", argv[1]);
                printf("Write (%ld): %s\n", offset, argument);
                break;
            default:
                printf("Unhandled Option: %c\n", option);
                break;
        }
    }
    
    /*** Close File ***/
    if(close(fd) == -1)
        error_exitFailure(true, "Failed Closing File: %s\n", argv[1]);
    
    exit(EXIT_SUCCESS);
}
