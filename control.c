#include<stdio.h>
#include<stdlib.h>

#include "os_net/os_net.h"

#include "headers/defs.h"
#include "headers/debug_op.h"
#include "headers/sec.h"

/* External debug and chroot flags */
short int dbg_flag=0;
short int chroot_flag=0;

int main(int argc, char** argv)
{
    int m_queue;
    if((m_queue = StartMQ(DEFAULTQUEUE,WRITE)) < 0)
    {
        printf("fuck\n");
        return;
    }
    if(SendMSG(m_queue, "/opt/hids/test/", "hei","ha",1) < 0)
    {
        printf("send failed.\n");
    }
}
