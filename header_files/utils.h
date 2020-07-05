
//Just prints out dashes for better output section separation
void printDashes(){
  printf("\n");
  for(int i = 0; i < 50; i++){
    printf("-");
  }
  //printf("\n");
  return;
}

//Print the current socket list from each thread
void printSocketArray(int *socket_list, int length){
  printf("array length: %d\n", length); 
  int j = 0;
  printf("\nSocket list showing socket fd:\n\n[ ");
  for (int i = 0; i < length - 1; i++){
    j = i++;
    if(socket_list[j] <= 0){printf("]\n",i);break;}
    --i;
    if(i == 0){printf(" %d ",socket_list[0]);continue;}
    printf(", %d ", socket_list[i]);
  }
  return;
}

//Generate random integer and convert to string
char* itoa(char* num_string, int num){

  int random = rand() % num;
  sprintf(num_string, "%d", random);
  return num_string;

}

//Split the total socket number per tor port provided
void calculateSocksPerTorPort(int socket_num, int* socks_per_tor_port){ 
  if(socket_num % TOR_PORTS_NUM == 0){
    for (int i = 0; i < TOR_PORTS_NUM; i++){
      socks_per_tor_port[i] = socket_num / TOR_PORTS_NUM;
    }
  }
  else{
    int rounded = floor(socket_num / TOR_PORTS_NUM);
    int remaining_socks = socket_num - (TOR_PORTS_NUM * rounded);
    for (int i = 0; i < TOR_PORTS_NUM; i++){
      socks_per_tor_port[i] = floor(socket_num / TOR_PORTS_NUM);
      if(remaining_socks != 0 && i == TOR_PORTS_NUM - 1) socks_per_tor_port[i] += remaining_socks;
      //printf("port %d will have %d sockets\n", i, socks_per_tor_port[i]);
    } 
  }
}
