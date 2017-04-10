/* Basic SUCESS/ERROR definitions */
#define ERROR 		1	
#define SUCCESS 		0	

/* Whenever whe need to speficy r/w permissions */
#define READ 		1
#define	WRITE		2

/* Size limit control */
#define OS_MAXSTR 	1024	/* Maximum size to be read */
#define OS_MAXMSG	512	    /* Maximum msg to be passed */
#define OS_RULESIZE	256	    /* Maximum size for a rule */	 	
#define OS_FLSIZE	256	    /* Maximum size whenever reading a file */		
#define OS_DEFSIZE	128	    /* Default size */
#define OS_KEYSIZE	32	    /* Maximum size for a key */

#define HIGHLEVEL       8	/* Maximum level for a rule */
#define LOWLEVEL        0	/* Lowest level for a rule */

#ifndef USER
    #define USER            "sec"
#endif

#ifndef GROUPGLOBAL
    #define GROUPGLOBAL     "sec"
#endif

#ifndef REMUSER
    #define REMUSER         "secr"
#endif

#ifndef DEFAULTDIR		
    #define DEFAULTDIR	"/opt/sec"
#endif

/* queues configuration */
#ifndef DEFAULTQUEUE
	#define DEFAULTQUEUE	"/queue/sec/queue"
#endif

#ifndef DEFAULTCPATH
	#define DEFAULTCPATH	"/opt/hids/test/sec.conf"
#endif

#ifndef DEFAULT_SECURE
	#define DEFAULT_SECURE 33440 /* Default UDP port- secure */
#endif

#ifndef DEFAULT_SYSLOG
	#define DEFAULT_SYSLOG 514 /* Default syslog port - udp */
#endif

#ifndef HEARTBEAT_PORT_C
	#define HEARTBEAT_PORT_C 8410
#endif

#ifndef HEARTBEAT_PORT_S
        #define HEARTBEAT_PORT_S 8411
#endif

#ifndef HEARTBEAT_INTERVAL_SEC
	#define HEARTBEAT_INTERVAL_SEC 30
#endif

#ifndef HEARTBEAT_INTERVAL_USEC
        #define HEARTBEAT_INTERVAL_USEC 0
#endif

#ifndef DEFAULT_SERVER_ADDR
	#define DEFAULT_SERVER_ADDR "10.4.242.141"
#endif
