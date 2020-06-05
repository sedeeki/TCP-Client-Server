# TCP-Client-Server
TCP Client Server in C++ MultiThreaded. 

A C++ program that in a loop listens on a port for incoming TCP requests from clients. 
For each accepted incoming request it forks a child to read and process the request. 
The parent process continues to listen and accept incoming TCP requests in an endless loop.


The program accepts 2 command line parameters:

the port number to listen on,
the pathname to a directory that serves as root to all requested files or directories.
For example:

	% ./z123456 9001 www
The requests received by the program are of the form:

	GET pathname
where the pathname refers to a file or directory to be sent back to the client. The file/directory will be found in the directory specified on the command line. The following rules apply to the pathname:

it must start with a "/"
it may contain additional "/" path separators to access subdirectories
a single "/" character refers to the directory specified on the command line
a trailing "/" in the pathname can be ignored if the pathname refers to a directory
any data in the request beyond the pathname should be ignored
it may not contain the substring ".."
If the pathname refers to a file, then the content of the file is returned to the client.

If the pathname refers to a directory, then:

if a file "index.html" exists in that directory, it will be returned;
else, a list of files in that directory will be returned (not including any file that starts with ".")

  	GET INFO 
It will return the current time and date in text format to the client

