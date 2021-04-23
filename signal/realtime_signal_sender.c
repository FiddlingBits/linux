/****************************************************************************************************
 * FILE:    realtime_signal_sender.c
 * BRIEF:   Realtime Signal Sender Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include "number.h"
#include <signal.h>
#include <stdio.h>
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
    int count, data, i, pid, signalNumber;
    union sigval sv;
    
    /*** Check Input ***/
    if(argc != 5)
        error_exitFailure(false, "Usage: %s [pid] [signal_number] [data] [count]\n", argv[0]);

    /*** Get Variables ***/
    pid = number_getLong(argv[1]);
    signalNumber = number_getLong(argv[2]);
    data = number_getLong(argv[3]);
    count = number_getLong(argv[4]);
    
    /*** Print Info ***/
    printf("%s:\n", argv[0]);
    printf("  PID: %ld\n", (long)getpid());
    printf("  UID: %ld\n", (long)getuid());
    
    /*** Send Signals ***/
    for(i = 0; i < count; i++)
    {
        sv.sival_int = data + i;
        if(sigqueue(pid, signalNumber, sv) == -1)
            error_exitFailure(true, "%s: sigqueue Failed: %d\n", argv[0], i);
    }
    
    exit(EXIT_SUCCESS);
}
