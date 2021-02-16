/****************************************************************************************************
 * FILE:    scatter_read_write.c
 * BRIEF:   Scatter Read/Write Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define SCATTER_READ_WRITE_STRING_SIZE (100)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

/****************************************************************************************************
 * Type Definitions
 ****************************************************************************************************/

typedef struct scatterReadWrite_data_s
{
    double e, pi;
} scatterReadWrite_data_t;

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
    char option, string[SCATTER_READ_WRITE_STRING_SIZE] = {'\0'};
    int fd, flags;
    unsigned int number;
    struct iovec iov[3];
    mode_t permissions;
    scatterReadWrite_data_t data;
    ssize_t actualLength, expectedLength;
    
    /*** Check Input ***/
    if(argc != 3)
        error_exitFailure(false, "Usage: %s [file] [r|w]\n", argv[0]);
        
    /*** Open File ***/
    flags = O_CREAT | O_RDWR;
    permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // rw-rw-rw-
    fd = open(argv[1], flags, permissions);
    if(fd == -1)
        error_exitFailure(true, "Failed Opening File: %s\n", argv[1]);
    
    /*** Populate I/O Vector Structures ***/
    iov[0].iov_base = &data;
    iov[0].iov_len = sizeof(data);
    iov[1].iov_base = &number;
    iov[1].iov_len = sizeof(number);
    iov[2].iov_base = &string;
    iov[2].iov_len = sizeof(string);
    expectedLength = (ssize_t)(iov[0].iov_len + iov[1].iov_len + iov[2].iov_len);
    
    /*** Handle Option ***/
    option = argv[2][0];
    switch(option)
    {
        case 'r':
            /* Read Data */
            actualLength = readv(fd, iov, 3); // readv Changes File Offset; Use preadv To Not Modify File Offset
            if(actualLength == -1)
                error_exitFailure(true, "Failed Reading File: %s\n", argv[1]);
            
            /* Print Data */
            printf("data.e = %f\n", data.e);
            printf("data.pi = %f\n", data.pi);
            printf("number = %u (0x%08X)\n", number, number);
            printf("string = %s (0x", string);
            for(int i = 0; i < (int)sizeof(string); i++)
                printf("%02X", string[i]);
            printf(")\n");
            break;
        case 'w':
            /* Generate Data */
            data.e = 2.71828;
            data.pi = 3.14159;
            number = 0xDEADBEEF;
            strcpy(string, "This is a string!");
            
            /* Write Data */
            actualLength = writev(fd, iov, 3); // writev Changes File Offset; Use pwritev To Not Modify File Offset
            if(actualLength == -1)
                error_exitFailure(true, "Failed Writing File: %s\n", argv[1]);
            if(actualLength != expectedLength)
                printf("Wrote Less Bytes Than Expected: %u != %u\n", actualLength, expectedLength);
            break;
        default:
            printf("Unhandled Option: %c\n", option);
            break;
    }
    
    /*** Close File ***/
    if(close(fd) == -1)
        error_exitFailure(true, "Failed Closing File: %s\n", argv[1]);
    
    exit(EXIT_SUCCESS);
}
