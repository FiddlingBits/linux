/****************************************************************************************************
 * FILE:    signal_catcher.c
 * BRIEF:   Signal Catcher Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void signalCatcher_signalHandler(int signal);

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
    /*** Set New Signal Handler ***/
    if(signal(SIGINT, signalCatcher_signalHandler) == SIG_ERR)
        error_exitFailure(true, "Failed Setting New Signal Handler: %u\n", SIGINT);
    if(signal(SIGQUIT, signalCatcher_signalHandler) == SIG_ERR)
        error_exitFailure(true, "Failed Setting New Signal Handler: %u\n", SIGQUIT);
    
    /*** Infinite Loop ***/
    printf("Press Ctrl + 'C' Or Ctrl + '\\'\n");
    while(1)
        pause(); // Block Until Signal Caught
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   signalCatcher_signalHandler
 * BRIEF:   Signal Handler
 * RETURN:  void: Returns Nothing
 * ARG:     signal: Signal
 * NOTE:    This Signal Handler Uses Non-Asynchronous Signal-Safe Functions (printf, exit)
 ****************************************************************************************************/
static void signalCatcher_signalHandler(int signal)
{
    static int count = 0;
    
    /*** Handle Signal ***/
    switch(signal)
    {
        case SIGINT: // Ctrl + 'C'
            printf("\b\bCaught SIGINT: %d\n", ++count);
            break;
        case SIGQUIT: // Ctrl + '\'
            printf("\b\bCaught SIGQUIT\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            /* Do Nothing */
            break;
    }
}
