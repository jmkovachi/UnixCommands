#Project 3

Joshua Anickat (811-148-237)
John Michael Kovachi (811-999-831)

## Instructions

To clean my code (i.e., remove any compiled files), type:

```
$ make clean
```

To compile my code, type:

```
$ make
```

To run my code (after compiling it), type:

	For cal
	```
	$ ./cal [[month] year]
	```
	
	For date
	```
	$ ./date [+format]
	```
	
	For head
	```
	$ ./head [-n number] [file...]
	```
	
	For ln
	```
	$ ./ln [-s] target-file link-file
	```
	
	For ls
	```
	$ ./ls [-al] [file...]
	```
	
	For mkdir
	```
	$ ./mkdir [-p] [-m mode] dir...
	```
	
	For env
	```
	$ ./env 
	```
	
	For tail
	```
	$ ./tail [-f] [-c number | -n number] [file]
	```
	
	For true
	```
	$ ./true
	```
	
	For false
	```
	$ ./false 
	```

	For wc
	```
	$ ./wc [-c | -m] [-lw] [file...]
	```
	
## Reflection
This project was a big relief for us. We were very happy to get away from 
anything that had to do with the ncurses library. This project focused 
completely around remaking our own versions of some unix system commands.
After completing this project we have learned a lot about reading and writing
files using low level file I/O. Also a great deal of option handling had to be
done. This was probably the hardest aspect of the project because the user can
enter various combinations of the options and our programs had to be able to 
detect them. We started working on this project pretty early so that we
wouldn't be cramming on the very last day. Because of this it has been a lot 
less stressful. Also it was significantly easier to divide the work between
us because each person can just take on a unix utility. We split them evenly
based on their difficulties. I feel like this project has been the most
helpful in terms of developing our coding skills and general knowledge of
both unix and c++ unlike the ncurses project.


##Works Cited

	"Cksum." Cksum. The IEEE and The Open Group, n.d. Web. 07 Nov. 2016.
	<http://pubs.opengroup.org/onlinepubs/9699919799/utilities/cksum.html>.

	Cotterell, Michael. "Cs1730/syscalls-2016-fall." GitHub. GitHub, n.d. Web. 07 Nov. 2016.
	<https://github.com/cs1730/syscalls-2016-fall/blob/master/cpp/mbrtowc.cpp>.