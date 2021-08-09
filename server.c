#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<stdlib.h>
#include<signal.h>
#include<sys/stat.h>

#define SIZE 1000000

void get_html(char * name, char * buffer){
	FILE * fp;
	char *header = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: %d\n\n";
	char buffer2[SIZE];
	char buffer3[SIZE];
	
	buffer[0] = '\0';

	fp = fopen("index.html" , "r");
	fread(buffer3 , sizeof(char) , SIZE , fp);
	fclose(fp);

	sprintf(buffer2, buffer3, name);
	int length = strlen(buffer2);
	sprintf(buffer, header, length);
	strcat(buffer, buffer2);
}

void get_name(char * message, char * buffer){
	int ptr = -1;
	for (int i = 0; i < SIZE; i++){
		if (ptr > -1){
			if (message[i] == ' '){
				buffer[ptr] = '\0';
                break;
			}
			else {
				if (message[i] == '+'){
					buffer[ptr] = ' ';
				} else {
					buffer[ptr] = message[i];
				}
				ptr++;
			}
		} else {
			if (message[i] == '='){
				ptr++;
			}
		}
	}
}

void get_file_name(char * message, char * buffer){
	int ptr = -1;
	for (int i = 0; i < SIZE; i++){
		if (ptr > -1){
			if (message[i] == ' '){
				buffer[ptr] = '\0';
                break;
			} else {
				buffer[ptr] = message[i];
			}
			ptr++;
		} else {
			if (message[i] == '/'){
				ptr++;
			}
		}
	}
}

void get_img_as_bytes(char * filename, char * buffer){
	FILE * fp = fopen(filename, "rb");	
	struct stat st;
	char * buffer2 = calloc(SIZE, sizeof(char));
	char * buffer3 = calloc(SIZE, sizeof(char));
	
	char * header = "HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: %d\n\n";
	
	stat(filename, &st);
	int size = st.st_size;

	sprintf(buffer2, header, size);
	
	fread(buffer3 , sizeof(char) , SIZE , fp);
	fclose(fp);
	
	strcpy(buffer, buffer2);
	int header_length = strlen(buffer);
    int ptr = 0;
	for (int i = header_length; i < SIZE; i++){
		buffer[i] = buffer3[ptr];
        ptr++;
	}
}

int main(int argc , char *argv[])
{
	signal(SIGPIPE, SIG_IGN);
	int socket_desc , new_socket , c;
	struct sockaddr_in server , client;
	char *message;
	char buffer[SIZE];
	char name[SIZE];
	FILE * fp;

	//get_html("Andrew Mackenzie", buffer);

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("%s\n", "Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind failed");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	int read_size;
	char client_message[SIZE];
	char filename[100];
	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		read_size = recv(new_socket , client_message , SIZE , 0);
		printf("%s\n", client_message);
		get_file_name(client_message, filename);

		if (strstr(client_message, "GET /?name") != NULL){
			get_name(client_message, name);
			get_html(name, buffer);
		} else if (strstr(filename, ".") != NULL){
			printf("Getting file...");
			get_img_as_bytes(filename, buffer);
			printf("Got file!\n");
		} else {
			get_html("Andrew Mackenzie", buffer);
		}

		write(new_socket , buffer , SIZE);
		close(new_socket);
	}
	
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}
