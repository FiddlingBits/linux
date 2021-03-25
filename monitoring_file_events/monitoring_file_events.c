/****************************************************************************************************
 * FILE:    monitoring_file_events.c
 * BRIEF:   Monitoring File Events Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "error.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void monitoringFileEvents_displayInotifyEvent(struct inotify_event *event);

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
    char buf[10 * (sizeof(struct inotify_event) + NAME_MAX + 1)]; __attribute__((aligned(8))) // 1 (Terminating '\0')
    char *p;
    int fd, wd;
    ssize_t numRead;
    struct inotify_event *event;
    
    /*** Check Input ***/
    if(argc != 2)
        error_exitFailure(false, "Usage: %s [directory_or_file_pathname]\n", argv[0]);
    
    /*** Create inotify Instance ***/
    fd = inotify_init();
    if(fd == -1)
        error_exitFailure(true, "Error Creating inotify Instance\n");
    
    /*** Add inotify Watch ***/
    wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    if(wd == -1)
        error_exitFailure(true, "Error Adding inotify Watch: %s\n", argv[1]);
    
    /*** Infinite Loop ***/
    printf("Monitoring %s...\n", argv[1]);
    while(1)
    {
        /* Wait For Event(s) */
        numRead = read(fd, buf, sizeof(buf));
        if((numRead == 0) || (numRead == -1))
            error_exitFailure(true, "inotify Read Failed!\n");
        printf("Read %zd Bytes From inotify\n", numRead);
        
        /* Process Event(s) */
        for(p = buf; p < (buf + numRead); p += (sizeof(struct inotify_event) + event->len))
        {
            event = (struct inotify_event *)p;
            monitoringFileEvents_displayInotifyEvent(event);
        }
        
    }
    
    exit(EXIT_SUCCESS);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   monitoringFileEvents_displayInotifyEvent
 * BRIEF:   Display inotify Event
 * RETURN:  void: Returns Nothing
 * ARG:     event: inotify Event To Display
 ****************************************************************************************************/
static void monitoringFileEvents_displayInotifyEvent(struct inotify_event *event)
{
    /*** Display inotify Event ***/
    /* Watch Descriptor */
    printf("  wd: %d; ", event->wd);
    
    /* Cookie */
    if(event->cookie > 0)
        printf("cookie: %d; ", event->cookie);
    
    /* Mask */
    printf("mask: ");
    if(event->mask & IN_ACCESS)
        printf("IN_ACCESS, ");
    if(event->mask & IN_ATTRIB)
        printf("IN_ATTRIB, ");
    if(event->mask & IN_CLOSE_NOWRITE)
        printf("IN_CLOSE_NOWRITE, ");
    if(event->mask & IN_CLOSE_WRITE)
        printf("IN_CLOSE_WRITE, ");
    if(event->mask & IN_CREATE)
        printf("IN_CREATE, ");
    if(event->mask & IN_DELETE)
        printf("IN_DELETE, ");
    if(event->mask & IN_DELETE_SELF)
        printf("IN_DELETE_SELF, ");
    if(event->mask & IN_IGNORED)
        printf("IN_IGNORED, ");
    if(event->mask & IN_ISDIR)
        printf("IN_ISDIR, ");
    if(event->mask & IN_MODIFY)
        printf("IN_MODIFY, ");
    if(event->mask & IN_MOVE_SELF)
        printf("IN_MOVE_SELF, ");
    if(event->mask & IN_MOVED_FROM)
        printf("IN_MOVED_FROM, ");
    if(event->mask & IN_MOVED_TO)
        printf("IN_MOVED_TO, ");
    if(event->mask & IN_OPEN)
        printf("IN_OPEN, ");
    if(event->mask & IN_Q_OVERFLOW)
        printf("IN_Q_OVERFLOW, ");
    if(event->mask & IN_UNMOUNT)
        printf("IN_UNMOUNT, ");
    printf("\b\b; ");
    
    /* Name */
    if(event->len > 0)
        printf("name = %s;", event->name);
    printf("\n");
}
