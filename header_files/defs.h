struct tor_struct {
  int tor_port;
  int target_port;
  char *ip;
  int sockets_num;
};

typedef struct tor_struct tor;
