#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#include "utils.h" //sendChunkData and shuffle

void execute(char **argv, int nProcesses);

#endif