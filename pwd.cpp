#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>

using std::cout;
using std::cerr;
using std::endl;

/**
This is the main method of the function that handles the arguements and options.
@param argc The number of arguments.
@param argv An array containing the arguements.
@return Returns 0 on success.
*/
int main() 
{
	cout.setf(std::ios::unitbuf);
	cerr.setf(std::ios::unitbuf);
	
	char pwd[1000];
	
	if(getcwd(pwd, 1000) != NULL)
		cout << pwd << endl;
	else
		cerr << strerror(errno) << endl;
	
	return EXIT_SUCCESS;
}