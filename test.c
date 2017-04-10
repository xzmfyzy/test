#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <netdb.h>
#include <dirent.h>

void _log(const char * msg,va_list args)
{
    (void)vfprintf(stderr, msg, args);
}

void merror(const char * msg,... )
{
    va_list args;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
}


void scanlist(char *path)
{
    printf("路径为[%s]\n", path);
    struct dirent *dt = NULL;
    DIR *dir;
    char dirname[50] = {0};

    strcpy(dirname, path);

    


    if((dir = opendir(path)) == NULL) printf("opendir %s error",path);
    while((dt = readdir(dir)) != NULL) 
    {
        if(dt->d_type & DT_DIR)
        {
            if(dt->d_name[0] == '.') continue;
            sprintf(dirname, "%s/%s",path,dt->d_name);
            scanlist(dirname);
        }
        else
            printf("%s\n",dt->d_name);
    }
}

int main(int argc, char *argv[])
{
  //char *str="ttt";
  //merror("test%s: %s",str,str);
  //  return(0);
  //  scanlist(argv[1]);
  char *ip="127.0.0.1";
  unsigned int addr;
  struct in_addr adr_inet;
  inet_aton(ip,&adr_inet);
  addr = adr_inet.s_addr;
  unsigned short p = addr >> 16;
  //printf("%u",addr>>16);


FILE *fp = fopen("agentlist","r");
char buff[17];
while(fgets(buff, 17, fp) != NULL)
{
printf(buff);
}
    return 0;
}
