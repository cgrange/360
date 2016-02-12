#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <semaphore.h>
#include <signal.h>

sem_t empty, full, mutex;

class myqueue {
	std::queue <int> stlqueue;
	public:
	void push(int socket){
		sem_wait(&empty);
		sem_wait(&mutex);
		stlqueue.push(socket);
		sem_post(&mutex);
		sem_post(&full);
	}
	int pop(){
		sem_wait(&full);
		sem_wait(&mutex);
		int rval = stlqueue.front();
		stlqueue.pop();
		sem_post(&mutex);
		sem_post(&empty);
		return(rval);
	}
} sockqueue; // sockqueue is an instance of myqueue

void *howdy(void *arg){
/* for(;;) {
	socke = dequeue();
	Read Request
	write response
	close
}*/
//	long tid;
//	tid = (long)arg;
//	printf("hi %ld\n",tid);
	for(;;){
		std::cout << "GOT " << sockqueue.pop() << std::endl;
	}
}

void handler(int status);

main(int argc, char* argv[]){
	struct sigaction sigold, signew;
	signew.sa_handler=&handler;
	sigemptyset(&signew.sa_mask);
	sigaddset(&signew.sa_mask,SIGINT);
	signew.sa_flags = SA_RESTART;
	sigaction(SIGINT, &signew, &sigold);
	sigaction(SIGPIPE,&signew,&sigold);
	//for(;;);
	int portNo = atoi(argv[1]);
	int NTHREADS = atoi(argv[2]);
	int NQUEUE = atoi(argv[2]) + 10;
	long threadid;
	pthread_t threads[NTHREADS];
	sem_init(&mutex, PTHREAD_PROCESS_PRIVATE, 1);
	sem_init(&full, PTHREAD_PROCESS_PRIVATE, 0);	
	sem_init(&empty, PTHREAD_PROCESS_PRIVATE, NQUEUE);
	for(int i = 0; i < NQUEUE; i++){
		sockqueue.push(i);
	}
//	exit(0);
	for(threadid = 0; threadid < NTHREADS; threadid++){

		pthread_create(&threads[threadid], NULL, howdy, 
				(void *) threadid);

	}
/*
	Set up socket, bind, listen
	for(;;){
		fd accept
		enqueue(fd)
	}
you'll wrap up the whole queue and force mutual exclusion.
*/
	pthread_exit(NULL);
}


void handler(int status){
	printf("received signal %d\n",status);
}
//pthread_create args : (pthread_t *thread, const pthread_attr_t *attr, void* (*start_routine) (void*), void *arg); 
