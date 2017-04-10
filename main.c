#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <netdb.h>
#ifndef ARGV0
   #define ARGV0 "sec-agentd"
#endif

#include "agentd.h"

#include "os_net/os_net.h"

#include "headers/defs.h"
#include "headers/debug_op.h"
#include "headers/sec.h"

#include "os_crypto/md5/md5_op.h"

#include "error_messages/error_messages.h"

#define IPSIZE 16
/* External debug and chroot flags */
short int dbg_flag=1;
short int chroot_flag=0;

int sock_hb_send;
/* filescan test */
#include <dirent.h>
#include <sys/stat.h>
typedef int (*_CHKF_FUNC)(char *, FILE *);

int checkfile(char *path, FILE *fp) {
    if (path == NULL) return ERROR;

    //if (access(path, R_OK) == -1) return ERROR;

    os_md5 filesum;

    OS_MD5_File(path, filesum);

    merror("%s: %s", path, filesum);
    time_t tm;
    struct tm *p;
    tm = time(NULL);
    p = localtime(&tm);
    if(fp)
    {
        (void)fprintf(fp,"%d/%02d/%02d %02d:%02d:%02d ",
                      p->tm_year+1900,p->tm_mon+1, 
                      p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
        (void)fprintf(fp, "%s:%s\n", path, filesum);
    }
    return SUCCESS;

}

void path_travel(char* path, _CHKF_FUNC chkf, FILE *fp)
{
    struct dirent *dt = NULL;
    DIR *dir;
    char dirname[50] = {0};
    strcpy(dirname, path);
    if((dir = opendir(path)) == NULL) printf("error\n");
    while((dt = readdir(dir)) != NULL)
    {
        sprintf(dirname, "%s/%s",path,dt->d_name);
        if(dt->d_type & DT_DIR)
        {
            if(dt->d_name[0] == '.') continue;
            path_travel(dirname, chkf, fp);
        }
        else
            chkf(dirname, fp);
    }
}

int filescan(char *path) {
    struct stat s;
    FILE *fp_res;
    fp_res = fopen("./filecheck.log","a");

    lstat(path, &s);
    if(S_ISDIR(s.st_mode))
        path_travel(path, checkfile, fp_res);
    else
        checkfile(path, fp_res);
    fclose(fp_res);
}
/**/

int do_cmd(char *buffer)
{
    /* now only filescan */
    int db;
    db = filescan(buffer);
    debug(buffer);
}

/**/
void heartbeat(int signo)
{
    char heartbeatmsg[16];
    struct sockaddr_in address;
    /* get local ip */
    struct hostent *he;
    char hostname[40] = {0};
    char ipaddr[40]={0};

    gethostname(hostname,sizeof(hostname));
    he = gethostbyname(hostname);

    char **phe = NULL;
    for( phe=he->h_addr_list ; NULL != *phe ; ++phe)
    {
        inet_ntop(he->h_addrtype,*phe,heartbeatmsg,sizeof(heartbeatmsg));

    }
    /**/

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr(DEFAULT_SERVER_ADDR);//这里不一样  
    address.sin_port=htons(HEARTBEAT_PORT_S);

    sendto(sock_hb_send, heartbeatmsg, sizeof(heartbeatmsg), 0, (struct sockaddr *)&address, sizeof(address));
}


int init_timer()
{
    struct sigaction tact;
    tact.sa_handler = heartbeat;
    tact.sa_flags = 0;
    sigaction(SIGALRM, &tact, NULL);

    struct itimerval hb_timerval;
    hb_timerval.it_value.tv_sec = HEARTBEAT_INTERVAL_SEC;
    hb_timerval.it_value.tv_usec = HEARTBEAT_INTERVAL_USEC;
    hb_timerval.it_interval = hb_timerval.it_value;

    setitimer(ITIMER_REAL, &hb_timerval, NULL);

    return SUCCESS;
}

void _startd(agent *logr, char *dir, int uid, int gid) {
    /* init */
    int m_queue = 0; 
    unsigned short int rand0; /* Rand addition */
    unsigned int port = 0;
    int sock_cmd_recv;
    char *buffer;    
    keystruct keys;

    char srcip[IPSIZE];


    //if((logr->port == NULL) || (port = atoi(logr->port) <= 0))
    //{
        port = DEFAULT_SECURE;
        /* merror(PORT_ERROR,ARGV0,port); */
    //}
    //if(Privsep_SetGroup(gid) < 0)
    //    ErrorExit(SETGID_ERROR,ARGV0,gid);

    //if(Privsep_Chroot(dir) < 0)
    //    ErrorExit(CHROOT_ERROR,ARGV0,dir);

    chroot_flag = 1; /* Inside chroot now */

    //if(Privsep_SetUser(uid) < 0)
    //    ErrorExit(SETUID_ERROR,ARGV0,uid);

    /* Create the queue. In this case we are going to create
     * and read from it
     * Exit if fails.
     */
    //if((m_queue = StartMQ(DEFAULTQUEUE,READ)) < 0)
    //    ErrorExit(QUEUE_ERROR,ARGV0,DEFAULTQUEUE);

    /* Only using UDP 
     * UDP is faster and unreliable. Perfect :)
     */	
    if((sock_cmd_recv = OS_Bindportudp(port,NULL)) < 0)
        ErrorExit(BIND_ERROR,ARGV0,port);

    if((sock_hb_send = OS_Bindportudp(HEARTBEAT_PORT_C,NULL)) < 0)
        ErrorExit(BIND_ERROR,ARGV0,HEARTBEAT_PORT_C);

    init_timer();
    /* daemon loop */	
    for(;;)
    {
        char *msg = NULL;
        /* Receiving from the unix queue */
        //if((msg = OS_RecvUnix(m_queue, OS_MAXSTR)) != NULL)
        {
        //    char *crypt_msg = NULL;
        //    int _ssize = 0; /* msg socket size */

        //    rand0 = (unsigned short int)rand(); /* new random */
            //crypt_msg = CreateSecMSG(&keys, msg, 0, &_ssize, rand0);
            
            /* Returns NULL if can't create encrypted message */
        //    if(crypt_msg == NULL)
        //    {
        //        merror(SEC_ERROR,ARGV0);
        //        free(msg);
        //        continue;
        //    }
            buffer = OS_RecvAllUDP(sock_cmd_recv, OS_MAXSTR, srcip, IPSIZE);
            if(buffer != NULL)
            {
                do_cmd(buffer);
                free(buffer);
            }
            //else
            //{printf("do nothing");fflush(stdout);}
            /* Send _ssize of crypt_msg */
            //if(OS_SendUDPbySize(sock, _ssize, crypt_msg) < 0)
            //    merror(SEND_ERROR,ARGV0);

            /* No need to set them to null */
        //    free(crypt_msg);
            //merror(msg);
            //printf(msg);
            //free(msg);
        }
    }
}

int main(int argc, char** argv) {
    int c = 0;

    char *dir = DEFAULTDIR;
    char *user = USER;
    char *group = GROUPGLOBAL;

    int uid=0;
    int gid=0;

    agent logr;

    while((c = getopt(argc, argv, "dhu:g:D:")) != -1) {
        switch(c) {
            case 'h':
                //help();
                printf("Help:\n");
                printf("just test\n");
                break;
            case 'd':
                dbg_flag++;
                break;
            case 'u':
                if(!optarg)
                    ErrorExit("%s: -u needs an argument",ARGV0);
                user = optarg;
                break;
            case 'g':
                if(!optarg)
                    ErrorExit("%s: -g needs an argument",ARGV0);
                group = optarg;
                break;		
            case 'D':
                if(!optarg)
                    ErrorExit("%s: -D needs an argument",ARGV0);
                dir = optarg;
                break;
        }
    }
    debug("start...\n",ARGV0);

    /* Starting the signal manipulation */
    StartSIG(ARGV0);

    if(fork() == 0)
    {
//debug("deamon",ARGV0);
        /* Forking and going to background */
        _startd(&logr,dir,uid,gid);
    }
    return(0);
}


