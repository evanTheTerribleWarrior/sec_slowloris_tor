//TODO: Refurbish all data types and dynamic allocations when sensible
//TODO: Change all functions like strcpy with safer versions
//TODO: Logic of socket re-creation review

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include "header_files/usage.h"
#include "header_files/constants.h"
#include "header_files/defs.h"
#include "header_files/utils.h"
#include "header_files/socks_funcs.h"
#include "header_files/threads_funcs.h"

int main(int argc, char *argv[]){
	
  if (argc < 2) {usage(argv[0]);exit(EXIT_SUCCESS);}
  
  time_t t;
  srand((unsigned) time(&t));
  
  char *ip;
  ip = (char *)malloc(strlen(argv[1]) * sizeof(char));
  strcpy(ip, argv[1]);
  int port = atoi(argv[2]);
  int socket_num = atoi(argv[3]);
  
  
  //Calculate socks per tor port in utils.h
  int* socks_per_tor_port = (int*)calloc(TOR_PORTS_NUM, sizeof(int));
  calculateSocksPerTorPort(socket_num, socks_per_tor_port);
  
  //Create all the threads and array of tor data structs to pass in each thread
  pthread_t tid[TOR_PORTS_NUM];
  tor tor_data[TOR_PORTS_NUM];  

  for(int i = 0; i < TOR_PORTS_NUM; i++){
    tor_data[i].sockets_num = socks_per_tor_port[i];
    tor_data[i].tor_port = tor_ports[i];
    tor_data[i].ip = (char*)malloc(sizeof(char) * strlen(ip));
    tor_data[i].target_port = port;
    strcpy(tor_data[i].ip, ip);
    pthread_create(&tid[i], NULL, createProxies, (void * restrict) &tor_data[i]);
  }
  pthread_exit(NULL);

}

