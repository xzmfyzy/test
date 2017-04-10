#ifndef __SIG_H

#define __SIG_H

void HandleSIG();
void HandleSIGPIPE();

/* Start signal manipulation */
void StartSIG(char *process_name);

#endif

