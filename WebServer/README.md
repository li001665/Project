test machine: kh1250-03

date: 12/13/2020

name: Jianhua Li, Qiyu Tian

x500: li001665@umn.edu(Li), tian0068@umn.edu(Tian)


# Complie

open the folder in P4final, then open termial and run:

$ make clean

$ make

The program will be compiled.

# Run

open one terminal and input: % ./webserver port path num_dispatch num_workers dynamic flag qlen cache_entries

open another terminal and input: wget http:IP: 9000(1025-65535)/file path in testing folder or input: xargs -n num_args -P num_procs cmd

# Program

Our program use dispatcher threads to get the requests of the client and place them in a queue. Then we use worker threads to retrieve requests from the queue and send it back to the client.

no self-created function.

We use socket programming to get request and information from the client to the server and return back to the client.

Also, we make sure that all the implementations are thread-safe.   


