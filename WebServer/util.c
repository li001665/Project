#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

#define BACKLOG 20
int mfd;
static pthread_mutex_t accept_con_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t get_request_mutex = PTHREAD_MUTEX_INITIALIZER;


/**********************************************
 * init
   - port is the number of the port you want the server to be
     started on
   - initializes the connection acception/handling system
   - YOU MUST CALL THIS EXACTLY ONCE (not once per thread,
     but exactly one time, in the main thread of your program)
     BEFORE USING ANY OF THE FUNCTIONS BELOW
   - if init encounters any errors, it will call exit().
 
************************************************/
void init(int port) {
  // how many pending connection needs?
  int sockfd;
  struct sockaddr_in my_addr;
  int enable = 1;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { //Create the socket
    perror("Can't create socket");  //check if socket is been created
    exit(1);
  }

  my_addr.sin_family = AF_INET; //creat socket address 
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {//Set the "re-use" option so we can shut down and restart the server without waiting for the standard timeout.
      perror("Can't set socket option"); //check set socket
      exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {//Bind the socket, making its address reusable
      perror("Could not bind"); //check bind
      exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {//Set up the socket to listen for incoming connection requests
      perror("Could not listen");//check listen
      exit(1);
  }
  mfd = sockfd;
}

/**********************************************
 * accept_connection - takes no parameters
   - returns a file descriptor for further request processing.
     DO NOT use the file descriptor on your own -- use
     get_request() instead.
   - if the return value is negative, the request should be ignored.
***********************************************/
int accept_connection(void) {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    while(1) {
        if (pthread_mutex_lock(&accept_con_mutex) < 0 ) {//lock the accept because of global variable
            puts("Failed to lock connection mutex.");
            fflush(stdout);
        }
      addr_size = sizeof(client_addr);
      //printf("addr_size %d",addr_size );
      if ((client_fd = accept(mfd,(struct sockaddr *)&client_addr,&addr_size)) < 0) {// accept request
          perror("Request being ignored");
      }
      if (pthread_mutex_unlock(&accept_con_mutex) < 0 )//unlock accept
      {
        puts("Failed to unlock connection mutex.");
        fflush(stdout);
      }
      return client_fd;
    }
}

/**********************************************
 * get_request
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        from where you wish to get a request
      - filename is the location of a character buffer in which
        this function should store the requested filename. (Buffer
        should be of size 1024 bytes.)
   - returns 0 on success, nonzero on failure. You must account
     for failures because some connections might send faulty
     requests. This is a recoverable error - you must not exit
     inside the thread that called get_request. After an error, you
     must NOT use a return_request or return_error function for that
     specific 'connection'.
************************************************/
int get_request(int fd, char *filename) {
    int readsize;
    char str[2048];
    char *name = "";
    char *name1 = "";
    char *name2 = "";

    if((readsize = read(fd, str, 2048)) >= 0){  //read from request fd into str
        if (pthread_mutex_lock(&get_request_mutex) < 0 ) {//lock to make strtok threadsafe
            puts("Failed to lock connection mutex.");//check lock
            fflush(stdout);//flush terminal
        }
        name = strtok(str, "\n");
        name1 = strtok(name, " ");
        if(strcmp(name1, "GET") != 0){//check recoverable request error "get"
            perror("Bad Request: request type is not GET.\n");
            return -1;
        }
        name2 = strtok(NULL, " ");
        if (pthread_mutex_unlock(&get_request_mutex) < 0) {//unlock, finish use strtok
            puts("Failed to lock connection mutex.");//check unlock
            fflush(stdout);
        }
        if (strstr(name2, "..") != NULL || strstr(name2, "//") != NULL){////check recoverable request error "..""//"
          perror("Bad Request: .. or // contained in requested file path.\n");
          return -1;
        }
        if (strlen(name2) > 1023){//check maxinmum request bytes
          perror("Bad Request: request file should be a maximum of 1023 bytes.\n");
          return -1;
        }
        strcpy(filename, name2);//copy request string to filename
        // printf("filename: %s\n", filename);
        return 0;
    }
    else{//check read
        perror("read problem.\n");
        return -1;
    }
}

/**********************************************
 * return_result
   - returns the contents of a file to the requesting client
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        to where you wish to return the result of a request
      - content_type is a pointer to a string that indicates the
        type of content being returned. possible types include
        "text/html", "text/plain", "image/gif", "image/jpeg" cor-
        responding to .html, .txt, .gif, .jpg files.
      - buf is a pointer to a memory location where the requested
        file has been read into memory (the heap). return_result
        will use this memory location to return the result to the
        user. (remember to use -D_REENTRANT for CFLAGS.) you may
        safely deallocate the memory after the call to
        return_result (if it will not be cached).
      - numbytes is the number of bytes the file takes up in buf
   - returns 0 on success, nonzero on failure.
************************************************/
int return_result(int fd, char *content_type, char *buf, int numbytes) {
  FILE *stream;
  if ((stream = fdopen(fd,"w")) == NULL) {
      perror("Cannot open the client fd.\n");
      if (fflush(stdout) != 0) {
          perror("fflush is failed.\n");
      }
      close(fd);
      return -1;
  }
  //print required message to stream
  if (fprintf(stream, "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %d\nConnection: Close\n\n", content_type, numbytes) < 0) {
      perror("Failed to print to stream.\n");//check if success
      if (fflush(stdout) != 0) {
          perror("fflush is failed.\n");//flush terminal
      }
      close(fd);
      return -1;
  }
  if (fflush(stream) < 0){
    puts("Failed to flush stream result.");
    fflush(stdout);
    close(fd);
    return -1;
  }//flush stream
  if((write(fd, buf, numbytes)) == -1){//write to client
      perror("Failed to write to client result.\n");//check if success
      if (fflush(stdout) != 0) {
          perror("fflush is failed.\n");//flush terminal
      }
      close(fd);
      return -1;
  }
  if (fflush(stdout) != 0) {
      perror("fflush is failed.\n");//flush terminal
  }
  close(fd);
  return 0;
}

/**********************************************
 * return_error
   - returns an error message in response to a bad request
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        to where you wish to return the error
      - buf is a pointer to the location of the error text
   - returns 0 on success, nonzero on failure.
************************************************/
int return_error(int fd, char *buf) {
    FILE *stream;
    stream = fdopen(fd,"w");
    if (stream) {//open fd as stream
        perror("Cannoty open the client fd.\n");//check if stream open
        if (fflush(stdout) != 0) { 
            perror("fflush is failed.\n");//flush terminal
        }
        close(fd);
        return -1;
    }
    size_t bytes = strlen(buf);
    if (fprintf(stream, "HTTP/1.0 404 Not Found\nContent-Type: text/html\nContent-Length: %lu\nConnection: Close\n\n", bytes) < 0) {
        perror("Failed to print to stream error\n");//check if success
        if (fflush(stdout) != 0) {
            perror("fflush is failed.\n");//flush terminal
        }
        close(fd);
        return -1;
    }
  if (fflush(stream) < 0){//flush stream
     puts("Failed to flush stream error.");
        fflush(stdout);
        close(fd);
        return -1;
  }
  if((write(fd, buf, bytes)) == -1){//write to client
      perror("Failed to write to client error.\n");
      if (fflush(stdout) != 0) {
          perror("fflush is failed.\n");//flush terminal
      }
      close(fd);
      return -1;
  }
  close(fd);
  return 0;
}