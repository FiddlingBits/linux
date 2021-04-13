/****************************************************************************************************
 * FILE:    signal_handler_jump.c
 * BRIEF:   Signal Handler Jump Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

static sigjmp_buf senv;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void signalHandlerJump_signalHandler(int signal);

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
int main(int argc, char *argv[])
{
    static bool first = true;
    struct sigaction sa;
    
    /*** Set New Signal Handler ***/
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = signalHandlerJump_signalHandler;
    if(sigaction(SIGINT, &sa, NULL) == -1)
        error_exitFailure(true, "sigaction Failed\n");
    
    /*** Set Jump ***/
    if(sigsetjmp(senv, 1) == 0)
        printf("Set Jump\n");
    else
        printf("Jumped From Signal Handler\n");
    
    /*** Infinite Loop ***/
    if(first)
    {
        printf("Press Ctrl + C To Test; Press Ctrl + \\ To Quit\n");
        first = false;
    }
    while(1)
        continue;
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   signalHandlerJump_signalHandler
 * BRIEF:   Signal Handler
 * RETURN:  void: Returns Nothing
 * ARG:     signal: Signal
 * NOTE:    This Signal Handler Uses Non-Asynchronous Signal-Safe Function (printf)
 ****************************************************************************************************/
static void signalHandlerJump_signalHandler(int signal)
{
    /*** Perform Jump ***/
    printf("\b\b  \b\b");
    fflush(stdout);
    siglongjmp(senv, 1);
}
