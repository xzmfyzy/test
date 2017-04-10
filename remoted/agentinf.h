#ifndef __AGENTINFO_H

#define __AGENTINFO_H

#define IPHASH 65536

#define AGT_NEW 	0
#define AGT_LIVE	1
#define AGT_DEAD	2

#include<stdint.h>
typedef struct _agent_info
{
    //unsigned char iphash;
    uint32_t ip_addr;
    uint16_t stat;
    struct _agent_info *next_ptr;
}agent_info;

agent_info * G_agt_inf[IPHASH] = {0,};

uint16_t HashIp(uint32_t ip);
uint8_t AddAgt(uint32_t ip);
uint8_t DelAgt(uint32_t ip);
agent_info * GetAgt(uint32_t ip);
#endif

