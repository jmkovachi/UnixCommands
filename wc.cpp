#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <cstring>
#include <clocale>
#include <cwchar>
#include <ctype.h>

const size_t BUFFSIZE = 1024;
bool cflag = false;
bool mflag = false;
bool lflag = false;
bool wflag = false;
int flagcount = 0;
int filecount = 0;
int totalline = 0;
int totalword = 0;
int totalchar = 0;
int totalbyte = 0;

using namespace std;

/**
This method returns the number of lines in a given file.
@param fd The file descriptor of the file
@param buf A pointer to a char that contains the contents of the file.
@param bytessum The number of bytes in the given file.
@return The number of lines in the given file.
*/
int lines(int fd, char * buf,int bytessum) {
	int linecount = 0;
	for (int i = 0; i < bytessum; i++) {
		if (buf[i] == '\n') {
			linecount++;
		}
	}
	totalline += linecount;
	return linecount;
}

/**
This method is used to return the number of words in a given file.
@param fd The file descriptor of the file
@param buf A pointer to a char that contains the contents of the file.
@param bytessum The number of bytes in the given file.
@return The number of words in the given file.
*/
int words(int fd, char * buf, int bytessum) {
	int wordcount = 0;
	//cout << buf << endl;
	for (int i = 0; i < bytessum; i++) {
		int icount = i;
		if (!isspace(buf[i])) {
			while(!isspace(buf[icount])) {
				icount++;
				if (icount == bytessum) {
					wordcount++;
					break;
				}
			}
			if (icount == bytessum) {
				break;
			}
			i = icount;
			wordcount++;
		}
	}
	totalword += wordcount;
	return wordcount;
}

/**
This method is used to get the number of bytes in a given file.
@param fd The file descriptor of the file
@param buf A pointer to a char that contains the contents of the file.
@param bytessum The number of bytes in the given file.
@return The number of bytes in the given file.
*/
int bytes(int fd,char * buf,int bytessum) {
	totalbyte += bytessum;
	return bytessum;
}

/**
This method is used to obtain the number of characters in a given file.
@param fd The file descriptor of the file
@param buf A pointer to a char that contains the contents of the file.
@param bytessum The number of bytes in the given file.
@return The number of characters in the given file.
*/
int chars(int fd, char * bbuf, int bytessum) {
  std::setlocale(LC_ALL, "en_US.utf8");
  //int nbytes = 0;
  //int chars = 0;
  char newbuf[100000];
  lseek(fd,0,SEEK_SET);
  size_t nbytes = 0;                           // # bytes read
  size_t nchars = 0;                           // # characters read
  while ((nbytes = read(fd, newbuf, BUFFSIZE)) > 0) {
    std::mbstate_t state = std::mbstate_t();   // initialize state
    size_t len = 0;                            // # bytes processed
    wchar_t wc = '\0';                         // multibyte character
    size_t sum = 0;                            // sum # bytes processed
    while((len = std::mbrtowc(&wc, newbuf+sum, nbytes-sum, &state)) > 0) {
      if (len == (size_t) -2) break;           // incomplete wchar
      //std::wcout << "Next " << len << " bytes are the character " << wc << std::endl;
      sum    += len;                           // accumulate sum
      nchars += 1;                             // increment character count
    } // while
    // handle incomplete wchar at end of buffer by seeking back a little
    if (sum < nbytes) lseek(fd, sum-nbytes, SEEK_CUR);
  }
  totalchar += nchars;
  return nchars;
}

/**
This method is used to actually list out a file and its properties.
@param a_option True if the a option is specified in the arguements.
@param l_option True if the l option is specified in the arguements.
@param file The file who's properties are going to be displayed.
*/
int main(const int argc, const char * argv []) {
	cout.setf(ios::unitbuf);
  /*std::setlocale(LC_ALL, "en_US.utf8");
  const char * filename = argv[1];
  char buffer [BUFFSIZE];
  int fd = open(filename, O_RDONLY);
  int nbytes = 0;
  int chars = 0;
  while ((nbytes = read(fd, buffer, BUFFSIZE)) > 0) {
    std::mbstate_t state = std::mbstate_t();
    char * ptr = buffer;
    const char* end = ptr + std::strlen(ptr);
    int len;
    wchar_t wc;
    while((len = std::mbrtowc(&wc, ptr, end-ptr, &state)) > 0) {
      std::wcout << "Next " << len << " bytes are the character " << wc << std::endl;
      ptr += len;
      chars += 1;
      if (ptr > end) break;
    } // while
  } // while
  std::wcout << "# chars = " << chars << std::endl;
  close(fd);*/
  
  
  for (int i = 1; i < argc; i++) {
	if (strcmp(argv[i],"-c") == 0) {
		cflag = true;
		flagcount++;
	}
	else if (strcmp(argv[i],"-m") == 0) {
		mflag = true;
		flagcount++;
	}
	else if (strcmp(argv[i],"-l") == 0) {
		lflag = true;
			flagcount++;
	}
	else if (strcmp(argv[i],"-w") == 0) {
		wflag = true;
		flagcount++;
	}
	else if (strcmp(argv[i],"-lw") == 0) {
		//cout << "HI" << endl;
		lflag = true;
		wflag = true;
		flagcount++;
	}
	else if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
		cout << "wc: invalid option " << argv[i] << endl;
			return 0;
	}
  }
  for (int i = 1; i < argc; i++) {
	  //cout << argc << endl;
	if (flagcount == 0 && strcmp(argv[1],"-") != 0) {
		int n = open(argv[i],O_RDONLY);
		if (n == -1) {
			perror("wc");
			continue;
		}
		char buf[100000];
		int y = 0;
		int bytessum = 0;
		while((y = read(n,buf,100000)) > 0){bytessum += y;}
		lflag = true;
		wflag = true;
		cflag = true;
		if (y == -1) {
			perror("wc");
		}
		if (n == -1) {
			perror("wc");
			continue;
		}
		filecount++;
		cout << lines(n,buf,bytessum) << " " << words(n,buf,bytessum) << " " << bytes(n,buf,bytessum) << " " << argv[i] << endl;
	}
	else if (flagcount == 0 && argc == 2 && strcmp(argv[1],"-") == 0) {
		lflag = true;
		wflag = true;
		cflag = true;
		char buf[100000];
		int bytessum = 0;
		//int y = 0;
		//cout << "HI" << endl;
			char copybuf[100000];
			//cout << "HI" << endl;
			//int o = open;
			
			int y = 0;
			int z = 0;
			while((y = read(STDIN_FILENO,copybuf,1)) > 0){
				//cout << "HI" << endl;
				buf[z] = copybuf[0];
				//cout << buf[i];
				bytessum += y;
				z++;
			}
			if (y == -1) {
				perror("wc");
			}
			cout << endl << lines(0,buf,bytessum) << " " << words(0,buf,bytessum) << " " << bytes(0,buf,bytessum) << endl;
	}
	else if(strcmp(argv[i],"-lw") != 0 && strcmp(argv[i],"-w") != 0 && strcmp(argv[i],"-l") != 0 && strcmp(argv[i],"-m") != 0 && strcmp(argv[i],"-c") != 0) {
		//cout << argv[i] << endl;
		int n;
		char buf[100000];
		int bytessum = 0;
		//cout << flagcount << endl;
		//cout << argc << endl;
		if ((strcmp(argv[i],"-")) == 0 || flagcount == (argc-1)) {
			n = STDIN_FILENO;
			char copybuf[100000];
			//cout << "HI" << endl;
			//int o = open;
			
			int y = 0;
			int z = 0;
			while((y = read(STDIN_FILENO,copybuf,1)) > 0){
				//cout << "HI" << endl;
				buf[z] = copybuf[0];
				//cout << buf[i];
				bytessum += y;
				z++;
			}
			if (y == -1) {
				perror("read");
			}
		}
		else {
			n = open(argv[i],O_RDONLY);
			if (n == -1) {
				perror("wc");
				continue;
			}
			int y = 0;
			while((y = read(n,buf,100000)) > 0){bytessum += y;}
			if (y == -1) {
				cout << argv[i] << endl;
				perror("read");
			}
		}
		if (n == -1) {
			perror("wc");
			continue;
		}
		if (lflag) {
			cout << lines(n,buf,bytessum) << " ";
		}
		if (wflag) {
			cout << words(n,buf,bytessum) << " ";
		}
		if (mflag) {
			cout << chars(n,buf,bytessum) << " ";
		}
		if (cflag) {
			cout << bytes(n,buf,bytessum) << " ";
		}
		cout << argv[i] << endl;
		filecount++;
	}
  }
  if (argc == 1) {
	 char buf[100000];
		int bytessum = 0;
		int y = 0;
		char copybuf[100000];
		int i = 0;
		
			while((y = read(STDIN_FILENO,buf,1)) > 0){
				//cout << "HI" << endl;
				copybuf[i] = buf[0];
				//cout << buf[i];
				bytessum += y;
				i++;
			}
				//cout << copybuf << endl;
			if (y == -1) {
				perror("wc");
			}
			//cout << endl << endl << buf;
			cout << endl << lines(0,copybuf,bytessum) << " " << words(0,copybuf,bytessum) << " " << bytes(0,copybuf,bytessum) << endl; 
			
			//delete[] copybuf;
			//cout << buf << endl;
			//cout << buf << endl;
  }
  if (filecount == 0 && flagcount == argc-1) {
	  	//n = STDIN_FILENO;
			char copybuf[100000];
			//cout << "HI" << endl;
			//int o = open;
			int bytessum = 0;
			char buf[100000];
			int n = STDIN_FILENO;
			int y = 0;
			int i = 0;
			while((y = read(STDIN_FILENO,copybuf,1)) > 0){
				//cout << "HI" << endl;
				buf[i] = copybuf[0];
				//cout << buf[i];
				bytessum += y;
				i++;
			}
			if (y == -1) {
				perror("read");
			}
			cout << "\t";
			if (lflag) {
			cout << lines(n,buf,bytessum) << " ";
			}
			if (wflag) {
				cout << words(n,buf,bytessum) << " ";
			}
			if (mflag) {
				cout << chars(n,buf,bytessum) << " ";
			}
			if (cflag) {
				cout << bytes(n,buf,bytessum) << " ";
			}
			cout << endl;
  }
  if (filecount > 1) {
		if (lflag) {
			cout << totalline << " ";
		}
		if (wflag) {
			cout << totalword << " ";
		}
		if (mflag) {
			cout << totalchar << " ";
		}
		if (cflag) {
			cout << totalbyte << " ";
		}
		cout << "total" << endl;
  }
} // main