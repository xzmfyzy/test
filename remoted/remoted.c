#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <sys/un.h>

#include <sys/epoll.h>

#include "os_net/os_net.h"

#ifndef ARGV0
   #define ARGV0 "sec-remoted"
#endif

#include "remoted.h"

#include "headers/defs.h"
#include "headers/mq_op.h"
#include "headers/debug_op.h"
#include "headers/sig_op.h"
//#include "headers/help.h"
//#include "headers/privsep_op.h"
#include "headers/file_op.h"
#include "headers/sec.h"

#define IPSIZE 16

short int dbg_flag=0;
short int chroot_flag=0;

/* Error messages */
#include "error_messages/error_messages.h"

char agent_list[][16] = {"127.0.0.1",};
/* epoll test*/
int epoll_init_heartbeat(int epfd)
{
    int livesock;
    struct sockaddr_in sin;
    int addr_len=sizeof(struct sockaddr_in);
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=htonl(INADDR_ANY);
    sin.sin_port=htons(8411);
    livesock = socket(AF_INET,SOCK_DGRAM,0);

    struct epoll_event ev;
    ev.data.fd = livesock;
    ev.events=EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, livesock, &ev);

    bind(livesock,(struct sockaddr *)&sin,sizeof(sin));
    return livesock;
}

int epoll_init_console(int epfd)
{
#ifndef SUN_LEN
#define SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path)        \
		                      + strlen ((ptr)->sun_path))
#endif
    int m_queue;
    /* Create the queue. In this case we are going to create
     * and read from it
     * Exit if fails.
     */
    //if((m_queue = StartMQ(DEFAULTQUEUE,READ)) < 0)
    //    ErrorExit(QUEUE_ERROR,ARGV0,DEFAULTQUEUE);
    struct sockaddr_un n_us;

    n_us.sun_family = AF_UNIX;
    strncpy(n_us.sun_path, DEFAULTQUEUE, sizeof(n_us.sun_path)-1);

    m_queue = socket(AF_UNIX, SOCK_DGRAM,0);
    unlink(DEFAULTQUEUE);

    struct epoll_event ev;
    ev.data.fd = m_queue;
    ev.events=EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, m_queue, &ev);

    bind(m_queue, (struct sockaddr *)&n_us, SUN_LEN(&n_us));
    chmod(DEFAULTQUEUE,READ);

    return m_queue;
}
/**/

void update_agents(char *msg)
{
    FILE *fp_agti;
    fp_agti = fopen("./agent_info","a");
    fprintf(fp_agti,"%s\n",msg);
    fclose(fp_agti);
}

void _startit(int position, int connection_type, int uid, 
              int gid, char *dir, remoted *logr)
{
    int sock,m_queue,livesock;
    char message[128];
    



    /* Connecting UDP */
    //sock = OS_ConnectUDP(33440,"127.0.0.1");
    //if(sock < 0)
    //    ErrorExit(CONNS_ERROR,ARGV0,"127.0.0.1");
    

    int epfd, nfds;
    int i;
    struct epoll_event events[20];
    epfd = epoll_create(1000);

    livesock = epoll_init_heartbeat(epfd);
    m_queue = epoll_init_console(epfd);

    for(;;)
    {
        char *msg = NULL;
/*        if((msg = OS_RecvUnix(m_queue, OS_MAXSTR)) != NULL)
        {
            printf("%s\n",msg);
            if(OS_SendUDPbySize(sock, OS_MAXSTR, msg) < 0)
                merror(SEND_ERROR,ARGV0);
            free(msg);
        }*/
        /* Send _ssize of crypt_msg */
        //if(OS_SendUDPbySize(sock, 20, "/opt/hids/test/test") < 0)
        //    merror(SEND_ERROR,ARGV0);
        //recvfrom(livesock, message, sizeof(message),0,(struct sockaddr *)&sin,&addr_len);
        nfds = epoll_wait(epfd, events,20,500);


        for(i = 0;i < nfds;i++)
        {
            if(events[i].data.fd == m_queue)//send command
            {
                int sockfd = events[i].data.fd;
                read(sockfd, message, sizeof(message));
                /* Connecting UDP */
                //for
                sock = OS_ConnectUDP(33440,"127.0.0.1");
                if(sock < 0)
                    ErrorExit(CONNS_ERROR,ARGV0,"127.0.0.1");
                if(OS_SendUDPbySize(sock, 20, message) <0)
                    merror(SEND_ERROR,ARGV0);
            }
            else if((events[i].data.fd == livesock) && (events[i].events&EPOLLIN))//recv heartbeat
            {
                int sockfd;
                sockfd = events[i].data.fd;
                read(sockfd, message, sizeof(message));
                update_agents(message);
                //message[127] = '\0';
            }
            else if(events[i].events&EPOLLIN)//recv other
            {
            }
        }
        if(message != NULL)
        {
            //printf(message);
            memset(message,0,128);
        }
        //fflush(stdout);
    }
}

/* main, v0.1, 2017/03/22
 */
int main(int argc, char **argv)
{
    
    int c,binds=0,i=0,uid=0,gid=0;
    
    remoted logr;
    
    int connection_type=0;
    
    char *cfg=DEFAULTCPATH;
    char *dir=DEFAULTDIR;
    char *user=REMUSER;
    char *group=GROUPGLOBAL;

    while((c = getopt(argc, argv, "dhu:g:D:")) != -1){
        switch(c){
            case 'h':
                //help();
                //printf("Help:\n");
                //printf("just test\n");
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
        }
    }

    debug(STARTED_MSG,ARGV0);
    
    /* Return 0 if not configured */
    //if((binds = BindConf(cfg,&logr)) == 0)
    //{	
    //    merror(CONN_ERROR,ARGV0);
    //    exit(0);
    //}

    /* Return < 0 if bad configured */
    //else if(binds < 0)
    //    ErrorExit(CONFIG_ERROR,ARGV0);

    /*Check if the user/group given are valid */
    //uid = Privsep_GetUser(user);
    //gid = Privsep_GetGroup(group);
    //if((uid < 0)||(gid < 0))
    //    ErrorExit(USER_ERROR,user,group);


    /* Starting the signal manipulation */
    StartSIG(ARGV0);	

FILE *agt_fp = fopen("agentlist","r");


    /* Really starting the program. */
    /*for(i=0;i<binds;i++)
    {
        if(!logr.conn[i])
        {
            merror(CONNTYPE_ERROR,ARGV0,logr.conn[i]);
            continue;
        }
        
        else if(strcmp(logr.conn[i],"syslog") == 0)
            connection_type=SYSLOG_CONN;
            
        else if (strcmp(logr.conn[i],"secure") == 0)
            connection_type=SECURE_CONN;
            
        else
        {
            merror(CONNTYPE_ERROR,ARGV0,logr.conn[i]);
            continue;
        }
        
        if(fork() == 0)
            _startit(i,connection_type,uid,gid,dir,&logr);
        else
            continue;
    }*/
    if(fork() == 0)
        _startit(i,connection_type,uid,gid,dir,&logr);
    else
    return(0);
}

/* EOF */

