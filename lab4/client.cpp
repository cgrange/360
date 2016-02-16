#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <math.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         10000
#define HOST_NAME_SIZE      255
#define NSTD 3

using namespace std;

double standardDeviation(double stdDevNumerator, int count){
	sqrt(stdDevNumerator/(double)(count-1));
}

double numerator(std::vector<double> values, double average){
	double summation = 0;
	while(values.size()>0){
		double value = values.back();
		values.pop_back();
		summation += pow((value-average), 2.0);
	}
	return summation;
}

int  main(int argc, char* argv[])
{
    

    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    string path;
    bool debug = false;
    int count;
	
    if(argc < 5)
      {
        printf("\nUsage: webtest host-name port path -d <count>\n");
        return 0;
      }
    else if(argc == 6){
	
	debug = true;
	count = atoi(argv[5]);

        strcpy(strHostName,argv[1]);
	string dummy(strHostName);
        nHostPort=atoi(argv[2]);
	string dummy2(argv[3]);
	path = dummy2;
	count = atoi(argv[5]);
    }
    else
      {

        strcpy(strHostName,argv[1]);
	string dummy(strHostName);
	
        nHostPort=atoi(argv[2]);
	string dummy2(argv[3]);
	path = dummy2;
	count = atoi(argv[4]);
	//use getopt for the -d and count?
      }
     struct timeval oldtime[count+NSTD];
     int hSocket[count]; //handle to socket

    printf("\nMaking sockets\n");
    /* make a socket */
	for(int i = 0; i < count; i++) {
	
	    hSocket[i]=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

		if(hSocket[i] == SOCKET_ERROR)
		{
			printf("\nCould not make a socket\n");
			return 0;
		}
	}

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

	int epollFD = epoll_create(1);
	// Send the requests and set up the epoll data
	for(int i = 0; i < count; i++) {
		/* connect to host */
		
		if(connect(hSocket[i],(struct sockaddr*)&Address,sizeof(Address)) 
		   == SOCKET_ERROR)
		{
			printf("\nCould not connect to host\n");
			return 0;
		}
		char request[] = "GET /test1.html HTTP/1.0\r\n\r\n";

	    	write(hSocket[i],request,strlen(request));
		// Keep track of the time when we sent the request
		gettimeofday(&oldtime[hSocket[i]],NULL);
		// Tell epoll that we want to know when this socket has data
		struct epoll_event event;
		event.data.fd = hSocket[i];
		event.events = EPOLLIN;
		int ret = epoll_ctl(epollFD,EPOLL_CTL_ADD,hSocket[i],&event);
		if(ret)
			perror ("epoll_ctl");
	}
	double total = 0;
	double stdDevNumerator;
	std::vector<double> values;	
	for(int i = 0; i < count; i++) {
		
		struct epoll_event event;
		int rval = epoll_wait(epollFD,&event,1,-1);
		if(rval < 0)
			perror("epoll_wait");
		rval = read(event.data.fd,pBuffer,BUFFER_SIZE);
		struct timeval newtime;
		// Get the current time and subtract the starting time for this request.
		gettimeofday(&newtime,NULL);
		double usec = (newtime.tv_sec - oldtime[event.data.fd].tv_sec)*(double)1000000+(newtime.tv_usec-oldtime[event.data.fd].tv_usec);	
		if(debug){	
			cout << "Time in microseconds: "<<usec<<endl;
			printf("got %d from %d\n",rval,event.data.fd);
		}
		values.push_back(usec);
		total += usec;
		
		//TODO calculate standard deviation.
		
		// Take this one out of epoll
		epoll_ctl(epollFD,EPOLL_CTL_DEL,event.data.fd,&event);
	}
	double average = total/(double)count;
	double stdDev = standardDeviation(numerator(values, average), count);
	cout << "average response time in microseconds: " << average << endl;
	cout << "standard deviation: " << stdDev << endl;
	// Now close the sockets
	for(int i = 0; i < count; i++) {	
		
		/* close socket */                       
		if(close(hSocket[i]) == SOCKET_ERROR)
		{
			printf("\nCould not close socket\n");
			return 0;
		}
	}
}
