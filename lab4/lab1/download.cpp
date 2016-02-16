#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.cpp"
#include <sys/epoll.h>
#include <sys/time.h>
#include <iostream>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

#define NCONNECTIONS 20
int  main(int argc, char* argv[])
{
	struct timeval oldtime[30];
	struct timeval newtime[30];
	int hSocket[NCONNECTIONS];
    //int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;  
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    char path[1024];
    bool debug = false;
    int timesToDownload = 1;
    bool count = false;
	setbuf(stdout, NULL);

    if(argc < 4)// idk why he changed it to 6?
		// I think it's because of the -c count and that junk
		// but we'll probably run into issues if those aren't in
		// it should probably be if argc > 6. print the error.
      {
        printf("\nUsage: host-name port-number URL\n");
        return 0;
      }
    else
      {
	extern char* optarg;
	int c, err = 0;
	debug = false;
	while ((c = getopt(argc, argv, "c:d")) != -1){
		switch(c){
			case 'c':	
				timesToDownload = atoi(optarg);
				count = true;
				break;
			case 'd':
				debug = true;
				break;
			case '?':
				err = 1;
				break;
		}
	}	
	strcpy(strHostName, argv[optind]);
	nHostPort = atoi(argv[optind + 1]);
	strcpy(path, argv[optind + 2]);
      }

	int epollfd = epoll_create(1);// idk if that's in the right place
   for(int i = 0; i<NCONNECTIONS; i++){
    /* make a socket */
    hSocket[i]=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket[i] == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    //printf("\nConnecting to %s (%X) on port %d",strHostName,nHostAddress,nHostPort);

    /* connect to host */
    if(connect(hSocket[i],(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
	struct epoll_event event;
	event.data.fd = hSocket[i];
	event.events = EPOLLIN;
	int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, hSocket[i], &event);
    char request[] = "GET /foo.html HTTP/1.0\r\n\r\n";
    write(hSocket[i],request,strlen(request));
   }
   for(int i = 0; i < NCONNECTIONS; i++){
	struct epoll_event event;
	int nr_events = epoll_wait(epollfd, &event, 1,-1);
	if(nr_events < 0)
		perror("epoll_wait");
	
	char pBuffer[BUFFER_SIZE];
	read(event.data.fd, pBuffer, BUFFER_SIZE);
	printf("got from %d\n", event.data.fd);
	struct timeval newtime;
	gettimeofday(&newtime,NULL);
	double usec = (newtime.tv_sec - oldtime[event.data.fd].tv_sec)*(double)1000000 + (newtime.tv_usec-oldtime[event.data.fd].tv_usec);
	std::cout << "Time " << usec/1000000 << std::endl;
	char buffer[10000];
	int rval = read(event.data.fd,buffer,10000);
	printf("Got %d from %d\n Closing socket\n", rval, event.data.fd);
	if(close(event.data.fd) == SOCKET_ERROR){
		printf("\nCould not close socket\n");
		return 0;
	}
   }
    /*int sizeOfMessage = strlen(path) + strlen(strHostName) +strlen("GET  HTTP/1.1\r\nHOST: \r\n\r\n") +1;
    char* message = (char*)malloc(sizeOfMessage);
    sprintf(message, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", path, strHostName);
    if(count){
	int downloads = 0;	
	for(int i = 0; i < timesToDownload; i++){
	  write(hSocket, message, strlen(message)); 
	  vector<char *> headerLines;
	  GetHeaderLines(headerLines, hSocket, false, debug);
	  if(strstr(headerLines[0], "OK")){
	 	downloads++;
	  }
	  printf("%d pass(es) through the for loop\n", i);
	}
	free(message);
	printf("succesfully downloaded the site %d times.\n", downloads);
    }
    else{
    	write(hSocket, message, strlen(message));
    	free(message);
    	vector <char *> headerLines;
    	int contentLength = GetHeaderLines(headerLines, hSocket, false, debug);
    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    /*
    	int amountRead = 0;
    	while(amountRead < contentLength){
	  char myBuffer[1];
	  nReadAmount = read(hSocket, myBuffer, 1);
	  amountRead += nReadAmount;
	  if(nReadAmount > 0){
	    cout << myBuffer[0];
	  }
    	}
    }
    // printf("\nReceived \"%s\" from server\n",pBuffer);
    /* write what we received back to the server */ 
/*	
    printf("\nClosing socket\n");
    /* close socket */                       
   /* if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }*/
}

