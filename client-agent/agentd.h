#ifndef __LOGCLIENT_H

#define __LOGCLIENT_H

typedef struct _agent
{
	char *port;
	char *rip; /* remote (server) ip */
}agent;


/** Prototypes **/

/* Client configuration */
int ClientConf(char *cfgfile,agent *logr);


#endif
