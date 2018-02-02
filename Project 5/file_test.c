/**************************************
*student: Yu-Hsuan, Chuang 1211219305 *
*student: Yiqian Zhang	   1211820737 *
**************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include"msgs.h"

int global_id = 1;

semaphore_t *clients;


void client( char *filename){

	int id = global_id;
    int port = global_id++; 
  
  	message *namemsg = (message *)malloc(sizeof(message));
  	namemsg->client_port = port;
    namemsg->name_or_filecontent = false; // name:false, content:true;
  	for(int i = 0; i < strlen(filename); i++)
		namemsg->data[i] = filename[i];

	namemsg->data[strlen(filename)] = '\0';
	printf("\n\n [%d] sent ", id);

	send(99, *namemsg );
	printf("\n\nClient[%d] sent filename [\"%s\"] to server on port: [99]", id, filename);
	message recieved_msg = recieve(port);
	printf("\n\nClient[%d] recieve feedback ", id);
	if(recieved_msg.client_number_exceed)
		printf("\n\nClient[%d] DO NOT DO THAT!!!!!!", id);
	else if(recieved_msg.filename_size_exceed)
		printf("\n\nClient[%d] filename [\"%s\"] size exceeds 15 char", id, filename);
	else
	{
		printf("\n\nClient[%d] sent filename [\"%s\"] to server successfully!!!!!!", id, filename);
		FILE *f = fopen( filename, "rb" );
		char c;
		int i = 0;
		message *contentmsg = (message *)malloc(sizeof(message));
  		contentmsg->client_port = port;
  		contentmsg->name_or_filecontent = true; // name:false, content:true;

		while((c = fgetc(f)) != EOF)
		{
			contentmsg->data[i] = c;
			i++;
		}
		contentmsg->data[i] = '\0';
		send(99, *contentmsg );
		printf("\n\nClient[%d] sent file content [\"%s\"] content to server on port: [99]", id, contentmsg->data);
		message recieved_msgg = recieve(port);
		printf("\n\nClient[%d] recieve feedback from server on port: [%d]", id, port);
		if(recieved_msgg.file_size_exceed)
			printf("\n\nClient[%d] file [\"%s\"] size exceeds 1MB", id, filename);
		else
			printf("\n\nClient[%d] sent file content [\"%s\"] to server successfully!!!!!!", id, contentmsg->data);
		printf("\n");
		fclose(f);
		free(contentmsg);
	}
	free(namemsg);

	while(1) 
	{
		yield();
	}
}


void server(char *arg){
	
	char filename[3][255];

	while(1)
	{
		message msg = recieve(99);
	    printf("\n\n\t\t\t\tServer recieved on port: [99] from Client[%d]", msg.client_port);
	    message *send_msg = (message*)malloc(sizeof(message));
	    send_msg->client_number_exceed = false;
	    send_msg->file_size_exceed = false;
	    send_msg->filename_size_exceed = false;

	    if(msg.client_port > 3)
	    	send_msg->client_number_exceed = true;
	    else
	    {
	    	if(msg.name_or_filecontent == false)  // file name
	    	{
	    		printf("\n\n\t\t\t\tServer recieved Client[%d] filename", msg.client_port);
	    		int i;
	    		for(i = 0; i < sizeof(msg.data); i++)
	    		{
	    			if(msg.data[i] == '\0')
	    				break;
	    		}
	    		printf("\n\n\t\t\t\t\tfilename length = %d\n", i);
	    		if(i > 14)
	    		{
	    			send_msg->filename_size_exceed = true;
	    			printf("\n\n\t\t\t\t\tfilename size exceeds");
	    		}
	    		else
	    		{
	    			for(int i = 0; i < sizeof(msg.data); i++)
	    				filename[msg.client_port][i] = msg.data[i];
	    			strcat(filename[msg.client_port], ".server");
	    		}
	    	}
	    	else if(msg.name_or_filecontent == true)  // file content
	    	{
	    		printf("\n\n\t\t\t\tServer recieved Client[%d] file content : ", msg.client_port);
	    		FILE *f = fopen(filename[msg.client_port], "ab" );
	    		for(int i = 0; i < sizeof(msg.data); i++)
	    		{
	    			printf("\n\t\t\t\t\t%c", msg.data[i]);
	    			if(msg.data[i] == '\0')
	    				break;
	    			fputc((char)msg.data[i], f);
	    			if(ftell(f) > 1000000)
	    			{
	    				printf("\n\n\t\t\t\t\tfile size exceeds %d", i);
	    				send_msg->file_size_exceed = true;
	    				break;
	    			}
	    		}
	    		fclose(f);
	    	}
	    }

	    send(msg.client_port, *send_msg);
	    printf("\n\n\t\t\t\tServer send back to Client[%d]", msg.client_port);
	    printf("\n");
		free(send_msg);
	}
}

int main( int argc, char *argv[] ) {

	clients = CreateSem(1);

	if( argc > 1 ) {
		port_initialize();
		for(int i = 2; i < argc; i++ ){
			start_thread( &client, argv[i] );
		}
		start_thread( &server, NULL );
		run();
	}

	return 0;
}
