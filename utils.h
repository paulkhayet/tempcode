#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <queue>
#include "types_p2.h"

#define MSEC(x) x*1000


long get_elapsed_time(struct timeval& start, struct timeval& end);
long get_runtime(void);
void show_line(std::queue<Person> line);
int gen_random(int min, int max);


#endif