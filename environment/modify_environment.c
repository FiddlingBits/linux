/****************************************************************************************************
 * FILE:    modify_environment.c
 * BRIEF:   Modify Environment Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

extern char **environ;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void modifyEnvironment_printEnvironmentVariables(void);

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
    int i;
    
    /*** Print Environment Variables ***/
    modifyEnvironment_printEnvironmentVariables();
    
    /*** Clear Environment Variables ***/
    printf("Clear Environment Variables\n");
    clearenv();
    modifyEnvironment_printEnvironmentVariables();
    
    /*** Put User Supplied Environment Variables ***/
    printf("Put User Supplied Environment Variables\n");
    for(i = 1; i < argc; i++)
    {
        if(putenv(argv[i]) != 0)
            error_exitFailure(true, "Error Putting User Supplied Environment Variables\n");
    }
    modifyEnvironment_printEnvironmentVariables();
    
    /*** Set Environment Variable ***/
    printf("Set Environment Variable\n");
    if(setenv("GREET", "Hello world!", 0) == -1) // 0 (Don't Overwrite Variable If It Already Exists)
        error_exitFailure(true, "Error Setting Environment Variable\n");
    modifyEnvironment_printEnvironmentVariables();
    
    /*** Unset Environment Variable ***/
    printf("Unset Environment Variable\n");
    if(unsetenv("BYE") == -1)
        error_exitFailure(true, "Error Unsetting Environment Variable\n");
    modifyEnvironment_printEnvironmentVariables();
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   modifyEnvironment_printEnvironment
 * BRIEF:   Print Environment Variables
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
static void modifyEnvironment_printEnvironmentVariables(void)
{
    char **env;
    
    /*** Print Environment Variables ***/
    printf("Environment Variables:\n");
    if(environ != NULL)
    {
        for(env = environ; *env != NULL; env++)
            printf("  %s\n", *env);
    }
    printf("\n");
}
