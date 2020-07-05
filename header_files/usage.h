void usage(char* prog_name){

  for(int i = 0; i < 50; i++){printf("-");}
  printf("\n\nNAME: \n");
  
  printf("\t%s - Slow Loris attack with proxychain option\n\n", prog_name);
  
  printf("DESCRIPTION: \n");
  
  printf("\t-t, --target <IP>\n");
  printf("\t\tSpecify the IP of the target - mandatory\n\n");
  
  printf("\t-p, --port <PORT_NUM>\n");
  printf("\t\tSpecify the port of the target - mandatory\n\n");
  
  printf("\t-s, --sockets <SOCK_NUM>\n");
  printf("\t\tSpecify the number of sockets to use - default 200\n\n");
  for(int i = 0; i < 50; i++){printf("-");}
  printf("\n");

  return;

}



