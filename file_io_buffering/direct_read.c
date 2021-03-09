/****************************************************************************************************
 * FILE:    direct_read.c
 * BRIEF:   Direct Read Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define _GNU_SOURCE

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <fcntl.h>
#include <malloc.h>
#include <number.h>
#include <stddef.h>
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
    char *buf;
    int fd;
    size_t alignment, length;
    ssize_t numRead;
    off_t offset;
    
    /*** Check Input ***/
    if(argc < 3)
        error_exitFailure(false, "Usage: %s [file] [length] ...\n  [offset]\n  [alignment]\n", argv[0]);
    
    /*** Handle Arguments ***/
    /* Open File */
    fd = open(argv[1], O_RDONLY | O_DIRECT); // O_DIRECT (Bypass Buffer Cache: Transfer Data Directly From User Space To A File Or Disk Device, Or Vice Versa)
    if(fd == -1)
        error_exitFailure(true, "Failed Opening File: %s\n", argv[1]);
    
    /* Length */
    length = (size_t)number_getLong(argv[2]);
    
    /* Offset */
    if(argc >= 4)
        offset = number_getLong(argv[3]);
    else
        offset = 0; // Default
        
    /* Alignment */
    if(argc >= 5)
        alignment = (size_t)number_getLong(argv[5]);
    else
        alignment = 4096; // Default
    
    /*** Allocate Memory ***/
    /* memalign Allocates A Block Of Memory Aligned On An Address That Is A Multiple Of Its First Argument. */
    /* By Specifying This Argument As alignment * 2 And Then Adding alignment To The Returned Pointer, We   */
    /* Ensure That buf Is Aligned On A Non-Power-Of-Two Multiple Of alignment.  We Do This To Ensure That   */
    /* If, For Example, We Ask For A 256-Byte Align Buffer, We Don't Accidently Get A Buffer That Is Also   */
    /* Aligned On A 512-Byte Boundary.                                                                      */
    buf = memalign(alignment * 2, length + alignment);
    if(buf == NULL)
        error_exitFailure(true, "Failed Allocating Memory\n");
    buf += alignment;
    
    /*** Read File ***/
    if(lseek(fd, offset, SEEK_SET) == -1)
        error_exitFailure(true, "Failed Seeking File: %s\n", argv[1]);
    numRead = read(fd, buf, length);
    if(numRead == -1)
        error_exitFailure(true, "Failed Reading File: %s\n", argv[1]);
    printf("Read %zd Bytes\n", numRead);
    
    /*** Close File ***/
    if(close(fd) == -1)
        error_exitFailure(true, "Failed Closing File: %s\n", argv[1]);
    
    exit(EXIT_SUCCESS);
}
