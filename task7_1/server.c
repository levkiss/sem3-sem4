#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define PORTNUM 8086 //port number
#define MAX_CLIENTS 5 //max clients
#define BUFLEN 100

#define FNFSTR "Error! File not found "
#define BRSTR "Bad request "

typedef struct{
	struct sockaddr_in addr;	// client remote address 
	int conectfd; //connection file descriptor
	char name[BUFLEN]; //user name
	int usrid; //user id
}client_t;

client_t *clients[MAX_CLIENTS];
static unsigned int cli_count = 0;
int userid = 1;
//int endflag = 0;



/* Add client to queue */
void queue_add(client_t *cl){
	int i;
	for(i=0;i<MAX_CLIENTS;i++){
		if(!clients[i]){
			clients[i] = cl;
			return;
		}
	}
}
/* Delete client from queue */
void queue_delete(int usrid){
	int i;
	for(i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(clients[i]->usrid == usrid){
				clients[i] = NULL;
				return;
			}
		}
	}
}
/* Send message to all clients */
void send_message_all(char *s){
	int i;
	for(i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(write(clients[i]->conectfd, s, strlen(s))<0){
				perror("write");
				exit(-1);
			}
		}
	}
}

/* Send message to sender */
void send_message_self(const char *s, int connfd){
	if(write(connfd, s, strlen(s))<0){
		perror("write");
		exit(-1);
	}
}
/* Strip CRLF */
void strip_newline(char *s){
	while(*s != '\0'){
		if((*s == '\r') || (*s == '\n')){
			*s = '\0';
		}
		s++;
	}
}

void print_client_addr(struct sockaddr_in addr){
	printf("%d.%d.%d.%d",
		addr.sin_addr.s_addr & 0xFF,
		(addr.sin_addr.s_addr & 0xFF00)>>8,
		(addr.sin_addr.s_addr & 0xFF0000)>>16,
		(addr.sin_addr.s_addr & 0xFF000000)>>24);
}

void send_active_clients(int connfd){
	int i;
	char s[64];
	for(i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			sprintf(s, "Client ID: %d | NAME: %s\r\n", clients[i]->usrid, clients[i]->name);
			send_message_self(s, connfd);
		}
	}
	sprintf(s, ">>>>>>>>>>>>>>>>>>>>>>>\n");
	send_message_self(s, connfd);
}

void *handle_client(void *arg){
	client_t *cli = (client_t *)arg;
	char buff_out[BUFLEN];
	char buff_in[BUFLEN];
	char last_word[BUFLEN];
	int len, i;


	printf("From -> ");
	print_client_addr(cli->addr);
	printf(" connected client, named %s\n", cli->name);

	sprintf(buff_out, "New user called %s connected\r\n", cli->name);
	send_message_all(buff_out);


	//working with clients
	while((len = read(cli->conectfd, buff_in, sizeof(buff_in)-1)) > 0){
	    buff_in[len] = '\0';
	    buff_out[0] = '\0';
		strip_newline(buff_in);

		/* Ignore empty buffer */
		if(!strlen(buff_in)){
			continue;
		}

		/* Special options */
		if(buff_in[0] == '\\') {
			char *command, *param;
			command = strtok(buff_in," ");

			if(!strcmp(command, "\\quit")){
					param = strtok(NULL, " ");
					while(param != NULL){
								strcat(last_word, " ");
								strcat(last_word, param);
								param = strtok(NULL, " ");
					}
					strcat(last_word, "\r\n");
					break;
			}
			else if(!strcmp(command,"\\users")){
				snprintf(buff_out, sizeof(buff_out), "There are clients online. Their names are: \n");
				send_message_self(buff_out, cli->conectfd);
				send_active_clients(cli->conectfd);
			}
			else {
				sprintf(buff_out, "Wrong command ! Choose another \n\n");
				if(write(cli->conectfd, buff_out, strlen(buff_out))<0){
					perror("write");
					exit(-1);
				}
			}
		}
		else {
			//send message to all
			snprintf(buff_out, sizeof(buff_out), "User called %s said %s\n", cli->name, buff_in);
			send_message_all(buff_out);

		}
	}
	// Close connection 
	sprintf(buff_out, "User %s is leaving! \n His last words are: %s\n", cli->name, last_word);
	send_message_all(buff_out);
	close(cli->conectfd);

	//delete from queue
	queue_delete(cli->usrid);
	printf("CLient called >> %s << is leaving \n", cli->name);
	free(cli);
	cli_count--;
	pthread_detach(pthread_self());


	return NULL;
}



int getnum(char *mess, int j){
	int num = 0;
	for (int i=j; (mess[i]!='\n')&&(mess[i]!='\0'); i++)
		num = num*10 + ((int)mess[i] - '0');
	return num;
}

int isint(char *mess, int j){
	for (int i = j; (mess[i]!='\n')&&(mess[i]!='\0'); i++){
		if ((mess[i]=='0')||(mess[i]=='1')||(mess[i]=='2')||(mess[i]=='3')||(mess[i]=='4')||(mess[i]=='5')||(mess[i]=='6')||(mess[i]=='7')||(mess[i]=='8')||(mess[i]=='9'));
		else return 0;
	}
	return 1;
}

/*
void handle_sigint(int s){
	char c;
	printf(">> You want to end server ? (Y/N) ");
	if (getchar(c) == 'Y'){
		
	}

}
*/


int main(int argc, char **argv){

	if (argc != 2){
        perror("Error: USE ./a.out [port num]");
        exit(1);
    }

    char buf[BUFLEN];
	int sockfd = 0, newsockfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	static unsigned int cli_count = 0;
	int len;
	pthread_t tid;
	int OK=1;
	int i;

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //for reusing port number

	/* Socket settings */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	/* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);
	//signal(SIGINT, handle_sigint); 

	/* Bind */
	if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) != 0){
		perror("Server-socket binding error");
        exit(2);
	}

	/* Listen */
	if(listen(sockfd, MAX_CLIENTS) < 0){
		perror("Socket listening failed");
		exit(2);
	}
	printf("%s \n","Server is listening");

	/* Accept clients */
	while(1){
		OK=1;
		socklen_t clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

		/* Check if max clients is reached */
		if((cli_count+1) == MAX_CLIENTS){
			printf("<<MAX CLIENTS REACHED\n");
			printf("<<REJECT ");
			
			printf("\n");
			close(newsockfd);
			continue;
		}
		//printf("Client connected !\n");
		while(OK){
			OK=0;
			strcpy(buf, "Hi! Print your name?\n");
			send(newsockfd, buf, BUFLEN, 0);
			if ((len = recv(newsockfd, &buf, BUFLEN, 0))<0){
					perror("server-socket son reading name error");
        			exit(1);
			}
			for(i=0;i<MAX_CLIENTS;i++){
				if(clients[i]){
					if (!strcmp(clients[i]->name, buf)) {
						OK=1;
						strcpy(buf, "We have one user with this name:( please choose another\n");
						send(newsockfd, buf, BUFLEN, 0);
						//return;
					}
				}
			}
		}


		//printf("Client named %s connected !\n", buf);

		//client settings
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->addr = cli_addr;
		cli->conectfd = newsockfd;
		cli->usrid = userid++;
		strcpy(cli->name,buf);
		
		//add client to the queue and handle
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);

		// Reduce CPU usage 
		sleep(1);
	}


	
	shutdown(sockfd,1);
	close(sockfd);
}