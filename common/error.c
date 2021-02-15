/****************************************************************************************************
 * FILE:    error.c
 * BRIEF:   Error Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <errno.h>
#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

/*** Error Strings (Produced From "errno -l" Output) ***/
static const char *error_string[] =
{
    "No Error (0)",
    "EPERM (1): Operation Not Permitted",
    "ENOENT (2): No Such File Or Directory",
    "ESRCH (3): No Such Process",
    "EINTR (4): Interrupted System Call",
    "EIO (5): Input/Output Error",
    "ENXIO (6): No Such Device Or Address",
    "E2BIG (7): Argument List Too Long",
    "ENOEXEC (8): Exec Format Error",
    "EBADF (9): Bad File Descriptor",
    "ECHILD (10): No Child Processes",
    "EAGAIN (11): Resource Temporarily Unavailable",
    "ENOMEM (12): Cannot Allocate Memory",
    "EACCES (13): Permission Denied",
    "EFAULT (14): Bad Address",
    "ENOTBLK (15): Block Device Required",
    "EBUSY (16): Device Or Resource Busy",
    "EEXIST (17): File Exists",
    "EXDEV (18): Invalid Cross-Device Link",
    "ENODEV (19): No Such Device",
    "ENOTDIR (20): Not A Directory",
    "EISDIR (21): Is A Directory",
    "EINVAL (22): Invalid Argument",
    "ENFILE (23): Too Many Open Files In System",
    "EMFILE (24): Too Many Open Files",
    "ENOTTY (25): Inappropriate ioctl For Device",
    "ETXTBSY (26): Text File Busy",
    "EFBIG (27): File Too Large",
    "ENOSPC (28): No Space Left On Device",
    "ESPIPE (29): Illegal Seek",
    "EROFS (30): Read-Only File System",
    "EMLINK (31): Too Many Links",
    "EPIPE (32): Broken Pipe",
    "EDOM (33): Numerical Argument Out Of Domain",
    "ERANGE (34): Numerical Result Out Of Range",
    "EDEADLK (35): Resource Deadlock Avoided",
    "ENAMETOOLONG (36): File Name Too Long",
    "ENOLCK (37): No Locks Available",
    "ENOSYS (38): Function Not Implemented",
    "ENOTEMPTY (39): Directory Not Empty",
    "ELOOP (40): Too Many Levels Of Symbolic Links",
    "EWOULDBLOCK (41): Operation Would Block",
    "ENOMSG (42): No Message Of Desired Type",
    "EIDRM (43): Identifier Removed",
    "ECHRNG (44): Channel Number Out Of Range",
    "EL2NSYNC (45): Level 2 Not Synchronized",
    "EL3HLT (46): Level 3 Halted",
    "EL3RST (47): Level 3 Reset",
    "ELNRNG (48): Link Number Out Of Range",
    "EUNATCH (49): Protocol Driver Not Attached",
    "ENOCSI (50): No CSI Structure Available",
    "EL2HLT (51): Level 2 Halted",
    "EBADE (52): Invalid Exchange",
    "EBADR (53): Invalid Request Descriptor",
    "EXFULL (54): Exchange Full",
    "ENOANO (55): No Anode",
    "EBADRQC (56): Invalid Request Code",
    "EBADSLT (57): Invalid Slot",
    "EDEADLOCK (58): Resource Deadlock Avoided",
    "EBFONT (59): Bad Font File Format",
    "ENOSTR (60): Device Not A Stream",
    "ENODATA (61): No Data Available",
    "ETIME (62): Timer Expired",
    "ENOSR (63): Out Of Streams Resources",
    "ENONET (64): Machine Is Not On The Network",
    "ENOPKG (65): Package Not Installed",
    "EREMOTE (66): Object Is Remote",
    "ENOLINK (67): Link Has Been Severed",
    "EADV (68): Advertise Error",
    "ESRMNT (69): Srmount Error",
    "ECOMM (70): Communication Error On Send",
    "EPROTO (71): Protocol Error",
    "EMULTIHOP (72): Multihop Attempted",
    "EDOTDOT (73): RFS Specific Error",
    "EBADMSG (74): Bad Message",
    "EOVERFLOW (75): Value Too Large For Defined Data Type",
    "ENOTUNIQ (76): Name Not Unique On Network",
    "EBADFD (77): File Descriptor In Bad State",
    "EREMCHG (78): Remote Address Changed",
    "ELIBACC (79): Can Not Access A Needed Shared Library",
    "ELIBBAD (80): Accessing A Corrupted Shared Library",
    "ELIBSCN (81): .lib Section In a.out Corrupted",
    "ELIBMAX (82): Attempting To Link In Too Many Shared Libraries",
    "ELIBEXEC (83): Cannot exec A Shared Library Directly",
    "EILSEQ (84): Invalid Or Incomplete Multibyte Or Wide Character",
    "ERESTART (85): Interrupted System Call Should Be Restarted",
    "ESTRPIPE (86): Streams Pipe Error",
    "EUSERS (87): Too Many Users",
    "ENOTSOCK (88): Socket Operation On Non-Socket",
    "EDESTADDRREQ (89): Destination Address Required",
    "EMSGSIZE (90): Message Too Long",
    "EPROTOTYPE (91): Protocol Wrong Type For Socket",
    "ENOPROTOOPT (92): Protocol Not Available",
    "EPROTONOSUPPORT (93): Protocol Not Supported",
    "ESOCKTNOSUPPORT (94): Socket Type Not Supported",
    "EOPNOTSUPP (95): Operation Not Supported",
    "EPFNOSUPPORT (96): Protocol Family Not Supported",
    "EAFNOSUPPORT (97): Address Family Not Supported By Protocol",
    "EADDRINUSE (98): Address Already In Use",
    "EADDRNOTAVAIL (99): Cannot Assign Requested Address",
    "ENETDOWN (100): Network Is Down",
    "ENETUNREACH (101): Network Is Unreachable",
    "ENETRESET (102): Network Dropped Connection On Reset",
    "ECONNABORTED (103): Software Caused Connection Abort",
    "ECONNRESET (104): Connection Reset By Peer",
    "ENOBUFS (105): No Buffer Space Available",
    "EISCONN (106): Transport Endpoint Is Already Connected",
    "ENOTCONN (107): Transport Endpoint Is Not Connected",
    "ESHUTDOWN (108): Cannot Send After Transport Endpoint Shutdown",
    "ETOOMANYREFS (109): Too Many References: Cannot Splice",
    "ETIMEDOUT (110): Connection Timed Out",
    "ECONNREFUSED (111): Connection Refused",
    "EHOSTDOWN (112): Host Is Down",
    "EHOSTUNREACH (113): No Route To Host",
    "EALREADY (114): Operation Already In Progress",
    "EINPROGRESS (115): Operation Now In Progress",
    "ESTALE (116): Stale File Handle",
    "EUCLEAN (117): Structure Needs Cleaning",
    "ENOTNAM (118): Not A XENIX Named Type File",
    "ENAVAIL (119): No XENIX Semaphores Available",
    "EISNAM (120): Is A Named Type File",
    "EREMOTEIO (121): Remote I/O Error",
    "EDQUOT (122): Disk Quota Exceeded",
    "ENOMEDIUM (123): No Medium Found",
    "EMEDIUMTYPE (124): Wrong Medium Type",
    "ECANCELED (125): Operation Canceled",
    "ENOKEY (126): Required Key Not Available",
    "EKEYEXPIRED (127): Key Has Expired",
    "EKEYREVOKED (128): Key Has Been Revoked",
    "EKEYREJECTED (129): Key Was Rejected By Service",
    "EOWNERDEAD (130): Owner Died",
    "ENOTRECOVERABLE (131): State Not Recoverable",
    "ERFKILL (132): Operation Not Possible Due To RF-Kill",
    "EHWPOISON (133): Memory Page Has Hardware Error",
    "ENOTSUP (134): Operation Not Supported"
};

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   error_exitFailure
 * BRIEF:   Exit Because Of Failure
 * RETURN:  void: Returns Nothing
 * ARG:     CheckErrno: Check errno Value
 * ARG:     errorMessage: Error Message
 * ARG:     ...: Optional Arguments
 ****************************************************************************************************/
void error_exitFailure(const bool CheckErrno, const char *errorMessage, ...)
{
    va_list argList;
    
    /*** Print Error Message ***/
    if(CheckErrno)
    {
        /* Print Error Reason */
        fprintf(stderr, "%s\n", error_getString(errno));
    }
    
    if(errorMessage != NULL)
    {
        /* Print Error Message */
        va_start(argList, errorMessage);
        vfprintf(stderr, errorMessage, argList);
        va_end(argList);
    }
    
    exit(EXIT_FAILURE);
}

/****************************************************************************************************
 * FUNCT:   error_getString
 * BRIEF:   Get Error String
 * RETURN:  char *: Error String
 * ARG:     Error: Error To Get String For
 ****************************************************************************************************/
const char *error_getString(const int Error)
{
    /*** Error Check ***/
    if((Error < 0) || (Error > (int)(sizeof(error_string) / sizeof(error_string[0]))))
        return "Error Out Of Range";
    
    /*** Get Error String ***/
    return error_string[Error];
}
