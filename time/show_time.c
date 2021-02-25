/****************************************************************************************************
 * FILE:    show_time.c
 * BRIEF:   Show Time Source File
 * NOTE:    Example Generating Locale:
 *            Uncomment (Remove #) For Language: sudo geany /etc/locale.gen 
 *            Run The Following:
 *              sudo locale-gen
 *              sudo update-locale
 *              locale -a
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define SHOW_TIME_BUFFER_SIZE (200)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
    char buf[SHOW_TIME_BUFFER_SIZE];
    time_t t;
    struct tm *loc;
    
    /*** Print Example Use ***/
    printf("Example Use:\n");
    printf("  %s\n", argv[0]);
    printf("  TZ=\":Pacific/Auckland\" %s\n", argv[0]);
    printf("  LANG=de_DE %s\n", argv[0]);
    printf("  LANG=de_DE LC_TIME=it_IT %s\n", argv[0]); // LC_TIME Has Precedence Over LANG
    printf("  LC_ALL=fr_FR LC_TIME=en_US %s\n", argv[0]); // LC_ALL Has Precedence Over LC_TIME
    printf("\n");
    
    /*** Set Locale ***/
    if(setlocale(LC_ALL, "") == NULL)
        error_exitFailure(true, "Failed Setting Locale\n");
    
    /*** Get Time ***/
    t = time(NULL);
    loc = localtime(&t);
    if(loc == NULL)
        error_exitFailure(true, "Failed Getting Local Time\n");
    if(strftime(buf, SHOW_TIME_BUFFER_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0)
        error_exitFailure(true, "Failed Converting Input Date Time (strftime)\n");
    
    /*** Print Time ***/
    printf("ctime: %s", ctime(&t));
    printf("asctime Of localtime: %s", asctime(loc));
    printf("strftime Of localtime: %s\n", buf);
    
    exit(EXIT_SUCCESS);
}
