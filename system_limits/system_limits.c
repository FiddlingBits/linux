/****************************************************************************************************
 * FILE:    system_limits.c
 * BRIEF:   System Limits Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <errno.h>
#include "error.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void systemLimits_fpathconfPrint(const char *msg, int fd, int name);
static void systemLimits_pathconfPrint(const char *msg, const char *pathname, int name);
static void systemLimits_sysconfPrint(const char *msg, int name);

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
    int i;
    
    /*** System Limits ***/
    printf("System Limits:\n");
    systemLimits_sysconfPrint("  _SC_ARG_MAX", _SC_ARG_MAX);
    systemLimits_sysconfPrint("  _SC_LOGIN_NAME_MAX", _SC_LOGIN_NAME_MAX);
    systemLimits_sysconfPrint("  _SC_NGROUPS_MAX", _SC_NGROUPS_MAX);
    systemLimits_sysconfPrint("  _SC_OPEN_MAX", _SC_OPEN_MAX);
    systemLimits_sysconfPrint("  _SC_PAGESIZE", _SC_PAGESIZE);
    systemLimits_sysconfPrint("  _SC_RTSIG_MAX", _SC_RTSIG_MAX);
    
    /*** File-Related System Limits ***/
    /* Pathname */
    for(i = 1; i < argc; i++)
    {
        printf("File-Related System Limits (%s):\n", argv[i]);
        systemLimits_pathconfPrint("  _PC_NAME_MAX", argv[i], _PC_NAME_MAX);
        systemLimits_pathconfPrint("  _PC_PATH_MAX", argv[i], _PC_PATH_MAX);
        systemLimits_pathconfPrint("  _PC_PIPE_BUF", argv[i], _PC_PIPE_BUF);
    }
    
    /* File Descriptor */
    printf("File-Related System Limits (STDIN_FILENO):\n");
    systemLimits_fpathconfPrint("  _PC_NAME_MAX", STDIN_FILENO, _PC_NAME_MAX);
    systemLimits_fpathconfPrint("  _PC_PATH_MAX", STDIN_FILENO, _PC_PATH_MAX);
    systemLimits_fpathconfPrint("  _PC_PIPE_BUF", STDIN_FILENO, _PC_PIPE_BUF);
    
    exit(EXIT_SUCCESS);
}


/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   systemLimits_fpathconfPrint
 * BRIEF:   Print fpathconf Attribute Limit
 * RETURN:  void: Returns Nothing
 * ARG:     msg: Message To Print With Limit
 * ARG:     fd: File Descriptor
 * ARG:     name: Name Of fpathconf Attribute To Get Limit Of
 ****************************************************************************************************/
static void systemLimits_fpathconfPrint(const char *msg, int fd, int name)
{
    long limit;
    
    /*** Print fpathconf Attribute Limit ***/
    /* Set Up */
    errno = 0; // We Must Set This To 0 To Distinguish Between -1 Meaning Indeterminate And -1 Meaning Error
    
    /* Get fpathconf Attribute Limit */
    limit = fpathconf(fd, name);
    
    /* Print Limit On Success Or Exit On Failure */
    if(limit != -1)
    {
        printf("%s: %ld\n", msg, limit);
    }
    else
    {
        if(errno == 0)
            printf("%s: Indeterminate\n", msg);
        else
            error_exitFailure(true, "Failed Getting fpathconf Attribute Limit\n");
    }
}

/****************************************************************************************************
 * FUNCT:   systemLimits_pathconfPrint
 * BRIEF:   Print pathconf Attribute Limit
 * RETURN:  void: Returns Nothing
 * ARG:     msg: Message To Print With Limit
 * ARG:     pathname: Pathname
 * ARG:     name: Name Of pathconf Attribute To Get Limit Of
 ****************************************************************************************************/
static void systemLimits_pathconfPrint(const char *msg, const char *pathname, int name)
{
    long limit;
    
    /*** Print pathconf Attribute Limit ***/
    /* Set Up */
    errno = 0; // We Must Set This To 0 To Distinguish Between -1 Meaning Indeterminate And -1 Meaning Error
    
    /* Get pathconf Attribute Limit */
    limit = pathconf(pathname, name);
    
    /* Print Limit On Success Or Exit On Failure */
    if(limit != -1)
    {
        printf("%s: %ld\n", msg, limit);
    }
    else
    {
        if(errno == 0)
            printf("%s: Indeterminate\n", msg);
        else
            error_exitFailure(true, "Failed Getting pathconf Attribute Limit\n");
    }
}

/****************************************************************************************************
 * FUNCT:   systemLimits_sysconfPrint
 * BRIEF:   Print sysconf Attribute Limit
 * RETURN:  void: Returns Nothing
 * ARG:     msg: Message To Print With Limit
 * ARG:     name: Name Of sysconf Attribute To Get Limit Of
 ****************************************************************************************************/
static void systemLimits_sysconfPrint(const char *msg, int name)
{
    long limit;
    
    /*** Print sysconf Attribute Limit ***/
    /* Set Up */
    errno = 0; // We Must Set This To 0 To Distinguish Between -1 Meaning Indeterminate And -1 Meaning Error
    
    /* Get sysconf Attribute Limit */
    limit = sysconf(name);
    
    /* Print Limit On Success Or Exit On Failure */
    if(limit != -1)
    {
        printf("%s: %ld\n", msg, limit);
    }
    else
    {
        if(errno == 0)
            printf("%s: Indeterminate\n", msg);
        else
            error_exitFailure(true, "Failed Getting sysconf Attribute Limit\n");
    }
}
