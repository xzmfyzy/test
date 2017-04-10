#ifndef __OS_REGEX_H
#define __OS_REGEX_H

/*
 * OS_StrBreak.
 * Break a string in "size" pieces, divided by a character
 * "match"
 * Returns 0 in case of success.
 */
char **OS_StrBreak(char match, char * str, int size);

/* Look at main.c for other examples */
#endif
