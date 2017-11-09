#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;

void read_user_input();
bool exists(char *);

/**
This is the main method of the function that handles the arguements and options.
@param argc The number of arguments.
@param argv An array containing the arguements.
@return Returns 0 on success.
*/
int main(int argc, char * argv[])
{		
    cout.setf(std::ios::unitbuf);
	cerr.setf(std::ios::unitbuf);
	
	string arguement, num_lines;
	int fd, write_up_to_here, count = 0, lines;
	bool digit = true, n_arg = false;
	char buffer[10000];
	
	if(argc == 1)
		read_user_input();
	else if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			arguement = argv[i];
			
			if((i == 1) & (arguement.length() > 1) & (arguement.substr(0, 1) == "-"))
			{
				num_lines = arguement.substr(1, string::npos);
				
				for(unsigned int i = 0; i < num_lines.length(); i++)
				{
					if(!isdigit(num_lines[i]))
						digit = false;
				}
				
				if(digit)
				{
					lines = atoi(num_lines.c_str());
					n_arg = true;
				}
				else
				{
					cerr << "head: invalid option -- '" << num_lines.substr(0, 1) << "'" << endl;
					return EXIT_FAILURE;
				}
			}
			else if(arguement == "-")
			{
				if(((n_arg) & (argc > 3)) | (argc > 2))
					cout << "==> standard input <==" << endl;
				read_user_input();
			}
			else if(exists(argv[i]))
			{
				if(((n_arg) & (argc > 3)) | (argc > 2))
					cout << "==> " << arguement << " <==" << endl;
				
				if(n_arg == false)
					lines = 10;
				
				fd = open(argv[i], O_RDONLY);
				
				if(fd < 0)
				{
					cerr << "ERROR: CANNOT OPEN FILE" << endl;
					continue;
				}
				
				read(fd, &buffer, 10000);
				
				for(int j = 0; j < 10000; j++)
				{
					if((buffer[j] == '\n') | (buffer[j] == '\0'))
						count++;
					if(count == lines) // <-- change depending on arguement 
					{
						write_up_to_here = j;
						buffer[write_up_to_here] = '\n';
						break;
					}
				}
				write(1, &buffer, write_up_to_here + 1);
			}
			else
			{
				cerr << "head: cannot open `" << arguement << "' for reading: No such file or directory" << endl;
			}
			
			digit = true;
			count = 0;
			close(fd);
		}
	}
	
	
	
	return EXIT_SUCCESS;
}

/**
This method is used to check if a given file exists.
@param file_name The file to check existance.
@return True if the file exists, false otherwise.
*/
bool exists(char * file_name)
{
	ifstream file_to_check(file_name);
	
	if(!file_to_check.good())
	{
		file_to_check.close();
		return false;
	}
	else
	{
		file_to_check.close();
		return true;
	}
}

/**
This method is used to read user input from stdin.
*/
void read_user_input()
{
	char buffer[256];
	int bytes_read;
	
	while((bytes_read = read(0, &buffer, 256)))
		write(1, &buffer, bytes_read);	
}