#include <dirent.h>
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
#include <sys/stat.h>
//#include "stat.cpp"

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         10000
#define QUEUE_SIZE          5

using namespace std;

string clDir = "";
size_t npos = -1;

string parse(int fd){
   vector<char*> headerLines;
   int contentLength = GetHeaderLines(headerLines, fd, false, false);
   char* pFirstHeader = (char*)malloc(strlen(headerLines[0])+1);
   pFirstHeader = headerLines[0];
   string firstHeader(pFirstHeader);
   free(pFirstHeader);
   string path = firstHeader.substr(firstHeader.find("/"), firstHeader.find(" HTTP/")-4);
   cout << "original path: " << path << endl;
   std::size_t found = path.find(clDir);
   if (found!=npos){
	string finalPath = path.substr(clDir.length(), path.length() - clDir.length());
  	cout << "final path: " << finalPath << endl;
	path = finalPath;
   }
   //found=str.find("needles are small",found+1,6);  
   cout << "firstHeader: " << firstHeader << endl;
   return path;
}

void outputPath(string argDir, string path, int fd){
   string fullPath = argDir + path;
   cout << "full path is: " << fullPath << endl;
	char* filePath = (char*) malloc(fullPath.size()+1);
	strcpy(filePath, fullPath.c_str());
	struct stat filestat;

	if(stat(filePath, &filestat)) {
	   cout << "ERROR in stat file wasn't found\n";
	   char* canned404 = (char*) malloc(1024);
	   char* synPath = (char*) malloc(path.size()+1);
	   strcpy(synPath, path.c_str());
	   sprintf(canned404,"HTTP/1.1 404 Not Found\r\n\r\n<html><head><title>Page Not Found</title></head><body> <h1>404 page not found</h1> <br/> <br/> <p>the requested page \"%s\" could not be found.</p>	</body>	</html>", synPath);
	   write(fd, canned404, strlen(canned404));
	   free(canned404);
	}
	else if(S_ISREG(filestat.st_mode)) {
	   if(strstr(filePath, ".txt")){	
		cout << filePath << " is a text file \n";
		//cout << "file size = "<< filestat.st_size <<"\n";
		int contentLength = filestat.st_size;
		FILE *fp = fopen(filePath,"r");
		char *buffer = (char *) malloc(filestat.st_size + 1);
		fread(buffer, filestat.st_size, 1, fp);
		char pBuffer[BUFFER_SIZE];
		memset(pBuffer, 0, sizeof(pBuffer));
		sprintf(pBuffer, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s\n", contentLength, buffer);
		write(fd, pBuffer, strlen(pBuffer));
		free(buffer);
		fclose(fp);
	   }
	   else if(strstr(filePath, ".html")){
		cout << filePath << " is an html file \n";
		//cout << "file size = "<< filestat.st_size <<"\n";
		int contentLength = filestat.st_size;
		FILE *fp = fopen(filePath,"r");
		char *buffer = (char *) malloc(filestat.st_size + 1);
		fread(buffer, filestat.st_size, 1, fp);
		char pBuffer[BUFFER_SIZE];
		memset(pBuffer, 0, sizeof(pBuffer));
		sprintf(pBuffer, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s\n", contentLength, buffer);
		write(fd, pBuffer, strlen(pBuffer));
		free(buffer);
		fclose(fp);
	   }
	   else if(strstr(filePath, ".gif")){
		char pBuffer [10000];
		memset(pBuffer,0,sizeof(pBuffer));
		int contentLength = filestat.st_size;
		sprintf(pBuffer,"HTTP/1.1 200 OK\r\n\
	Content-Type: image/gif\r\n\
	Content-Length: %d\
	\r\n\r\n", contentLength);
		write(fd,pBuffer, strlen(pBuffer));
		FILE *fp = fopen(filePath,"r");
		char *buffer = (char *)malloc(contentLength+1);
		fread(buffer, contentLength, 1,fp);
		write(fd,buffer,contentLength);
		free(buffer);
		fclose(fp);
	   }
	   else if(strstr(filePath, ".jpg") || strstr(filePath, ".jpeg")){
		char pBuffer [10000];
		memset(pBuffer,0,sizeof(pBuffer));
		int contentLength = filestat.st_size;
		sprintf(pBuffer,"HTTP/1.1 200 OK\r\n\
	Content-Type: image/jpg\r\n\
	Content-Length: %d\
	\r\n\r\n", contentLength);
		write(fd,pBuffer, strlen(pBuffer));
		FILE *fp = fopen(filePath,"r");
		char *buffer = (char *)malloc(contentLength+1);
		fread(buffer, contentLength, 1,fp);
		write(fd,buffer,contentLength);
		free(buffer);
		fclose(fp);
	   }
	}
	else if(S_ISDIR(filestat.st_mode)) {
		cout << filePath << " is a directory \n";	  
		DIR* dirp;
 		struct dirent* dp;
		dirp = opendir(filePath);
		char* cPath = (char*) malloc(path.length()+1);
		strcpy(cPath,path.c_str());
		// for concatenation
		string list = "";
		while ((dp = readdir(dirp)) != NULL){   			
		   string fileName(dp->d_name);
		   if(fileName == "index.html"){
			string argDir(filePath);
			string path = "/index.html";
			string newPath = argDir + path;
			cout << "entered my index.html code. the path I'm giving it is: " << newPath << endl;
			outputPath(argDir, path, fd);
		   }
		   char* pListItem = (char*) malloc(strlen(dp->d_name)*2+strlen(filePath)+30);
		   sprintf(pListItem, "<a href=\"%s/%s\"><li>%s</li></a>", filePath, dp->d_name, dp->d_name);
		   string listItem(pListItem);
		   free(pListItem);
		   list += listItem;
		}
		(void)closedir(dirp);
		char* cList = (char*) malloc(list.length()+1);
		strcpy(cList, list.c_str());
		char endHtml[20];
		memset(endHtml, 0, 20); 
		sprintf(endHtml, "</ul></body></html>");		
		char * new_str ;
		int contentLength = path.length() + strlen(cList) + strlen(endHtml) + 160;
		char* header = (char*) malloc(strlen(cPath)+160);
		sprintf(header,"<HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n<html><head><title>directory</title></head> <body> <h1>contents of %s</h1> <ul>", contentLength,cPath);
		if((new_str = (char*) malloc(contentLength)) != NULL){
    		   new_str[0] = '\0';   // ensures the memory is an empty string
    		   strcat(new_str,header);
    		   strcat(new_str,cList);
		   strcat(new_str,endHtml);
		   write(fd,new_str,strlen(new_str));	
		   free(header);
		   free(cList);
		   free(cPath);
		} else {
    		   printf("malloc failed!\n");
    		   // exit?
		}
		
	}
	free(filePath);
}

void serve(int fd, string argDir){ 
//   printf("Got from browser \n%s\n", pBuffer);
   clDir = argDir; 
   string path = parse(fd);
   cout << "clDir = " << clDir << endl;
   if(path == "/favicon.ico"){
	cout << "entered the if statement about /favicon.ico\n";
	return;	
   }
   //stat junk here
   outputPath(argDir, path, fd);
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
	serve(hSocket, argv[2]); //most the rest of this should be in the serve function
	
	
	printf("\nClosing the socket");
		/* close socket */
        if(close(hSocket) == SOCKET_ERROR)
        {
         printf("\nCould not close socket\n");
         return 0;
        }
    }
}
