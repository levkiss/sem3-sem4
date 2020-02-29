#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define PORTNUM 8086 //port number
#define SIZEBUF 100
#define IPADRESS "127.0.0.1"
#define QUIT_CMD "\\quit"

//char message[SIZEBUF];
//char buf[SIZEBUF];

void receive_msg(int sockfd) {
    //close(1);
    char buf[SIZEBUF] = {0};
    int mslen = 0; 
    while ((mslen = recv(sockfd, buf, SIZEBUF-1, 0))) {
        buf[mslen] = '\0'; 
        if (!strcmp(buf, "Hi! What is your name?\n")){
            printf("The server asked your name, print it (not more 32 sym) >> ");
            scanf("%s", buf);
            if (strlen(buf)>32){
                perror("client-socket name is longer than 32");
                exit(1);
            }
            if(send(sockfd, buf, strlen(buf)+1, 0) < 0) 
                perror("send error");
        }
        else { 
            printf("%s", buf);
            printf("> ");
            //fflush(stdout);
        }
    }
}

void send_msg(int sockfd, int pid) {
    //close(0);
    printf("> ");

    char *send_text = "  Hi. ";
    char str[SIZEBUF] = {0};
    char res[SIZEBUF] = {0};
    int out=0;

    while (fgets(str, SIZEBUF, stdin) == str) {
        if ((strncmp(str, QUIT_CMD, strlen(QUIT_CMD)) == 0)){
            printf("> Goodbye! You are leaving ");
            if (out == 1) break;
            out =1;
        }
        else
            printf("\n> ");

        if (strlen(str) > 1024)
            printf("Your sentence is too big. Use maximum 1024 symbols!");
        else {
            int j = 0;
            int i;
            for (i = 0; i<strlen(str) ; i++) {
                if ((str[i] == ' ') || (str[i]=='\t')) {
                    if (j==0) continue;
                    if (str[i+1] == ' ') continue;
                    if (str[i+1] == '\t') continue;
                }
                res[j] = str[i];
                j++;
            }
            i=strlen(res);
            if ((res[i-2] == ' ') || (res[i-2] == '\t'))
                res[i-2] = '\0';
            if (res[i-1]=='\n')
                res[i-1]='\0';   
            if(send(sockfd, res, strlen(str)+1, 0) < 0) perror("send error");
        }
    }
    kill(pid, SIGKILL);
    printf("Goodbye.\n");
}



int main(int argc, char **argv)
{
    if (argc != 2){
        perror("Error: USE ./a.out [port num]");
        exit(1);
    }

    int sockfd;
    int enddd;
    struct sockaddr_in addr;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))<0) { //creating socket
    	perror("client-socket creation error");
        exit(1);
    }
    /*
    addr.sin_family = PF_INET;
    addr.sin_port = htons(atoi(argv[1])); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(IPADRESS);
    */
    struct in_addr server_addr;
    if(!inet_aton(IPADRESS, &server_addr)) perror("inet_aton");


    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, &server_addr, sizeof(server_addr));
    addr.sin_port = htons(atoi(argv[1]));

    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("client-socket connection error");
        exit(2);
    }
 	//printf("Enter you message >>\t");

    int pid;    
    if ((pid = fork())) //father
        send_msg(sockfd, pid);
    else //son
        receive_msg(sockfd);
    /*while (gets(message)){
    	send(sockfd, message, SIZEBUF, 0);
    	printf("Message sent\t");
    	enddd= (!strcmp(message,"\\-"));
    	printf("waiting for the answer\n");
    	recv(sockfd, message, SIZEBUF,0);
    	printf("Client: server answered: %s\n", message);
    	if (enddd){
    		printf("Client: ending! BYE\n");
    		break;
    	}
    	printf("Enter you message >>\t");
    //recv(sockfd, buf, sizeof(message), 0);
    } */
    //printf(buf);
    shutdown(sockfd,1);
    close(sockfd);
    
    return 0;
}
