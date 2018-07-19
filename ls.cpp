#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::sort;
using std::tolower;
using std::lexicographical_compare;
using std::tm;
using std::ifstream;

void list(bool, bool);
void list(bool, bool, string);
string get_pwd();
bool compare(char, char);
bool ignore_case(const string, const string);
string permissions(string);
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
	
	bool a_option = false, l_option = false, options = true, files_present = false;
	string arg;
	
	if(argc == 1)
	{
		list(a_option, l_option);
		exit(EXIT_SUCCESS);
	}
	else if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			arg = argv[i];
			
			if(options & (arg.substr(0, 1) == "-") & (arg.length() > 1))
			{
				if((arg == "-al") | (arg == "-la"))
				{
					a_option = true;
					l_option = true;
					options = false;
				}
				else if(arg == "-a")
				{
					a_option = true;
				}
				else if(arg == "-l")
				{
					l_option = true;
				}
				else
				{
					cerr << "ls: invalid option -- '" << arg.substr(1, 1) << "'" << endl;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				options = false;
				files_present = true;
				
				if(exists(argv[i]))
				{
					struct stat sb;
					string temp_dir = get_pwd();
					string file;
					int num_dir = 0, num_file = 0, nd, nf;
					
					if(stat(argv[i], &sb) == 0)
					{
						for(int j = i; j < argc; j++)
						{
							if(stat(argv[j], &sb) == 0)
							{
								if(S_ISDIR(sb.st_mode))
									num_dir++;
								else
									num_file++;
							}
						}
						
						if(num_dir == 0)
							num_dir = -1;
						if(num_file == 0) 
							num_file = -1;
						
						nd = num_dir;
						nf = num_file;
						
						for(int j = i; j < argc; j++) //print files first
						{
							file = argv[j];
							if(stat(argv[j], &sb) == 0)
							{
								if(!S_ISDIR(sb.st_mode))
								{
									list(a_option, l_option, file);
									num_file--;
								}
							}
							else
							{
								cerr << "ERROR6: stat(): " << strerror(errno) << endl;
								exit(EXIT_FAILURE);
							}	
							
							if((num_file == 0) & (num_dir != 0))
								cout << endl;
							
						}
						for(int j = i; j < argc; j++) //print directories last
						{
							file = argv[j];
							if(stat(argv[j], &sb) == 0)
							{
								if(S_ISDIR(sb.st_mode))
								{
									if((nd > 1) | ((nd >= 1) & (nf > 0)))
										cout << file << ": " << endl;
									
									chdir(argv[j]);
									list(a_option, l_option);
									chdir(temp_dir.c_str());
									num_dir--;
								}
							}
							else
							{
								cerr << "ERROR7: stat(): " << strerror(errno) << endl;
								exit(EXIT_FAILURE);
							}
							
							if(num_dir != 0)
								cout << endl;
						}
						/*if(S_ISDIR(sb.st_mode))
						{
							chdir(argv[i]);
							list(a_option, l_option);
							chdir(temp_dir.c_str());
						}
						else
							list(a_option, l_option, arg);*/
						exit(EXIT_SUCCESS);
					}
					else
					{
						cerr << "ERROR8: stat(): " << strerror(errno) << endl;
						exit(EXIT_FAILURE);
					}
					
				}
				else
				{
					cerr << "ls: cannot access " << argv[i] << ": No such file or directory" << endl; 
				}
			}
		}
		
		if(!files_present)
			list(a_option, l_option);
	}
}

/**
This method is used as an accessory method inorder to lexicographical_compare.
@param a The first char to compare.
@param b The second char to compare.
@return Returns true if a < b;
*/
bool compare(char a, char b)
{
	return(tolower(static_cast<unsigned char>(a)) < tolower(static_cast<unsigned char>(b)));
}

/**
This method is used to generate the comparison arguement for sorting the vector array.
@param a The first string to compare.
@param b The second string to compare.
@return Returns true if a (ignorecase) precedes b (ignorecase) alphabetically. 
*/
bool ignore_case(const string a, const string b)
{
	return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), compare);
}

/**
This method is used to actually list out a file and its properties.
@param a_option True if the a option is specified in the arguements.
@param l_option True if the l option is specified in the arguements.
@param file The file who's properties are going to be displayed.
*/
void list(bool a_option, bool l_option, string file)
{
	cout.setf(std::ios::unitbuf);
	cerr.setf(std::ios::unitbuf);
	
	if((!a_option & !l_option) | (a_option & !l_option)) // just $ ls
		cout << file << endl;
	else if((a_option & l_option) | (!a_option & l_option))
	{
		struct stat sb;
		struct passwd * u;
		uid_t uid;
		
		u = getpwuid(uid = getuid());
		

			if(stat(file.c_str(), &sb) < 0)
			{
				cerr << "ERROR1: stat(): " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			struct group * g = getgrgid(sb.st_gid);
			
			cout << permissions(file);// << sb.st_nlink << " " << entries[i] << "\t" << u->pw_name << endl;
			if(sb.st_nlink < 10)
				cout << " " << sb.st_nlink;
			else
				cout << sb.st_nlink;
			cout << " " << u->pw_name << " " << g->gr_name;
			string g_name = g->gr_name;
			if(g_name != "apache")
				cout << " ";
			if(sb.st_size < 10)
				cout << "     " << sb.st_size;
			else if(sb.st_size < 100)
				cout << "    " << sb.st_size;
			else if(sb.st_size < 1000)
				cout << "   " << sb.st_size;
			else if(sb.st_size < 10000)
				cout << "  " << sb.st_size;
			else if(sb.st_size < 100000)
				cout << " " << sb.st_size;
			else 
				cout << sb.st_size;
			
			struct tm * t = localtime(&sb.st_mtime);
			string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
			string pad = "", zero = "";
			cout << " " << month[t->tm_mon] << " ";
			if(t->tm_mday < 10)
				cout << " " << t->tm_mday;
			else 
				cout << t->tm_mday;
			if(t->tm_hour < 10)
				zero += "0";
			if(t->tm_min < 10)
				pad += " ";
			cout << " " << zero << t->tm_hour << ":" << t->tm_min << " " << pad << file << endl;
	}
}

/**
This method is used to actually list out a file and its properties.
@param a_option True if the a option is specified in the arguements.
@param l_option True if the l option is specified in the arguements.
*/
void list(bool a_option, bool l_option)
{
	cout.setf(std::ios::unitbuf);
	cerr.setf(std::ios::unitbuf);

	vector<string> entries;
	
	if(l_option)
	{
		DIR * directory = opendir(get_pwd().c_str());
		struct dirent * entry;
		struct stat sb;
		string name;
		int total = 0;
				
		while((entry = readdir(directory)) != NULL)
		{
			name = entry->d_name;
			
			if(stat(name.c_str(), &sb) == 0)
			{
				if(!a_option)
				{
					if(name.substr(0, 1) != ".") //exclude "." and ".."
						total += sb.st_blocks;
				}
				else
					total += sb.st_blocks;	
			}
			else
			{
				cerr << "ERROR2: stat(): " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
		}
		
		cout << "total " << total / 2 << endl; 
		closedir(directory);
	}
	
	if(!a_option & !l_option) // just $ ls
	{
		DIR * directory = opendir(get_pwd().c_str());
		struct dirent * entry;
		string name;
				
		while((entry = readdir(directory)) != NULL)
		{
			name = entry->d_name;
			
			if(name.substr(0, 1) != ".") //exclude "." and ".."
				entries.push_back(entry->d_name);
		}
		closedir(directory);
		
		sort(entries.begin(), entries.end(), ignore_case);
		
		for(unsigned int i = 0; i < entries.size(); i++)
			cout << entries[i] << endl;
		closedir(directory);
	}
	else if(a_option & !l_option)
	{
		DIR * directory = opendir(get_pwd().c_str());
		struct dirent * entry;
				
		while((entry = readdir(directory)) != NULL)
				entries.push_back(entry->d_name);
			
		closedir(directory);
		
		sort(entries.begin(), entries.end(), ignore_case);
		
		for(unsigned int i = 0; i < entries.size(); i++)
			cout << entries[i] << endl;
		closedir(directory);
	}
	else if(a_option & l_option)
	{
		DIR * directory = opendir(get_pwd().c_str());
		struct dirent * entry;
				
		while((entry = readdir(directory)) != NULL)
				entries.push_back(entry->d_name);
			
		closedir(directory);
		
		sort(entries.begin(), entries.end(), ignore_case);
		
		struct stat sb;
		struct passwd * u;
		uid_t uid;
		
		u = getpwuid(uid = getuid());
		
		for(unsigned int i = 0; i < entries.size(); i++)
		{
			if(stat(entries[i].c_str(), &sb) < 0)
			{
				cerr << "ERROR3: stat(): " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			struct group * g = getgrgid(sb.st_gid);
			
			cout << permissions(entries[i]);// << sb.st_nlink << " " << entries[i] << "\t" << u->pw_name << endl;
			if(sb.st_nlink < 10)
				cout << " " << sb.st_nlink;
			else
				cout << sb.st_nlink;
			cout << " " << u->pw_name << " " << g->gr_name;
			string g_name = g->gr_name;
			if(g_name != "apache")
				cout << " ";
			if(sb.st_size < 10)
				cout << "     " << sb.st_size;
			else if(sb.st_size < 100)
				cout << "    " << sb.st_size;
			else if(sb.st_size < 1000)
				cout << "   " << sb.st_size;
			else if(sb.st_size < 10000)
				cout << "  " << sb.st_size;
			else if(sb.st_size < 100000)
				cout << " " << sb.st_size;
			else 
				cout << sb.st_size;
			
			struct tm * t = localtime(&sb.st_mtime);
			string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
			string pad = "", zero = "";
			cout << " " << month[t->tm_mon] << " ";
			if(t->tm_mday < 10)
				cout << " " << t->tm_mday;
			else 
				cout << t->tm_mday;
			if(t->tm_hour < 10)
				zero += "0";
			if(t->tm_min < 10)
				pad += " ";
			cout << " " << zero << t->tm_hour << ":" << t->tm_min << " " << pad << entries[i] << endl;
		}
	}
	else if(!a_option & l_option)
	{
		DIR * directory = opendir(get_pwd().c_str());
		struct dirent * entry;
		string name;
				
		while((entry = readdir(directory)) != NULL)
		{
			name = entry->d_name;
			
			if(name.substr(0, 1) != ".") //exclude "." and ".."
				entries.push_back(entry->d_name);
		}
			
		closedir(directory);
		
		sort(entries.begin(), entries.end(), ignore_case);
		
		struct stat sb;
		struct passwd * u;
		uid_t uid;
		
		u = getpwuid(uid = getuid());
		
		for(unsigned int i = 0; i < entries.size(); i++)
		{
			if(stat(entries[i].c_str(), &sb) < 0)
			{
				cerr << "ERROR4: stat(): " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			struct group * g = getgrgid(sb.st_gid);
			
			cout << permissions(entries[i]);
			if(sb.st_nlink < 10)
				cout << " " << sb.st_nlink;
			else
				cout << sb.st_nlink;
			cout << " " << u->pw_name << " " << g->gr_name;
			string g_name = g->gr_name;
			if(g_name != "apache")
				cout << " ";
			if(sb.st_size < 10)
				cout << "     " << sb.st_size;
			else if(sb.st_size < 100)
				cout << "    " << sb.st_size;
			else if(sb.st_size < 1000)
				cout << "   " << sb.st_size;
			else if(sb.st_size < 10000)
				cout << "  " << sb.st_size;
			else if(sb.st_size < 100000)
				cout << " " << sb.st_size;
			else 
				cout << sb.st_size;
			
			struct tm * t = localtime(&sb.st_mtime);
			string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
			string pad = "", zero = "";
			cout << " " << month[t->tm_mon] << " ";
			if(t->tm_mday < 10)
				cout << " " << t->tm_mday;
			else 
				cout << t->tm_mday;
			if(t->tm_hour < 10)
				zero += "0";
			if(t->tm_min < 10)
				pad += " ";
			cout << " " << zero << t->tm_hour << ":" << t->tm_min << " " << pad << entries[i] << endl;
		}
	}
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
This method is used to get the present working directory.
@return A string representation of the pwd.
*/
string get_pwd()
{
	cerr.setf(std::ios::unitbuf);
	cout.setf(std::ios::unitbuf);

	char cwd[1000];
	string pwd;
	
	if(getcwd(cwd, 1000) == NULL)
		cerr << "ERROR: getcwd(): " << strerror(errno) << endl;

	pwd = cwd;
	
	return pwd;
}

/**
This method is used to generate a string representation of the file permissions.
@param file The file who's permissions you will display.
@return A string representation of the file permissions.
*/
string permissions(string file)
{
	cerr.setf(std::ios::unitbuf);
	
	struct stat sb;
	string permissions = "";
	int mode = 0;
	
	if(lstat(file.c_str(), &sb) < 0)
		cerr << "ERROR5: stat(): " << strerror(errno) << endl;
	else
	{
		if(sb.st_mode & S_ISVTX)
			mode += 1;
		if(sb.st_mode & S_ISGID)
			mode += 2;
		if(sb.st_mode & S_ISUID)
			mode += 4;
		
		switch(sb.st_mode & S_IFMT)
		{
			case S_IFBLK: 
				permissions += "b";
				break;
			case S_IFCHR:
				permissions += "c";
				break;
			case S_IFDIR:
				permissions += "d";
				break;
			case S_IFIFO:
				permissions += "p";
				break;
			case S_IFLNK:
				permissions += "l";
				break;
			case S_IFREG:
				permissions += "-";
				break;
			case S_IFSOCK:
				permissions += "s";
				break;
			default:
				permissions += "?";
				break;
		}
		switch(((sb.st_mode & S_IRWXU) >> 6))
				{
					case 0:	
						if(mode >= 4)
							permissions += "--S";
						else
							permissions += "---";		
						break;
					case 1:		
						if(mode >= 4)
							permissions += "--s";
						else
							permissions += "--x";		
						break;
					case 2:	
						if(mode >= 4)
							permissions += "-wS";
						else
							permissions += "-w-";		
						break;
					case 3:
						if(mode >= 4)
							permissions += "-ws";
						else
							permissions += "-wx";		
						break;
					case 4:
						if(mode >= 4)
							permissions += "r-S";
						else
							permissions += "r--";		
						break;
					case 5:	
						if(mode >= 4)
							permissions += "r-s";
						else
							permissions += "r-x";		
						break;
					case 6:	
						if(mode >= 4)
							permissions += "rwS";
						else
							permissions += "rw-";		
						break;
					case 7:
						if(mode >= 4)
							permissions += "rws";
						else
							permissions += "rwx";		
						break;
					default:	cerr << "ERROR: PERMISSIONS";	break;
				}
				switch(((sb.st_mode & S_IRWXG) >> 3))
				{
					case 0:	
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))       
							permissions += "--S";
						else
							permissions += "---";		
						break;
					case 1:		
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))        
							permissions += "--s";
						else
							permissions += "--x";		
						break;
					case 2:	
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))         
							permissions += "-wS";
						else
							permissions += "-w-";		
						break;
					case 3:
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))        
							permissions += "-ws";
						else
							permissions += "-wx";		
						break;
					case 4:
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))       
							permissions += "r-S";
						else
							permissions += "r--";		
						break;
					case 5:	
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))         
							permissions += "r-s";
						else
							permissions += "r-x";		
						break;
					case 6:	
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))       
							permissions += "rwS";
						else
							permissions += "rw-";		
						break;
					case 7:
						if((mode == 2) | (mode == 3) | (mode == 6) | (mode == 7))       
							permissions += "rws";
						else
							permissions += "rwx";		
						break;
					default:	cerr << "ERROR: PERMISSIONS";	break;
				}
				switch(((sb.st_mode & S_IRWXO) >> 0))
				{
					case 0:	
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7))       
							permissions += "--T";
						else
							permissions += "---";		
						break;
					case 1:		
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7)) 
							permissions += "--t";
						else
							permissions += "--x";		
						break;
					case 2:	
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7))   
							permissions += "-wT";
						else
							permissions += "-w-";		
						break;
					case 3:
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7)) 
							permissions += "-wt";
						else
							permissions += "-wx";		
						break;
					case 4:
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7)) 
							permissions += "r-T";
						else
							permissions += "r--";		
						break;
					case 5:	
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7))  
							permissions += "r-t";
						else
							permissions += "r-x";		
						break;
					case 6:	
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7)) 
							permissions += "rwT";
						else
							permissions += "rw-";		
						break;
					case 7:
						if((mode == 1) | (mode == 3) | (mode == 5) | (mode == 7)) 
							permissions += "rwt";
						else
							permissions += "rwx";		
						break;
					default:	cerr << "ERROR: PERMISSIONS";	break;
				}
				
		permissions += ". ";
	}
	return permissions;
}