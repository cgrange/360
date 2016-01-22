#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strhostname[host_name_size];
    int nHostPort;
    char* path;
    bool debug;
    int times-to-download = 1;

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
				std::cout << optarg << std::endl;
				times-to-download = atoi(optarg);
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
	path = (char*)malloc(strlen(argv[optind + 2]));
      }
	

    printf("\nMaking a socket");
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

    printf("\nConnecting to %s (%X) on port %d",strHostName,nHostAddress,nHostPort);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
    
    char* message = "GET <insert path here> HTTP/1.0\r\nHOST: <insert host name here> \r\n\r\n";
    write(hSocket, message, strlen(message));

    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
    printf("\nReceived \"%s\" from server\n",pBuffer);
    /* write what we received back to the server */ 

    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
}

