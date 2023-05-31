#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <queue>
#include "types_p2.h"


void *threadfunc(void *parm);
void *input_thread(void *args);
void *queue_thread(void *args);
void woman_wants_to_enter(Person& pers);
void man_wants_to_enter(Person& pers);
void woman_leaves(int index);
void man_leaves(int index);


#endif