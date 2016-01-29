#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <dirent.h>

using namespace std;

//main(int argc, char **argv)
void typeOfFile(string path)
{
	char* filePath = (char*) malloc(path.size()+1);
	strcpy(filePath, path.c_str());
	struct stat filestat;

	if(stat(filePath, &filestat)) {
		cout << "ERROR in stat file wasn't found\n";
	char* canned404 = (char*) malloc(1024);
	sprintf(canned404,"<html><head><title>Page Not Found</title></head><body> <h1>404 page not found</h1> <br/> <br/> <p>the requested page \"%s\" could not be found.</p>	</body>	</html>", path);
	write( canned404);
	free(canned404);
	}
	if(S_ISREG(filestat.st_mode)) {
		cout << filePath << " is a regular file \n";
		cout << "file size = "<< filestat.st_size <<"\n";
		FILE *fp = fopen(filePath,"r");
		char *buffer = (char *) malloc(filestat.st_size + 1);
		fread(buffer, filestat.st_size, 1, fp);
		printf("Got\n%s", buffer);
		free(buffer);
		fclose(fp);
	}
	//if(S_ISDIR(filestat.st_mode)) {
	//	cout << filePath << " is a directory \n";	  
	//	DIR* dirp;
  	//	struct dirent* dp;
  	//	dirp = opendir(filePath);
 	//	while ((dp = readdir(dirp)) != NULL)
   	//		printf("name %s\n", dp->d_name);
	//	(void)closedir(dirp);
	}
	free(filePath);
}
