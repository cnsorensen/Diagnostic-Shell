// dhs_fork.c

#include "header.h"

// Uses the fork command to signal the system with a command
//  Layout is similar to Dr. Jeff McGough's code which was taken from
//  "Advanced Linux Programming," by CodeSourcery LLC
//  Copyright (C) 2001 by New Riders Publishing
//
// Params: char* args[] - list of the commands from the user
// Return: 1 to continue for loop for the program
int dsh_fork( char* args[], int num_params )
{    
    // duplicate the process
    pid_t c_pid;
    c_pid = fork();

    int fds[2];

    // create a pipe
    if( redirectFlag == 0 )
    {
        pipe( fds );
    }
    else
    {
        pipe( redirect_fd );
    }

    // array to hold the arguments
    char* args1[num_params + 1];

    // add the null to the end in order for execvp to work
    int i;
    for( i = 0; i < num_params; i++ )
    {
        args1[i] = args[i];
    }
    args1[i] = NULL;

    // child processe
    if( c_pid == (pid_t) 0 )
    {
        // add another fork here if piping
        if(  pipeFlag == 1 )
        {
            //another fork
            int pipe_pid;
            pipe_pid = fork();

            // child process
            if( pipe_pid == 0 )
            {
                dup2( fds[1], STDOUT_FILENO );                
				close( fds[0] );
                execvp( args1[0], args1 );
            }
            // parent process
            else
            {
				dup2( fds[0], STDIN_FILENO );
                close( fds[1] );
                execvp( args2[0], args2 );
                exit(0);
            }

//            pipeFlag = 0;

			exit(0);
        }
        else if( redirectFlag == 1 )
        {
            dup2( redirect_fd, STDOUT_FILENO );
            execvp( args1[0], args1 );
            close( redirect_fd );
        }
		else
		{
        	// execute the program
        	execvp( args1[0], args1 );
        	// only returns when an erro occurs
        	exit(0);
		}
    
    }
    // parent process
    else
    {
        int waiting;
        wait( &waiting );
        if( redirectFlag == 0 )
        {
            close( fds[0] );
            close( fds[1] );
        }
        else if( redirectFlag == 1 );
        {
            close( redirect_fd );           
        }
    }

    return 1;
}
