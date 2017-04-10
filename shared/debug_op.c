#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>

#include "headers/defs.h"
#include "headers/debug_op.h"

extern short int dbg_flag;
extern short int chroot_flag;
short int log_flag;

#ifndef LOGFILE
    //#define LOGFILE   "/logs/sec.log"
    #define LOGFILE   "../sec.log"
#endif

/* _log function */
void _log(const char * msg,va_list args)
{
    time_t tm;
    struct tm *p;
    va_list args2; /* For the stderr print, the pointer can be used only once! */
    tm = time(NULL);
    p = localtime(&tm);

    /* Duplicate args */
    va_copy(args2, args);

    if(log_flag == 1)
    {
        FILE *fp;

        /* If under chroot, log directly to /logs/sec.log */
        if(chroot_flag == 1)
            fp = fopen(LOGFILE,"a");
        else
        {
            char _logfile[128];
            memset(_logfile,'\0',128);
            snprintf(_logfile,127,"%s%s",DEFAULTDIR,LOGFILE);
            fp = fopen(_logfile, "a");
        }
        	
        if(fp)
        {
            (void)fprintf(fp,"%d/%02d/%02d %02d:%02d:%02d ",
                          p->tm_year+1900,p->tm_mon+1, 
                          p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
            (void)vfprintf(fp, msg, args);
            (void)fprintf(fp, "\n");
            fclose(fp);
        }
    }
    
    /* Print to stderr */		
    (void)fprintf(stderr,"%d/%02d/%02d %02d:%02d:%02d ",
                  p->tm_year+1900,p->tm_mon+1 ,p->tm_mday,
                  p->tm_hour,p->tm_min,p->tm_sec);
    (void)vfprintf(stderr, msg, args2);
    (void)fprintf(stderr, "\n");
}

void merror(const char * msg,... )
{
    va_list args;
    log_flag=1;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
}

void debug(const char * msg,...)
{
    if(dbg_flag >= 1)
    {
        va_list args;
        log_flag=0;
        va_start(args, msg);

        _log(msg, args);

        va_end(args);
    }
}

void ErrorExit(const char *msg, ...)
{
    va_list args;
    log_flag=1;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
    exit(1);
}

