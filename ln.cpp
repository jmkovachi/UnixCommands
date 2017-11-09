#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

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
	
	struct stat sb;
	bool s_option = false, options = true;
	string arg;
	
	for(int i = 1; i < argc; i++)
	{
		arg = argv[i];
		
		if((arg.substr(0, 1) == "-") & options)
		{
			if(arg.length() > 2)
				cerr << "ln: invalid option -- `" << arg.substr(1, 1) << "'" << endl;
			else if((arg.length() == 2) & (arg.substr(1, 1) == "s"))
			{
				s_option = true;
				options = false;
				continue;
			}
		}
		
		if(stat(argv[i], &sb) == 0)
		{
			if((S_ISDIR(sb.st_mode)) | (S_ISCHR(sb.st_mode)) | (S_ISREG(sb.st_mode)) | 
			(S_ISFIFO(sb.st_mode)) | (S_ISLNK(sb.st_mode)) | (S_ISSOCK(sb.st_mode)))
			{
				if(s_option & (argc != 4))
				{					
					cerr << "ln: creating symbolic link `./" << argv[i] << "': File exists" << endl;
					exit(EXIT_FAILURE);
				}
				else if(S_ISDIR(sb.st_mode) & (i != 2))
				{
					cerr << "ln: `" << arg << "': hard link not allowed for directory" << endl;
					exit(EXIT_FAILURE);
				}
				else if((argc != 3) & (s_option == false))
				{
					cerr << "ln: creating hard link `./" << argv[i] << "': File exists" << endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		else if((s_option == false) & (i != 2))	 
		{
				cerr << "ln: accessing `" << argv[i] << "': No such file or directory" << endl;
				exit(EXIT_FAILURE);
		}
		//else
		{
			if(s_option)
			{ 
				errno = 0;
				if(argc == 3)
				{ 
					if(symlink("33fjioe0", argv[2]) < 0)
					{
						if(errno == EEXIST)
						{
							cerr << "ln: creating symbolic link `./" << argv[2] << "': File exists" << endl;
							exit(EXIT_FAILURE);
						}
						else
						{
							cerr << "ERROR: symlink(3): " << strerror(errno) << endl;
							exit(EXIT_FAILURE);
						}
					}
				}
				else if(argc == 4)
				{
					if((stat(argv[3], &sb) == 0) & S_ISDIR(sb.st_mode))
					{
						if(chdir(argv[3]) < 0)
						{
							cerr << "ln: cannot change directory: " << strerror(errno) << endl;
							exit(EXIT_FAILURE);
						}
						
						if(symlink("24fjowe0", argv[2]) < 0)
						{
							if(errno == EEXIST)
							{
								cerr << "ln: creating symbolic link `./" << argv[3] << "': File exists" << endl;
								errno = 0;
								exit(EXIT_FAILURE);
							}
							else
							{
								cerr << "ERROR: symlink(3): " << strerror(errno) << endl;
								exit(EXIT_FAILURE);
							}
						}
						else
							exit(EXIT_SUCCESS);
					}
					else
					{
						if(symlink(argv[2], argv[3]) < 0)
						{
							if(errno == EEXIST)
							{
								cerr << "ln: creating symbolic link `./" << argv[3] << "': File exists" << endl;
								errno = 0;
								exit(EXIT_FAILURE);
							}
							else
							{
								cerr << "ERROR: symlink(3): " << strerror(errno) << endl;
								exit(EXIT_FAILURE);
							}
						}
						else
							exit(EXIT_SUCCESS);
					}
					
					
				}
			}
			else
			{	
				if((stat(argv[2], &sb) == 0) & (S_ISDIR(sb.st_mode)))
				{
					string arg1 = argv[1];
					string arg2 = argv[2];
					
					if(arg2.substr(arg2.length(), 1) != "/")
						arg2 += "/";
					
					string arg3 = arg2 + arg1;
										
					if(link(argv[1], arg3.c_str()) < 0)
					{
						cerr << "ERROR: link(2): " << strerror(errno) << endl;
						exit(EXIT_FAILURE);
					}
					else
						exit(EXIT_SUCCESS);
				}
				else if(link(argv[1], argv[2]) < 0)
				{
					cerr << "ERROR: link(2): " << strerror(errno) << endl; 
					exit(EXIT_FAILURE);
				}
				else
					exit(EXIT_SUCCESS);
			}
		}
	}
}