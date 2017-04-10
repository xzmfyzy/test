#ifndef __FILE_H

#define __FILE_H

#define OS_PIDFILE	"/var/run"

int File_DateofChange(char *file);

int IsDir(char *file);

int CreatePID(char *name, int pid);

int DeletePID(char *name);

#endif

