#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
//#include < unistd.h>
#include <string>

std::queue<int> work;
sem_t work_to_do;
sem_t mutex;
sem_t space_on_q;

/*struct thread_params{
	long thread_id;
	std::string dir;
}*/

void* serve(void* arg){
   long thread_id = long(arg);
   std::cout << "I'm thread " << thread_id << std::endl;
   

   for(;;)
   {
	   sem_wait(&work_to_do);
	   sem_wait(&mutex);
	   
	   int my_conn = work.front();
	   work.pop();
	   std::cout << thread_id  << " working on " << my_conn << std::endl;

	   sem_post(&mutex);
	   sem_post( &space_on_q);
   }

   //perform normal request serving stuff
}

int main(int argc, char* argv[])
{
   int queue_size = 20;
   sem_init(&space_on_q,0, queue_size);
   sem_init(&work_to_do, 0, 0);
   sem_init(&mutex,0, 1);

   int num_threads = 10;
   std::cout << "threads hello!" << std::endl;
   pthread_t threads[num_threads];
   //std::string dir = "something/";
   for(long i = 0 ; i < num_threads; i++){
	//struct thread_params tp;
	//tp.thread_id = i;
	//tp.dir = dir;
	int ret_val = pthread_create(&threads[i], 0, serve, (void*) i );
   }

   // socket()
   // bind()
   // listen()


   for(int i = 0 ;; i++){
	// int i = accept(...);
	sem_wait( &space_on_q);
	sem_wait( &mutex);

	work.push(i);
	std::cout << "pushed " << i << std::endl;

	sem_post( &mutex);
	sem_post( &work_to_do);
   }	   
   return 0;
   // critical sections are the places where code wants to acces and jimmy with the code
}
