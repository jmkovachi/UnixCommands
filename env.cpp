#include <iostream>

using std::cout;
using std::endl;

/**
This is the main method of the function that handles the arguements and options.
@param argc The number of arguments.
@param argv An array containing the arguements.
@param envp This array contains the elements that descrive the environment of the system.
@Return Returns 0 on success.
*/
int main(const int argc, const char * argv[], const char * envp[])
{
	cout.setf(std::ios::unitbuf);

	for(int i = 0; envp[i] != nullptr; ++i)
		cout << envp[i] << endl;
	
	return EXIT_SUCCESS;
}