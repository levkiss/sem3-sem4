#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>

#define NMAX 5

//int *shmaddr;
//int shmid;
/*
void handler(int sig) {
	printf("The tower height is -> %d\n", *shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    exit(-1);
}
*/
int main () {
    int msgid, shmid;
	int *shmaddr;
    key_t key;
    int number;
	struct Message {
		long type;
		int ball;
	} mess;

	key=ftok("1",128);
	shmid = shmget(key, NMAX, 0666 | IPC_CREAT);
     /* создаем разделяемую память на NMAX элементов*/
    shmaddr = shmat(shmid, NULL, 0);

	msgid = msgget(IPC_PRIVATE,  0666|IPC_CREAT);
	
	if (fork()==0) {
		// kid 1
        mess.type=1;
        mess.ball=0;
        *shmaddr = 0;
        printf("Kid1 == %d\n", *shmaddr);
		msgsnd (msgid, &mess, sizeof(mess.ball),0);
		sleep(1);
		
		while (1) {
			//mess.type=2;
			msgrcv(msgid, &mess,sizeof(mess.ball), 2,0);
		    mess.type=1;
		    mess.ball+=1;
		    *shmaddr+=1;
		    printf("Kid1 == %d\n", *shmaddr); 
			msgsnd (msgid, &mess, sizeof(mess.ball),0);
			sleep(1);
		}
	}
		
		
	
	if (fork()==0) {
		//kid 2 
		while (1) {
			//mess.type=1;
			msgrcv(msgid, &mess,sizeof(mess.ball),1,0);
			mess.type=2;
            mess.ball+=1; 
            *shmaddr+=1;
		    printf("Kid2 == %d\n", *shmaddr);
			//printf("Pong %d\n", mess.ball);
		    msgsnd (msgid, &mess, sizeof(mess.ball),0);
            sleep(1);
		}
	}
	//sleep(10);
	//signal (SIGINT, handler);
	signal(SIGINT,SIG_IGN);
	pause();
	wait(NULL);
	wait(NULL);

	printf("The tower height is -- %d\n", *shmaddr);
	
	shmdt(shmaddr) ; /* отключаемся от разделяемой
     памяти */
    shmctl(shmid, IPC_RMID, NULL);
     /* уничтожаем разделяемую память */
	msgctl(msgid,  IPC_RMID,  0); // deleting очередь messages
	
	return 0;
}