#include <string.h>
#include <vector>             //stl vector
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

using namespace std;

#define MAX_MSG_SZ      1024

// Determine if the character is whitespace
bool isWhitespace(char c)
{ switch (c)
    {
        case '\r':
        case '\n':
        case ' ':
        case '\0':
            return true;
        default:
            return false;
    }
}

// Strip off whitespace characters from the end of the line
void chomp(char *line)
{
    int len = strlen(line);
    while (isWhitespace(line[len]))
    {
        line[len--] = '\0';
    }
}

char * GetLine(int fds)
{
    char tline[MAX_MSG_SZ];
    char *line;
    
    int messagesize = 0;
    int amtread = 0;
    while((amtread = read(fds, tline + messagesize, 1)) < MAX_MSG_SZ)
    {

        if (amtread > 0)
        {
            messagesize += amtread;
        }
        else if( amtread == 0 )
        {
            break;
        }
        else
        {
            perror("Socket Error is:");
            fprintf(stderr, "Read Failed on file descriptor %d messagesize = %d\n", fds, messagesize);
            exit(2);
        }
        //fprintf(stderr,"%d[%c]", messagesize,message[messagesize-1]);
        if (tline[messagesize - 1] == '\n')
        {
            break;
        }
    }
    tline[messagesize] = '\0';
    chomp(tline);
    line = (char *)malloc((strlen(tline) + 1) * sizeof(char));
    strcpy(line, tline);
    //fprintf(stderr, "GetLine: [%s]\n", line);
    return line;
}
 
// Change to upper case and replace with underlines for CGI scripts
void UpcaseAndReplaceDashWithUnderline(char *str)
{
    int i;
    char *s;
    
    s = str;
    for (i = 0; s[i] != ':'; i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = 'A' + (s[i] - 'a');
        
        if (s[i] == '-')
            s[i] = '_';
    }
    
}


// When calling CGI scripts, you will have to convert header strings
// before inserting them into the environment.  This routine does most
// of the conversion
char *FormatHeader(char *str, const char *prefix)
{
    char *result = (char *)malloc(strlen(str) + strlen(prefix));
    char* value = strchr(str,':') + 1;
    UpcaseAndReplaceDashWithUnderline(str);
    *(strchr(str,':')) = '\0';
    sprintf(result, "%s%s=%s", prefix, str, value);
    return result;
}

// Get the header lines from a socket
//   envformat = true when getting a request from a web client
//   envformat = false when getting lines from a CGI program

int GetHeaderLines(vector<char *> &headerLines, 
		int skt, bool envformat, bool debug)
{
    // Read the headers, look for specific ones that may change our responseCode
    char *line;
    char *tline;
    
    tline = GetLine(skt);
	
    while(strlen(tline) != 0)
    {
        if (strstr(tline, "Content-Length"))
        {
            if (envformat)
                line = FormatHeader(tline, "");
            else
                line = strdup(tline);
        }
        else
        {
            if (envformat)
                line = FormatHeader(tline, "");
            else
            {
                line = (char *)malloc((strlen(tline) + 10) * sizeof(char));
                sprintf(line, "%s", tline);                
            }
        }
        //fprintf(stderr, "Header --> [%s]\n", line);
        
        headerLines.push_back(line);	
        free(tline);
        tline = GetLine(skt);
    }
	
    free(tline);
    int* pContentLength;
    int contentLength;
    char dummy[MAX_MSG_SZ];
    for (int i = 0; i < headerLines.size(); i++) {
      if(debug){
      	printf("[%d] %s\n",i,headerLines[i]);
      }
      if(strstr(headerLines[i], "Content-Length")) {
      	sscanf(headerLines[i], "%s %d", dummy, pContentLength);
	contentLength = *pContentLength;
      }
    }
    return contentLength;
}


/*main(int argc, char **argv)
{
  int fd;
  vector<char *> headerLines;
  char buffer[MAX_MSG_SZ]; 
  char dummy[MAX_MSG_SZ];
  int * pContentLength;
  int contentLength;

  printf("Web Program Tools Example\n\n");

  // This shows how you could use these tools to implement a web client
  // We will talk about how to use them for the server too

  // Open the file that will simulate the socket
  fd = open("sample.txt",O_RDONLY);
  if(fd < 0) {
        perror("open of sample.txt failed");
        exit(0);
  }

  // First read the status line
    char *startline = GetLine(fd);
  printf("Status line %s\n\n",startline);

  // Read the header lines
    GetHeaderLines(headerLines, fd , false);

  
  // Now print them out
  for (int i = 0; i < headerLines.size(); i++) {
    printf("[%d] %s\n",i,headerLines[i]);
    if(strstr(headerLines[i], "Content-Length")) {
        sscanf(headerLines[i], "%s %d", dummy, pContentLength);
	contentLength = *(pContentLength);
    }
  }

  printf("\n=======================\n");
  printf("Headers are finished, now read the file\n");
  printf("Content Length is %d\n",contentLength);
  printf("=======================\n\n");

  // Now read and print the rest of the file
  int rval;
  while((rval = read(fd,buffer,MAX_MSG_SZ)) > 0) {
        write(1,buffer,rval);
  }
}
 */
