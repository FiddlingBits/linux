/****************************************************************************************************
 * FILE:    number.c
 * BRIEF:   Number Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <errno.h>
#include "error.h"
#include "number.h"
#include <stdbool.h>
#include <stdlib.h>

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   number_getLong
 * BRIEF:   Convert String Long To Decimal Long
 * RETURN:  long: Decimal Long
 * ARG:     stringLong: String Long
 ****************************************************************************************************/
long number_getLong(const char *stringLong)
{
    char *endPtr;
    long result;
    
    /*** Convert String Long To Decimal Long ***/
    errno = 0;
    result = strtol(stringLong, &endPtr, 0); // 0 (Base Determined By Contents Of stringLong)
    
    /*** Error Check ***/
    if(errno != 0)
        error_exitFailure(true, "strtol Failed: %s\n", stringLong);
    if(*endPtr != '\0')
        error_exitFailure(false, "Non-Numeric Characters: %s\n", stringLong);
    
    return result;
}
