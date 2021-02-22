/****************************************************************************************************
 * FILE:    check_password.c
 * BRIEF:   Check Password Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <errno.h>
#include "error.h"
#include <pwd.h>
#include <shadow.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    bool match;
    char *c, *encryptedPassword, *password, *userName;
    long loginNameMax;
    size_t len;
    struct passwd *passwordRecord;
    struct spwd *shadowPasswordRecord;
    
    /*** Set Up ***/
    /* User Name */
    loginNameMax = sysconf(_SC_LOGIN_NAME_MAX);
    if(loginNameMax == -1) // Check If Limit Is Indeterminate
        loginNameMax = 256;
    userName = malloc((unsigned int)loginNameMax);
    if(userName == NULL)
        error_exitFailure(true, "Failed Allocating Memory\n");
    
    /*** Get User Name ***/
    /* Get User Name */
    printf("User Name: ");
    fflush(stdout);
    if(fgets(userName, loginNameMax, stdin) == NULL)
        error_exitFailure(false, "Failed Getting User Name\n");
    
    /* Remove Newline, If Necessary */
    len = strlen(userName);
    if(userName[len - 1] == '\n')
        userName[len - 1] = '\0';
    
    /*** Get Password Record ***/
    /* Get Password Record */
    passwordRecord = getpwnam(userName);
    if(passwordRecord == NULL)
        error_exitFailure(false, "Failed Getting Password Record\n");
    
    /* Get Shadow Password Record */
    shadowPasswordRecord = getspnam(userName);
    if((shadowPasswordRecord == NULL) && (errno == EACCES))
        error_exitFailure(false, "Don't Have Permission To Read Shadow Password Record\n");
    if(shadowPasswordRecord != NULL)
        passwordRecord->pw_passwd = shadowPasswordRecord->sp_pwdp; // Use Shadow Password
    
    /*** Get Password ***/
    password = getpass("Password: ");
    
    /*** Encrypt Password And Erase Clear Text Version Immediately ***/
    encryptedPassword = crypt(password, passwordRecord->pw_passwd);
    for(c = password; *c != '\0'; c++)
        *c = '\0';
    if(encryptedPassword == NULL)
        error_exitFailure(true, "Failed To Encrypt Password\n");
    
    /*** Verify Encrypted Passwords Match ***/
    match = (strcmp(encryptedPassword, passwordRecord->pw_passwd) == 0);
    if(!match)
        error_exitFailure(false, "Incorrect Password!\n");
    printf("Successfully Authenticated User: UID = %ld\n", (long)passwordRecord->pw_uid);
    
    /*** Clean Up ***/
    free(userName);
    
    exit(EXIT_SUCCESS);
}
