#include<stdio.h>

#ifndef ARGV0
   #define ARGV0 "sec-remoted"
#endif

#include "agentinf.h"

#include "headers/defs.h"
#include "headers/debug_op.h"

#include "error_messages/error_messages.h"


/* the ip value should be Big Endian */
uint16_t HashIp(uint32_t ip)
{
    uint16_t hash;
    hash = (uint16_t)((ip & 0xFFFF0000) >> 16);
    return hash;
}

uint8_t AddAgt(uint32_t ip)
{
    uint16_t hash = HashIp(ip);
    agent_info *ptr = malloc(sizeof(agent_info));
    if(ptr == NULL)
    {
        merror(MEM_ERROR,ARGV0);
        return ERROR;
    }
    ptr->ip_addr = ip;
    ptr->stat = AGT_NEW;
    if(G_agt_inf[hash] == NULL)
    {
        ptr->next_ptr = NULL;
        G_agt_inf[hash] = ptr;
    }
    else
    {
        ptr->next_ptr = G_agt_inf[hash];
        G_agt_inf[hash] = ptr;
    }
}

uint8_t DelAgt(uint32_t ip)
{
    uint16_t hash = HashIp(ip);
    if(G_agt_inf[hash] == NULL) return SUCCESS;
    agent_info *ptr = G_agt_inf[hash];
    if(ptr->ip_addr == ip)
    {
        G_agt_inf[hash] = ptr->next_ptr;
        free(ptr);
        return SUCCESS;
    }
    if(ptr->next_ptr == NULL) return SUCCESS;
    agent_info *ptr_current = ptr->next_ptr;
    while(ptr_current != NULL)
    {
        if(ptr_current->ip_addr == ip)
        {
            ptr->next_ptr = ptr_current->next_ptr;
            free(ptr_current);
            return SUCCESS;
        }
        else
        {
            ptr = ptr->next_ptr;
            ptr_current = ptr_current->next_ptr;
        }
    }
    return SUCCESS;
}

agent_info * GetAgt(uint32_t ip)
{
    uint16_t hash = HashIp(ip);
    agent_info *ptr = G_agt_inf[hash];
    while(ptr != NULL)
    {
        if(ptr->ip_addr == ip)
            return ptr;
    }
    return ptr;
}
