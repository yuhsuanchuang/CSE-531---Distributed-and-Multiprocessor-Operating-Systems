#include "sem.h"

#define MSG_SIZE 10
#define MSG_COUNT 10
#define PORT_COUNT 100

typedef struct message {
  char data[255];
  int client_port;
  bool name_or_filecontent;
  bool client_number_exceed;
  bool file_size_exceed;
  bool filename_size_exceed;
}message;

typedef struct port {
  message msgsss[MSG_COUNT];
  semaphore_t *empty, *full, *mutex;
  int in, out;
}port;

port ports[PORT_COUNT];

void port_initialize() {
  for(int i = 0; i < PORT_COUNT; i++) {
    ports[i].mutex = CreateSem(1);
    ports[i].empty = CreateSem(0);
    ports[i].full = CreateSem(MSG_COUNT);
    ports[i].in = 0;
    ports[i].out = 0;
  }
}

void send(int port_num, message send_msg) {
  port *p = &ports[port_num - 1];
  P(p->full);
  P(p->mutex);
  
  p->msgsss[p->in] = send_msg;
  p->in = (p->in + 1) % MSG_COUNT;
  V(p->mutex);
  V(p->empty);
}

message recieve(int port_num) {
  port *p = &ports[port_num - 1];
  message recieve_msg;
  P(p->empty);
  P(p->mutex);

  recieve_msg = p->msgsss[p->out];
  p->out = (p->out + 1)% MSG_COUNT;
  V(p->mutex);
  V(p->full);
  return recieve_msg;
}

