#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.cpp"

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
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
	 
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
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
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
    int sizeOfMessage = strlen(path) + strlen(strHostName) +strlen("GET  HTTP/1.1\r\nHOST: \r\n\r\n") +1;
    char* message = (char*)malloc(sizeOfMessage);
    sprintf(message, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", path, strHostName);
    if(count){
	int downloads;	
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

    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
}

