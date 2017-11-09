#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
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
	
	bool p_option = false, m_option = false, options = true, valid_mode = true; 
	string arg, opt, mode = "uninitialized_copy";
	int mask;
	char pwd[1000];
	mode_t file_permission;
	
	if(getcwd(pwd, 1000) != NULL) {}
	else
		cerr << "ERROR: getcwd(): " << strerror(errno) << endl;
	
	
	if(argc == 1)
		cerr << "mkdir: missing operand" << endl;
	else if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{			
			arg = argv[i];
			
			if((arg.substr(0, 1) == "-") & (arg.length() > 1) & options)
			{
				opt = arg.substr(1, string::npos);
								
				if((arg.substr(1, 1) != "p") & (arg.substr(1, 1) != "m"))
				{
					cerr << "mkdir: invalid option -- '" << opt.substr(0, 1) << "'" << endl;
					exit(EXIT_FAILURE);
				}
				else
				{
					if(opt == "m")
						m_option = true;
					else if((opt.substr(0, 1) == "m") & (opt.length() > 1))
					{
						m_option = true;
						mode = opt.substr(1, string::npos);
					}
					else if(opt == "p")
						p_option = true;
					else if(opt == "pm")
					{
						p_option = true;
						m_option = true;
					}
					else if((opt.substr(0, 2) == "pm") & (opt.length() > 2))
					{
						p_option = true;
						m_option = true;
						mode = opt.substr(2, string::npos);
					}
					
					//if(m_option)
						//options = false;
				}
			}
			else
			{
				//options = false;
				
				if(m_option & (mode == "uninitialized_copy"))
					mode = arg;
				else if(m_option == false)
					mode = "0755";
				
				for(unsigned int i = 0; i < mode.length(); i++)
				{
					if(!(mode.substr(i, 1) == "0") & !(mode.substr(i, 1) == "1") & !(mode.substr(i, 1) == "2") & !(mode.substr(i, 1) == "3") &
						!(mode.substr(i, 1) == "4") & !(mode.substr(i, 1) == "5") & !(mode.substr(i, 1) == "6") & !(mode.substr(i, 1) == "7"))
							valid_mode = false;
				}
				
				if((mode.length() != 4)) 
					valid_mode = false;
				
				if(mode.substr(0, 1) != "0")
					valid_mode = false;
				
				if(valid_mode == false)
				{
					cerr << "mkdir: invalid mode `" << mode << "'" << endl;
					exit(EXIT_FAILURE);
				}
				
				if(mode == arg)
					continue;
				else
				{
					mask = 0;
					mask += (atoi(mode.substr(0, 1).c_str()) * 512);
					mask += (atoi(mode.substr(1, 1).c_str()) * 64);
					mask += (atoi(mode.substr(2, 1).c_str()) * 8);
					mask += (atoi(mode.substr(3, 1).c_str()) * 1);
					
					file_permission = mask;
					
					if(p_option)
					{						
						string dir = arg, path = arg, temp = arg, dilimiter = "/";
						int num_dir = 0;
						
						while(path.find(dilimiter) != string::npos)
						{
							path = path.substr(path.find(dilimiter) + 1, string::npos);
							num_dir++;
						}
						
						if(arg.substr(arg.length() - 1, 1) == "/")
							num_dir--;
						
						if(arg.substr(0, 1) == "/")	// if path is absolute
						{
							if(chdir("/") < 0)
								cerr << "ERROR: chdir(): " << strerror(errno) << endl;
							else
							{
								for(int i = 0; i < num_dir; i++) //	loop through each dir between /***/
								{
									struct stat st;
									
									if(i != (num_dir - 1))
									{
										dir = temp.substr(temp.find(dilimiter) + 1, temp.find(dilimiter, 1));
										temp = temp.substr(temp.find(dilimiter, 1), string::npos);
										
										if((stat(dir.c_str(), &st) == 0) & (S_ISDIR(st.st_mode))) {}
										else
										{
											if(mkdir(dir.c_str(), 0755) < 0)
												cerr << "mkdir: cannot create directory `" << dir << "': " << strerror(errno) << endl;
											else
												if(chmod(dir.c_str(), 0755) < 0)
													cerr << "ERROR: chmod(): " << strerror(errno) << endl;
										}
										
										if(chdir(dir.c_str()) < 0)
											cerr << "ERROR: chdir(): " << strerror(errno) << endl;											
									}
									else
									{
										temp = temp.substr(1, string::npos);
										
										if((stat(temp.c_str(), &st) == 0) & (S_ISDIR(st.st_mode))) {}
										else
										{
											if(mkdir(temp.c_str(), file_permission) < 0)
												cerr << "mkdir: cannot create directory `" << temp << "': " << strerror(errno) << endl;
											else
												if(chmod(temp.c_str(), file_permission) < 0)
													cerr << "ERROR: chmod(): " << strerror(errno) << endl;
										}
									}
								}
							}
						}
						else	// if path is relative 
						{
							//check what happens if p_option is true and path has only one "/", may interfere with num_dir;
							
							dir = "/" + arg, path = "/" + arg, temp = "/" + arg, dilimiter = "/";
							num_dir = 0;
							
							while(path.find(dilimiter) != string::npos)
							{
								path = path.substr(path.find(dilimiter) + 1, string::npos);
								num_dir++;
							}
						
							if(arg.substr(arg.length() - 1, 1) == "/")
								num_dir--;
							
							for(int i = 0; i < num_dir; i++)
							{								
								if(i != (num_dir - 1))
								{
									dir = temp.substr(temp.find(dilimiter) + 1, temp.find(dilimiter, 1));
									temp = temp.substr(temp.find(dilimiter, 1), string::npos);
									
									if(mkdir(dir.c_str(), 0755) < 0)
										cerr << "mkdir: cannot create directory `" << dir << "': " << strerror(errno) << endl;
									else
										if(chmod(dir.c_str(), 0755) < 0)
											cerr << "ERROR: chmod(): " << strerror(errno) << endl;
									
									if(chdir(dir.c_str()) < 0)
										cerr << "ERROR: chdir(): " << strerror(errno) << endl;
								}
								else
								{
									temp = temp.substr(1, string::npos);
									
									if(mkdir(temp.c_str(), file_permission) < 0)
										cerr << "mkdir: cannot create directory `" << temp << "': " << strerror(errno) << endl;
									else
										if(chmod(temp.c_str(), file_permission) < 0)
											cerr << "ERROR: chmod(): " << strerror(errno) << endl;
								}
							}
							
						}
						
						if(chdir(pwd) < 0)
							cerr << "ERROR: chdir(): " << strerror(errno) << endl;
					}
					else
					{
						if(mkdir(arg.c_str(), file_permission) < 0)
							cerr << "mkdir: cannot create directory `" << arg << "': " << strerror(errno) << endl;
						else
							if(chmod(arg.c_str(), file_permission) < 0)
								cerr << "ERROR: chmod(): " << strerror(errno) << endl;
					}
				}
			}
		}
	}
	
}