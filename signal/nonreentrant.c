/****************************************************************************************************
 * FILE:    nonreentrant.c
 * BRIEF:   Non-Reentrant Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

static char *nonreentrant_str2;
static int nonreentrant_handled = 0;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void nonreentrant_signalHandler(int signal);

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
    char *encryptedString;
    int callNum = 0, mismatch = 0;
    struct sigaction sa;

    /*** Check Input ***/
    if(argc != 3)
        error_exitFailure(false, "Usage: %s [str1] [str2]\n", argv[0]);
    
    /*** Encrypt argv[1] ***/
    encryptedString = strdup(crypt(argv[1], "xx"));
    if(encryptedString == NULL)
        error_exitFailure(true, "crypt Failed\n");
    
    /*** Make argv[2] Available To Signal Handler ***/
    nonreentrant_str2 = argv[2];
    
    /*** Set New Signal Handler ***/
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = nonreentrant_signalHandler;
    if(sigaction(SIGINT, &sa, NULL) == -1)
        error_exitFailure(true, "sigaction Failed\n");
    
    /*** Infinite Loop ***/
    /* Repeatedly Call crypt() Using argv[1].  If Interrupted By Signal Handler, Then The   */
    /* Static Storage Returned By crypt()Will Be Overwritten By The Results Of Encrypting   */
    /* argv[2] And strcmp() Will Detect A Mismatch Between The Actual And Expected Results. */
    printf("Press Ctrl + C To Test; Press Ctrl + \\ To Quit\n");
    while(1)
    {
        /* Increment Counter */
        callNum++;
        
        /* Encrypt argv[1]; Compare To Expected Result */
        if(strcmp(crypt(argv[1], "xx"), encryptedString) != 0) // argv[1] (Key); "xx" (Salt)
        {
            /* Increment Counter */
            mismatch++;
            
            /* Print Status */
            printf("Status:\n");
            printf("  Call Number: %d\n", callNum);
            printf("  Mismatch: %d\n", mismatch);
            printf("  Handled: %d\n", nonreentrant_handled);
        }
    }
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   nonreentrant_signalHandler
 * BRIEF:   Signal Handler
 * RETURN:  void: Returns Nothing
 * ARG:     signal: Signal
 * NOTE:    This Signal Handler Uses Non-Asynchronous Signal-Safe Function (crypt)
 ****************************************************************************************************/
static void nonreentrant_signalHandler(int signal)
{
    /*** Encrypt nonreentrant_str2 (argv[2]) ***/
    printf("\b\b  \b\b");
    fflush(stdout);
    (void)crypt(nonreentrant_str2, "xx"); // nonreentrant_str2 (Key); "xx" (Salt)
    nonreentrant_handled++;
}

