// dhs_fork.c

#include "header.h"

// Uses the fork command to signal the system with a command
//  Layout is similar to Dr. Jeff McGough's code which was taken from
//  "Advanced Linux Programming," by CodeSourcery LLC
//  Copyright (C) 2001 by New Riders Publishing
//
// Params: char* args[] - list of the commands from the user
// Return: 1 to continue for loop for the program

int dsh_fork(char* args[], int num_params)
{    
  // duplicate the process
  pid_t c_pid;
  c_pid = fork();

  // for pipelining
  int fds[2];

  // create a pipe
  if(pipeFlag == 1)
  {
    if(pipe(fds) == -1)
    {
      return -1;
    }
  }

  // for redirect
  int rd_fds[1];

  // create a pipe
  if(redirectFlag == 1)
  {
    if(pipe(rd_fds) == -1)
    {
      // FIXME: need to close the fds pipe
      return -1;
    }
  }

  // array to hold the arguments
  char* args1[num_params + 1];

  // add the null to the end in order for execvp to work
  // Note to me: Move this.
  int i;
  for(i = 0; i < num_params; i++)
  {
    args1[i] = args[i];
  }

  args1[i] = NULL;

  // child process
  // typecasting 0 to a pid_t
  if(c_pid == (pid_t)0)
  {
    // if there is pipelining
    if(pipeFlag == 1)
    {
      // create another fork
      int pipe_pid;
      pipe_pid = fork();

      // child process
      if(pipe_pid == 0)
      {
        dup2(fds[1], STDOUT_FILENO);                
        close(fds[0]);

        // execute the first command
        execvp(args1[0], args1);
      }
      // parent process
      else
      {
        dup2(fds[0], STDIN_FILENO);
        close(fds[1]);

        // execute the second command
        execvp(args2[0], args2);
        exit(0);
      }

      exit(0);
    }

    // if there is redirection
    if(redirectFlag == 1)
    {
      // if it's <
      if(redirectDirection == DIRECT_IN)
      {
        rd_fds[0] = open(redirectFilename, O_RDONLY, 0666);
        dup2(rd_fds[0], STDIN_FILENO);
      }
      // if it's >
      else
      {
        rd_fds[0] = creat(redirectFilename, 0644);
        dup2(rd_fds[0], STDOUT_FILENO);
      }

      close(rd_fds[0]);

      // execute the command
      execvp(args1[0], args1);

      exit(0);
    }
    else
    {
      // if it's a client, set it up
      if(remoteClientFlag == 1)
      {
        clientSetup(remoteAddress, remotePort);
      }
      // if it's a server, set it up
      else if(remoteServerFlag == 1)
      {
        serverSetup(remotePort);
      }

      // execute the program
      execvp(args1[0], args1);

      // only returns when an erro occurs
      exit(0);
    }
  
  }
  // parent process
  else
  {
    int waiting;
    wait(&waiting);

    // if there was piping, close 'em up!
    if(pipeFlag == 1)
    {
      close(fds[0]);
      close(fds[1]);
    }
  }

  return 1;
}
