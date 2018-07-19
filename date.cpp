#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string.h>

using namespace std;

/**
Sets up and prints out formatted time string
@param argc number of args
@param argv string of args
return exit value
*/
int main (int argc, char * argv[])
{
  char buf[10000];
  setvbuf(stdout,buf,_IONBF,sizeof(buf));
  cout.setf(ios::unitbuf);
  if (argc == 2 && argv[1][0] == '+') {
    int length = strlen(argv[1]);
    char * stringformat = new char [length-1];
    for (int i = 0; i < length - 1; i++) {
      stringformat[i] = argv[1][i+1];
    }
    time_t rawtime;
    struct tm *info;
    char buffer[200];

    time(&rawtime);
  
    info = localtime(&rawtime);

    strftime(buffer,200,stringformat, info);
    printf("%s\n", buffer );
    delete[] stringformat;
  } else if (argc > 2) {
    cout << "date: extra operand '" << argv[2] << "'" << endl;    
  } else if (argc == 1) {
    time_t rawtime;
    struct tm *info;
    char buffer[200];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer,200,"%a %b %d %T %Z %Y",info);
    printf("%s\n",buffer);
  } else {
    cout << "date: invalid date '" << argv[1] << "'" << endl;
  } 
  
  return(0);
}
