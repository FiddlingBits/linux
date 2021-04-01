/****************************************************************************************************
 * FILE:    signal_sender.c
 * BRIEF:   Signal Sender Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <errno.h>
#include "error.h"
#include "number.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

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
    int pid, signalNumber, status;
    
    /*** Check Input ***/
    if(argc != 3)
        error_exitFailure(false, "Usage: %s [pid] [signal_number]\n  Use kill -l For Signal List\n", argv[0]);
    
    /*** Get Variables ***/
    pid = number_getLong(argv[1]);
    signalNumber = number_getLong(argv[2]);
    
    /*** Send Signal ***/
    status = kill(pid, signalNumber);
    
    /*** Error Check ***/
    if((signalNumber != 0) && (status == -1))
        error_exitFailure(true, "kill Failed: pid = %d, signal_number = %d\n", pid, signalNumber);
        
    /*** Null Signal: Process Existence Check ***/
    if(signalNumber == 0)
        printf("Process Exists And Can Be Sent A Signal\n");
    else if(errno == EPERM)
        printf("Process Exists But This Process Does Not Have Permission To Send It A Signal\n");
    else if(errno == ESRCH)
        printf("Process Does Not Exist\n");
    else
        error_exitFailure(true, "kill Failed: pid = %d, signal_number = %d\n", pid, signalNumber);
    
    exit(EXIT_SUCCESS);
}
