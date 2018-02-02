/**************************************
*student: Yu-Hsuan, Chuang 1211219305 *
*student: Yiqian Zhang	   1211820737 *
**************************************/

#include "msgs.h"

int global_id = 1;

void server() {
  while(1) {
    message msg = recieve(99);
    printf("\n\n\t\t\t\tServer recieved on port: [99] from Client[%d]", msg.client_port);
    send(msg.client_port, msg);
    printf("\n\n\t\t\t\tServer sent to port: [%d]", msg.client_port);
  }
}

void client() {
  int id = global_id;
  int port = global_id++; 
   while(1) {
    message *msg = (message *)malloc(sizeof(message));
    
    for(int i = 0; i< 10; i++) {
      msg->data[i] = i+(id*10);
    }
    msg->client_port = port;
    
    send(99, *msg);
    printf("\n\nClient[%d] sent to port: [99]",id);
    
    message recieved_msg = recieve(port);
    printf("\n\nClient[%d] recieved from port: [%d]",id, port);

    sleep(2);
    free(msg);
   }
}

int main()
{
  port_initialize();
  ReadyQ = (queue *)malloc(sizeof(queue));
  NewQueue(ReadyQ);

  start_thread(&client);
  start_thread(&client);
  start_thread(&client);
  start_thread(&client);
  start_thread(&client);

  start_thread(&server);

  run();
  while(1) sleep(1);
  return 0;
}