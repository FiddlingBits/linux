/****************************************************************************************************
 * FILE:    realtime_signal_catcher.c
 * BRIEF:   Realtime Signal Catcher Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include "number.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

static volatile bool realtimeSignalCatcher_terminate = false;
static volatile char *realtimeSignalCatcher_programName;
static volatile int realtimeSignalCatcher_handlerSleepSeconds;
static volatile int realtimeSignalCatcher_numSignals = 0;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void realtimeSignalCatcher_signalHandler(int signal, siginfo_t *info, void *context);

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
    int blockSeconds, sig;
    struct sigaction sa;
    sigset_t blockMask, prevMask;
    
    /*** Check Input ***/
    if(argc != 3)
        error_exitFailure(false, "Usage: %s [block_seconds] [handler_sleep_seconds]\n", argv[0]);
    
    /*** Get Variables ***/
    realtimeSignalCatcher_programName = argv[0];
    blockSeconds = number_getLong(argv[1]);
    realtimeSignalCatcher_handlerSleepSeconds = number_getLong(argv[2]);
    
    /*** Set New Signal Handler; Catch Most Signals ***/
    sa.sa_sigaction = realtimeSignalCatcher_signalHandler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);
    for(sig = 1; sig < NSIG; sig++)
    {
        if((sig != SIGTSTP) && (sig != SIGQUIT))
            sigaction(sig, &sa, NULL);
    }
    
    /*** Print Info ***/
    printf("%s:\n", argv[0]);
    printf("  PID: %ld\n", (long)getpid());
    
    /*** Block Signals, If Necessary ***/
    if(blockSeconds > 0)
    {
        /* Set Block Mask */
        sigfillset(&blockMask);
        sigdelset(&blockMask, SIGINT);
        sigdelset(&blockMask, SIGTERM);
        if(sigprocmask(SIG_SETMASK, &blockMask, &prevMask) == -1)
            error_exitFailure(true, "%s: sigprocmask Failed\n", argv[0]);
        
        /* Sleep */
        printf("%s: Signals Blocked; Sleeping %d Seconds\n", argv[0], blockSeconds);
        sleep((unsigned int)blockSeconds);
        
        /* Reset Block Mask */
        if(sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
            error_exitFailure(true, "%s: sigprocmask Failed\n", argv[0]);
        
    }
    
    /*** Wait For Signals ***/
    printf("%s: Wait For Signals\n", argv[0]);
    while(!realtimeSignalCatcher_terminate)
        pause();
    
    /*** Print Info ***/
    printf("%s: Caught %d Signals\n", argv[0], realtimeSignalCatcher_numSignals);
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   realtimeSignalCatcher_signalHandler
 * BRIEF:   Signal Handler
 * RETURN:  void: Returns Nothing
 * ARG:     signal: Signal
 * ARG:     info: Info
 * ARG:     context: Context
 * NOTE:    This Signal Handler Uses Non-Asynchronous Signal-Safe Functions (printf)
 ****************************************************************************************************/
static void realtimeSignalCatcher_signalHandler(int signal, siginfo_t *info, void *context)
{
    char *code;
    
    /*** Terminate Program, If Necessary ***/
    if((signal == SIGINT) || (signal == SIGTERM))
    {
        /* Terminate Program */
        realtimeSignalCatcher_terminate = true;
    }
    else
    {
        /* Print Info */
        realtimeSignalCatcher_numSignals++;
        printf("%s: Caught Signal\n", realtimeSignalCatcher_programName);
        printf("  Signal: %d (%d)\n", signal, info->si_signo);
        switch(info->si_code)
        {
            case SI_QUEUE:
                code = "SI_QUEUE";
                break;
            case SI_USER:
                code = "SI_USER";
                break;
            default:
                code = "OTHER";
                break;
        }
        printf("  Code: %d (%s)\n", info->si_code, code);
        printf("  Value: %d\n", info->si_value.sival_int);
        printf("  From:\n");
        printf("    PID: %ld\n", (long)info->si_pid);
        printf("    UID: %ld\n", (long)info->si_uid);
        
        /* Sleep, If Necessary */
        sleep((unsigned int)realtimeSignalCatcher_handlerSleepSeconds);
    }
}
