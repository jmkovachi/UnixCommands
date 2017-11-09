#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <ctype.h>
#include <string.h>

//int stdin = 0;
int fflag = 0;

using namespace std;

/**
This method prints the trailing lines of a given file.
@param pathname The name of the path of the file to print.
@param lines The number of lines to print.
*/
void printlines(const char * pathname, int lines) {
	cout.setf(ios::unitbuf);
	int n;
	//cout << pathname << endl;
  if (strcmp(pathname,"-") == 0) {
	  //cout << "HI" << endl;
	  n = STDIN_FILENO;
  }
  else {
	n = open(pathname, O_RDONLY);
	if (n == -1) {
		perror("tail");
		return;
	}
  }
  char buffer[100000];
  int x = 0;
  int bytesum = 0;
  if (strcmp(pathname,"-") == 0) {
	//int bytessum = 0;
	int y = 0;
	char copybuf[100000];
	int i = 0;
	//cout << "HI" << endl;
	while((y = read(STDIN_FILENO,copybuf,1)) > 0){
		//cout << "HI" << endl;
		buffer[i] = copybuf[0];
		//cout << buf[i];
		bytesum += y;
		i++;
	}
  }
  else {
	while ((x = read(n,buffer,100000)) > 0) {bytesum += x;} //bytesum is sum of bytes in file
  }
  //cout << buffer << endl;
  //cout << "HI" << endl;
  //cout << buffer << endl;
  //char buf;
 
  int linecount = 1;
   lseek(n,0,SEEK_SET);
 // while (read(n,&buf,1) > 0) { if (buf == '\n') linecount++;} //counts number of lines in file
 for (int i = 0; i < bytesum; i++) {
	 if (buffer[i] == '\n') {
		 linecount++;
	 }
 }
  //cout << linecount << endl;
  int * lineindex = new int [linecount];
  
 // cout << bytesum << endl;
  int linecount2 = 0;
  for (int i = 0; i < bytesum; i++) {
    if (buffer[i] == '\n') {
      lineindex[linecount2] = i; //stores index of newline character into an int
      linecount2++; //increments to next index of lineindex array
    }
  }
  //cout << lineindex[linecount - 1] << endl;
  if (lines > linecount) {
	//cout << "h" << endl;
	lseek(n,0,SEEK_SET);
  }
  else {
	 //cout << "HI" << endl;
	lseek(n,lineindex[(linecount-lines)-2]+1,SEEK_SET);
  }
  
  if (strcmp(pathname,"-") == 0) {
	  cout << endl;
	  int z;
	  if (lines > linecount) {
		  z = 0;
	  }
	  else {
		  z = lineindex[(linecount-lines)-2];
		}
	  for (int i = 0; i < bytesum; i++) {
		  cout << buffer[z];
		  z++;
	  }
	  cout << endl;
  }
  else {
	  char buff [10000];
	  int y = 0;
	  while((y = read(n,buff,10000)) > 0){}
	  if (y == -1) {
		perror("read");
	  }
	  cout << buff;
  }
  if (fflag == 1) {
	 // cout << "hi" << endl;
	  char newbuf[1000];
	  while ((x = read(n,newbuf,1)) != -1) {
		if (x > 0) {
			write(STDOUT_FILENO,newbuf,1);
		}  
	  }
  }
  //now seek for number of lines - int lines. Read from there
  delete[] lineindex;
}

/**
This method is used to print the number of bytes specified from the end of the file.
@param pathname The name of the path of the file to print.
@param lines The number of lines to print.
*/
void printbytes(const char * pathname, int bytes) {
  cout.setf(ios::unitbuf);
  int n;
  if (strcmp(pathname,"-") == 0) {
	  //cout << "HI" << endl;
	  n = STDIN_FILENO;
  }
  else {
	n = open(pathname, O_RDONLY);
	if (n == -1) {
		perror("tail");
		return;
	}
  }
  int filesize = lseek(n,0,SEEK_END);
  //  cout << "FILESIZE: " << filesize << endl;
  int endbytes = filesize-bytes;
  if (bytes > filesize) {
    endbytes = filesize;
    int x = lseek(n,0,SEEK_SET);
	//cout << x << endl;
	if (x == -1) {
		perror("lseek");
	}
  }
  else {
    lseek(n,endbytes,SEEK_SET);
  }
  char buf [100000];
  // while (x = read(n,buf,100) != 0) write(STDOUT_FILENO,buf,x);
  //int sum = 0;
  //int bytesread;
  int x = 0;
  if (strcmp(pathname,"-")) {
	  for (int z = endbytes; z < filesize; z++) {
		  cout << buf[z];
	  }
  }
  if (strcmp(pathname,"-") == 0) {
	//int bytessum = 0;
	int y = 0;
	char copybuf[100000];
	int i = 0;
	filesize = 0;
	//cout << "HI" << endl;
	while((y = read(STDIN_FILENO,copybuf,1)) > 0){
		//cout << "HI" << endl;
		buf[i] = copybuf[0];
		//cout << buf[i];
		filesize += y;
		i++;
	}
	//cout << buf << endl << "hi" << endl;
	endbytes = filesize-bytes;
	if (bytes > filesize) {
		endbytes = 0;
	}
	//cout << endl << endbytes << endl;
	//cout << buf[endbytes] << endl;
  }
  else {
	  //cout << lseek(n,0,SEEK_CUR) << endl;
	  while((x = read(n,buf,100000)) > 0){}
	  if (x == -1) {
		perror("read");
	  }
	  cout << buf;
  }
  if (strcmp(pathname,"-") == 0) {
	 // cout << "HI" << endl;
	  //cout << endl;
	  for (int z = endbytes; z < filesize; z++) {
		  //cout << endl << buf[z];
		  cout << buf[z];
	  }
  }
  if (fflag == 1) {
	  char newbuf[10000];
	  while ((x = read(n,newbuf,1)) != -1) {
		if (x > 0) {
			write(STDOUT_FILENO,newbuf,1);
		}  
	  }
  }
  //write(STDOUT_FILENO,buf,endbytes);
}


/**
This is the main method of the function that handles the arguements and options.
@param argc The number of arguments.
@param argv An array containing the arguements.
@return Returns 0 on success.
*/
int main(int argc, char * argv[]) {
  //printlines(argv[2],atoi(argv[1]));
	
	if (argc < 6) {
	  //char ** options = new char * [argc-1];
	  //int i = 0;
	  //cout << "HI" << endl;
	  /*while (getopt(argc,argv,options[i]) != -1) {
		i++;
		if (i == argc-2) {
			break;
		}
	  }*/
	  //cout << options[0];
	  //int fflag = 0;
	  int cflag = 0;
	  int nflag = 0;
	  int number = 0;
	  string filename;
	  int filecount = 0;
	  for (int j = 1; j < argc; j++) {
		if (strcmp(argv[j],"-f") == 0) {
		  fflag = 1;
		}
		else if (strcmp(argv[j],"-n") == 0 || strcmp(argv[j],"-c") == 0) {
			if (nflag != 1 && cflag != 1) {
				//check if options[j+1] is digit using while loop
				bool digbool = false;
				if (j!= argc-1) {
					int strl = strlen(argv[j+1]);
					int y = 0;
					while(y < strl && isdigit(argv[j+1][y])) {
						y++;
					}
					if (y == strl) {
						digbool = true;
						j++;
					}
				}
				if (j != argc - 1 && digbool) {
					if (strcmp(argv[j-1],"-n") == 0) {
						nflag = 1;
					}
					else {
						cflag = 1;
					}
					number = atoi(argv[j]);
				}
				else if (j == argc-1) {
					//cout << "HI" << endl;
					if (strcmp(argv[j-1],"-n") == 0) {
						nflag = 1;
					}
					else {
						cflag = 1;
					}
					int strl = strlen(argv[j]);
					int y = 0;
					while(y < strl && isdigit(argv[j][y])) {
						y++;
					}
					if (y == strl) {
						digbool = true;
						number = atoi(argv[j]);
						j++;
					}
					if (digbool) {
						filename = "-";
					}
					//stdin = 1;
				}
				else {
					cout << "Format incorrect." << endl;
					return 0;
				}
			}
			if (nflag == 1 && cflag == 1) {
				cout << "Too many options specified. Use one of -n or -c." << endl;
				return 0;
			}
		}
		else {
			int n = 0;
			char c = argv[j][0];
			if (c == '-') {
				cout << "tail: invalid option " << argv[j] << endl;
				return 0;
			}
			if ((n = open(argv[j],O_RDONLY)) == -1) {
				perror("tail");
				close(n);
				return 0;
			}
			else if (filecount > 0) {
				cout << "Too many filenames entered." << endl;
				return 0;
			}
			else {
				string file(argv[j]);
				filename = file;
				filecount++;
			}
		}
	  }
	  //int count = 0;
	  if (nflag == 1) {
		  printlines(filename.c_str(),number);
	  }
	  else if (cflag == 1) {
		  printbytes(filename.c_str(),number);
	  }
	  else if (argc == 2 && strcmp(argv[1],"-f") == 0) {
		 // cout << "HI" << endl;
		printlines("-",10);  
	  }
	  else if (argc == 1) {
		  printlines("-",10);
	  }
	  else if (argc == 2 && strcmp(argv[1],"-") == 0) {
		  printlines("-",10);
	  }
	  else if (argc == 2) {
		  printlines(filename.c_str(),10);
	  }
	}
	else {
		cout << "Too many options entered." << endl; 
	}
}

