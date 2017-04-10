#ifndef _MQ__H

#define _MQ__H

#define LOCALFILE_MQ 	1

#define SYSLOG_MQ	    2

#define SECURE_MQ	    4

#define SNORT_MQ_FULL   5
#define SNORT_MQ_FULLC '5'

#define SNORT_MQ_FAST   6
#define SNORT_MQ_FASTC '6'

#define APACHERR_MQ	    7

#define SYSCHECK_MQ     8
#define SYSCHECK_MQ_C  '8'


int StartMQ(char * key,short int type);

int FinishMQ();

int SendMSG(int queue, char * message, char *locmsg, 
                       char *logroup, unsigned short int loc);

#endif

