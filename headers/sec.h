#ifndef __SEC_H

#define __SEC_H

#define IDMAXSIZE 8

typedef struct _keystruct
{
    char **ids;
    char **keys;
    char **ips;
    int keysize;
}keystruct;

void ReadKeys(keystruct *keys);
char *ReadSecMSG(keystruct *keys, char *srcip, char *buffer);
char *CreateSecMSG(keystruct *keys,char *msg, int id, int *msgsize, 
	unsigned short int rand0);
int CheckAllowedIP(keystruct *keys, char *srcip, char *id);
int CheckSum(char *msg, int size);

#endif

/* EOF */

