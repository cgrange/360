#include "parse.cpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         10000
#define QUEUE_SIZE          5

using namespace std;

void parse(int fd, char* &contentType, int &contentLength){
   vector<char*> headerLines;
   contentLength = GetHeaderLines(headerLines, fd, false, false);
   char* pFirstHeader = (char*)malloc(strlen(headerLines[0])+1);
   pFirstHeader = headerLines[0];
   string firstHeader(pFirstHeader);
   free(pFirstHeader);
   string path = firstHeader.substr(firstHeader.find("/"), firstHeader.find(" HTTP/")-4);
   cout << "path: " << path << endl;
   cout << "firstHeader: " << firstHeader << endl;
   if(path == "/favicon.ico"){
	cout << "entered the if statement about /favicon.ico\n";
   }
   else{
	//return the path that they're looking for.
   }
}

void serve(int fd){ 
//   printf("Got from browser \n%s\n", pBuffer);
   int contentLength = 0;
   char* contentType;  
   parse(fd, contentType, contentLength);
   char pBuffer[BUFFER_SIZE];   
   memset(pBuffer, 0, sizeof(pBuffer));
   //sprintf(pBuffer, "HTTP/1.1 200 OK\r\nContent-Type:%s\r\nContent-Length:%d\r\n\r\n<html>Hello</html>\n", contentType, contentLength);
   sprintf(pBuffer, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 19\r\n\r\n<html>Hello</html>\n");
   write(fd, pBuffer, strlen(pBuffer));
}

int main(int argc, char* argv[])
{
    int hSocket,hServerSocket;  /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in); 
    int nHostPort;
    char* directory;

    if(argc < 3)
      {
        printf("\nUsage: server host-port directory\n");
        return 0;
      }
    else
      {
        nHostPort=atoi(argv[1]);
	directory = (char*) malloc(strlen(argv[2]) + 1);
	directory = argv[2];
      }

    printf("\nStarting server");

    printf("\nMaking socket");
    /* make a socket */
    hServerSocket=socket(AF_INET,SOCK_STREAM,0);

    if(hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* fill address struct */
    Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    printf("\nBinding to port %d",nHostPort);

    /* bind to a port */
    if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address)) 
                        == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
 /*  get port number */
    getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize);
    printf("opened socket as fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port) );

        printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );


    printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return 0;
    }

    for(;;)
    {
        printf("\nWaiting for a connection\n");
        /* get the connected socket */
        hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);

        printf("\nGot a connection from %X (%d)\n",
              Address.sin_addr.s_addr,
              ntohs(Address.sin_port));
	serve(hSocket); //most the rest of this should be in the serve function
	
	
	printf("\nClosing the socket");
		/* close socket */
        if(close(hSocket) == SOCKET_ERROR)
        {
         printf("\nCould not close socket\n");
         return 0;
        }
    }
}
