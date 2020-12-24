test machine: kh1250-03

date: 11/3/2020

name: Jianhua Li

x500: li001665@umn.edu(Li)

# Purpose

In this project we implemented sendChunkData(), getChunkData(), shuffle() and getInterData()
function using inter process commuciation(IPC) message queue. These functions are used to help implement map and reduce in project 1.

# Complie

open the folder Template in P2, then open termial and run:

$ make

$ ./mapreduce #mappers #reducers filepath.txt

The program will be compiled.

# Program

## sendChunkData

This function distribute chunks of the input file to the mappers in a round robin fashion. Each chunk will contain 1024B and be stored in a message queue.

## getChunkData

This function will receive chunks which stored in a message queue from master. The mapper will
use this function to retrieve them one by one until there are no more.

## shuffle

This function will send the paths of word.txt to the reducers based on a hash function.Each file name will be store in a message queue. Files with same names across different Map_mapperID will go to the same reducer. 

## getInterData

Each reducer uses this function to retrieve the file path of words in a message queue.

# Assumptions

None













































