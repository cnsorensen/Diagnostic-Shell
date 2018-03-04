// dsh.c
// 
// Program v2: Diagnostic Shell
// CSC 456 - Operating Systems
// South Dakota School of Mines and Technology
// Dr. Jeff S. McGough
//
// Author: Christine N. Sorensen
// Version 1: 2/12/16
// Version 2: 3/14/16
//
// Diognostic shell. A process identification program and a simple shell.
// Purpose: To intoduce unix environment, system calls, signal, the proc
//          system, harvard commas, and for/exec system calls.
//
// Bugs:
//      pthread it up - NOT DONE - well it is, cmdnm is being picky and it's all
//                      being trippy
//      hb - sometimes when you run it, it will spit out everything at once. Just run
//           it again and it will work properly
//      cmdnm - I have to have it print out the PID or else it won't work. It doesn't look
//              *terrible* but it keeps me awake at night.
//
//      | - doesn't a three-way with it though
//      < and > - it only works for the standard use. I made a cute program that reads
//                in a file like < and prints the number in it. But it doesn't print :(
//      ((and)) - it doesn't work for everything
//  

#include "header.h"

// Split the instructions (used in pipelining)
char* splitInstructions(char* line)
{
    // set the pipe flag
    pipeFlag = 1;

    // the delimeter to split the two commands
    const char delimPipe[2] = "|";

    // to hold each instruction
	char* instruction1;
	char* instruction2;

    // extract the first instruction
    instruction1 = strtok(line, delimPipe);
	
	// and the second instruction
	instruction2 = strtok(NULL, delimPipe);

	// delimiters
    const char delim[3] = " \t";
    // string to hold each token 
    char* token = NULL;

    int i = 0;
    // extract the first word
    token = strtok(instruction2, delim);
    // collect the words
    while(token != NULL)
    {      
		// add to list of tokens
        args2[i] = token;
        i++;
        // get the next word
        token = strtok(NULL, delim);
    }
	args2[i+1] = NULL;

    return instruction1;      
}

char* findRedirect(char* line)
{
    // delimiters for in or out
    const char delimIn[2] = "<";
	const char delimOut[2] = ">";
    const char delim[3] = "\n ";

	// string to hold each token 
    char* instruction = NULL;

    // set the flag for redirect
    redirectFlag = 1;

    // if it's a file in
	if(strchr( line, '<') != NULL )	
	{
        // grab the instruction
		instruction = strtok(line, delimIn);

        // get the filename
        redirectFilename = strtok(NULL, delim);
    	//redirectFilename = strtok(NULL, delimIn);

		// set the file direction
		redirectDirection = DIRECT_IN;

        // return the instruction to be executed
		return instruction;
	}
	
	// if it's a file out
	else if(strchr( line, '>') != NULL )	
	{
        // grab the instruction
        instruction = strtok(line, delimOut);
		
        // grab the filename
        redirectFilename = strtok(NULL, delim);
		//redirectFilename = strtok(NULL, delimOut);

        // set the filedirection
        redirectDirection = DIRECT_OUT;

		// return the instruction to be executed
		return instruction;	
	}

    // if there's no redirection, continue as normal
    return line;
}

// cmd ((server port
char* splitRemoteClient(char* line)
{
	char* instruction;

	// delimiters
    const char delim[3] = " (";

    // string to hold each token 
    char* token = NULL;

    int i = 0;
	char* tokens[64];

    // extract the first word
    token = strtok(line, delim);

    // collect the words
    while(token != NULL)
    {
        // add to list of tokens
        tokens[i] = token;
        i++;

        // get the next word
        token = strtok(NULL, delim);
    }
	
	instruction = tokens[0];
	remoteAddress = tokens[1];
	remotePort = tokens[2];

    return instruction;
} 

// cmd)) port
char* splitRemoteServer(char* line)
{
	char* instruction;

	// delimiters
    const char delim[3] = ")";

    // string to hold each token 
    char* token = NULL;

    int i = 0;
	char* tokens[64];

    // extract the first word
    token = strtok(line, delim);

    // collect the words
    while(token != NULL)
    {
        // add to list of tokens
        tokens[i] = token;
        i++;

        // get the next word
        token = strtok(NULL, delim);
    }
	
	instruction = tokens[0];
	remotePort = tokens[1];

    return instruction;
} 

// Handles the user input
// Param: char* line - the command the user submitted
// Return: 1 - continue while loop. 0 - end program
int dsh(char* line)
{
	// check for a pipe
	if(strchr( line, '|') != NULL )
	{
		line = splitInstructions(line);
	}
    
    // check for a redirect
    else if(strchr( line, '<') != NULL || strchr( line, '>' ) != NULL )
    {
        line = findRedirect(line);    
	}

    // check for remote client
    else if(strchr( line, '(') != NULL )
    {
        // turn on the flag
        remoteClientFlag = 1;
		line = splitRemoteClient(line);
    }

    // check for remote server
    else if(strchr( line, ')') != NULL )
    {
        // turn on the flag
        remoteServerFlag = 1;
		line = splitRemoteServer(line);
    }

    // delimiters
    const char delim[3] = " \t";

    // string to hold each token 
    char* token = NULL;
 
    // array to hold the tokens
    char* tokens[64];
    int i = 0;
    
    // extract the first word
    token = strtok(line, delim);

    // collect the words
    while(token != NULL)
    {
        // add to list of tokens
        tokens[i] = token;
        i++;

        // get the next word
        token = strtok(NULL, delim);
    }

    // call the appropriate function
    // check for the correct number of parameters
    // if there are more parameters than needed, it will ignore the excess
    // lowercase the command
    char* command = toLowerCase(tokens[0]);
    
    // default to return successful
    int return_val = 1;

    // the number of parameters to check for
    int num_params = i;
    if(strcmp( command, "cmdnm") == 0 )
    {
        if(num_params >= 2)
        {
            // check to see if pid is an int
            int flag = isInt(tokens[1]);
            if(flag != -1)
            {
                // pass in the pid
                return_val = cmdnm(tokens[1]);
            }
            else
            {
                return_val = -1;
            }
        }
        else
        {
            return_val = -1;
        }
    }
    else if(strcmp( command, "signal") == 0 )
    {
        // pass in the signal number and pic
        if(num_params >= 3)
        {
            int i1 = isInt(tokens[1]);
            int i2 = isInt(tokens[2]);
            if(i1 != -1 && i2 != -1)
            {
                return_val = dsh_signal(atoi(tokens[1]), atoi(tokens[2]));
            }
        }
        else
        {
            return_val = -1;
        }
    }
    else if(strcmp( command, "systat") == 0 )
    {
        return_val = systat();
    }
    else if(strcmp( command, "exit") == 0 )
    {
        return_val = dsh_exit();
    }
    else if(strcmp( command, "hb") == 0 )
    {
        if(num_params >= 4)
        {
            int i1 = isInt(tokens[1]);
            int i2 = isInt(tokens[2]);
            if(i1 != -1 && i2 != -1)
            {
                return_val = dsh_hb(atoi(tokens[1]), atoi(tokens[2]), tokens[3]);    
            }           
        }
        else
        {
            return_val = -1;
        }
    }
    else if(strcmp( command, "cd") == 0 )
    {
        // pass in path
        if(num_params >= 2)
        {
            return_val = cd(tokens[1]);
        }
        else
        {
            return_val = -1;
        }
    }
    else if(strcmp( command, "pwd") == 0 )
    {
        return_val = pwd();
    }
    else if(strcmp( command, "kill") == 0 )
    {
        if(num_params >= 3)
        {
            int i1 = isInt(tokens[1]);
            int i2 = isInt(tokens[2]);
            if(i1 != -1 && i2 != -1)
            {
                return_val = dsh_kill(atoi(tokens[1]), atoi(tokens[2]));
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        // it it doesn't match one of the custom commands above,
        // fork it
        return_val = dsh_fork(tokens, num_params);
    }

    // if one of the custom commands failed, fork it
    if(return_val == -1)
    {
        return_val = dsh_fork(tokens, num_params);
    }
    
    // return val determines whether to continue the dsh loop
    return return_val;
}

void collect_threads()
{ 
    int i;
    
    for(i = 0; i < 4; i++)
    {
        if(thread_flags[i] == 1)
        {
            pthread_join(dsh_threads[i], NULL);
            thread_count--;
        }
    }

    return;    
}

// Sets up a client
// Found at http://www.linuxhowtos.org/C_C++/socket.htm
void clientSetup(char* targeaddr, char* port)
{
	int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent* server;

	portno = atoi(port);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0) 
	{
        fprintf(stderr, "ERROR opening socket\n");
	    exit(0);
    }
	
	server = gethostbyname(targeaddr);

	if(server == NULL) 
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char*)&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;

	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect( sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 ) 
	{
		fprintf(stderr, "ERROR connecting\n");
        exit(0);
	}

	dup2(sockfd , STDIN_FILENO); 

	return;
}

// Sets up the server. 
// Found on http://www.linuxhowtos.org/C_C++/socket.htm
void serverSetup(char* port)
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{	
		fprintf(stderr, "ERROR opening socket\n");
        exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(port);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{ 
		fprintf(stderr, "ERROR on binding\n");
	    exit(0);
    }

	listen(sockfd, 5);

	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
    { 
		fprintf(stderr, "ERROR on accept\n");
        exit(0);
    }

	dup2(newsockfd , STDOUT_FILENO);
	dup2(newsockfd , STDIN_FILENO); 

	return;
}


int main(int argc, char** argv)
{
    // flag to control the loop
    int flag = 1;
    
    // each line read in from the user
    char line[80];

    while(flag == 1)
    {
        // print the shell name
        printf("dsh> ");

        // get the line from the user
        fgets(line, sizeof(line), stdin);

        // remove that annoying newline character at the end
        // that fgets seems to have necessary to include
        char* line2 = removeNewLine(line);

        // call the dsh function on the command line
        flag = dsh(line2);

        // collect the threads created
        if(thread_count > 0)
        {
            collect_threads();
        }
        
        // reset the flags
        pipeFlag = 0;
        redirectFlag = 0;
        remoteClientFlag = 0;
        remoteServerFlag = 0;
    }

    return 1;
}
