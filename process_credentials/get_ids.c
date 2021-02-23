/****************************************************************************************************
 * FILE:    get_ids.c
 * BRIEF:   Get IDs Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#define _GNU_SOURCE // For getresuid And getresgid

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fsuid.h>
#include <unistd.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static char *getIds_getGroupNameFromId(gid_t gid);
static char *getIds_getUserNameFromId(uid_t uid);

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
    char *name;
    int fileSystemGid, fileSystemUid, i, numGroups;
    uid_t effectiveUid, realUid, savedSetUid;
    gid_t effectiveGid, realGid, savedSetGid, supplementaryGroups[NGROUPS_MAX];
    
    /*** Get IDs ***/
    /* Get User IDs */
    if(getresuid(&realUid, &effectiveUid, &savedSetUid) == -1)
        error_exitFailure(true, "Failed Getting User IDs\n");
    fileSystemUid = setfsuid(0); // Attempts To Change File System ID Is Always Ignored For Unprivileged Processes, However, The Following Call Returns The Current File System ID
    
    /* Get Group IDs */
    if(getresgid(&realGid, &effectiveGid, &savedSetGid) == -1)
        error_exitFailure(true, "Failed Getting Group IDs\n");
    fileSystemGid = setfsgid(0); // Attempts To Change File System ID Is Always Ignored For Unprivileged Processes, However, The Following Call Returns The Current File System ID
    
    /*** Print IDs ***/
    /* User IDs */
    printf("UIDs:\n");
    name = getIds_getUserNameFromId(realUid);
    printf("  Real: %s (%u)\n", name, realUid);
    name = getIds_getUserNameFromId(effectiveUid);
    printf("  Effective: %s (%u)\n", name, effectiveUid);
    name = getIds_getUserNameFromId(savedSetUid);
    printf("  Saved Set: %s (%u)\n", name, savedSetUid);
    name = getIds_getUserNameFromId((uid_t)fileSystemUid);
    printf("  File System: %s (%d)\n", name, fileSystemUid);
    
    /* Group IDs */
    printf("GIDs:\n");
    name = getIds_getGroupNameFromId(realGid);
    printf("  Real: %s (%u)\n", name, realGid);
    name = getIds_getGroupNameFromId(effectiveGid);
    printf("  Effective: %s (%u)\n", name, effectiveGid);
    name = getIds_getGroupNameFromId(savedSetGid);
    printf("  Saved Set: %s (%u)\n", name, savedSetGid);
    name = getIds_getGroupNameFromId((gid_t)fileSystemGid);
    printf("  File System: %s (%d)\n", name, fileSystemGid);
    
    /*** Print Supplementary Groups ***/
    numGroups = getgroups(NGROUPS_MAX, supplementaryGroups);
    if(numGroups == -1)
        error_exitFailure(true, "Failed Getting Supplementary Groups\n");
    printf("Supplementary Groups:\n");
    for(i = 0; i < numGroups; i++)
    {
        name = getIds_getGroupNameFromId(supplementaryGroups[i]);
        printf("  %s (%d)\n", name, supplementaryGroups[i]);
    }
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   getIds_getGroupNameFromId
 * BRIEF:   Get Group Name From ID
 * RETURN:  char *: Group Name
 * ARG:     gid: Group ID
 ****************************************************************************************************/
static char *getIds_getGroupNameFromId(gid_t gid)
{
    struct group *groupRecord;
    
    /*** Get Group Name From ID ***/
    groupRecord = getgrgid(gid);
    return (groupRecord == NULL) ? "UNKNOWN" : groupRecord->gr_name;
}

/****************************************************************************************************
 * FUNCT:   getIds_getUserNameFromId
 * BRIEF:   Get User Name From ID
 * RETURN:  char *: User Name
 * ARG:     uid: User ID
 ****************************************************************************************************/
static char *getIds_getUserNameFromId(uid_t uid)
{
    struct passwd *passwordRecord;
    
    /*** Get User Name From ID ***/
    passwordRecord = getpwuid(uid);
    return (passwordRecord == NULL) ? "UNKNOWN" : passwordRecord->pw_name;
}
