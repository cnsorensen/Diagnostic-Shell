// header.h

#ifndef _DSH_H_
#define _DSH_H_

/**Includes**/

#define _GNU_SOURCE
//#define _BSD_SOURCE
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**Function prototpyes**/

// misc functions
char* toLowerCase( char* );
int isInt( char* );
char* removeNewLine( char* );

// dsh commands
int cmdnm( char* );
void* p_cmdnm( void* );
int systat();
void* p_systat( void* );
int dsh_exit();
int cd( char* );
int pwd();
void* p_pwd( void* );
int dsh_kill( int, int );
int dsh_fork( char**, int );
int dsh_signal( int, int );
int dsh_hb( int, int, char* );
void* p_hb( void* );

// dsh functions
int dsh( char* );
char* splitInstructions( char* );
char* findRedirect( char* );
void clientSetup( char*, char* );
void serverSetup( char* );
char* splitRemoteClient( char* );
char* splitRemoteServer( char* );

/**globals**/

// for redirects
extern int redirectDirection;
extern char* redirectFilename;
extern const int DIRECT_IN;
extern const int DIRECT_OUT;
extern int redirectFlag;

// for pthreads
// [cmdnm, systat, hb, pwd]
extern pthread_t dsh_threads[4];
extern int thread_count;
extern int thread_flags[4];
extern const int CMDNM_T;
extern const int SYSTAT_T;
extern const int HB_T;
extern const int PWD_T;

// for pipelining
extern int pipeFlag;
extern char* args2[64];

// for remote pipelining
extern int remoteFlag; 
extern int remoteClientFlag;
extern int remoteServerFlag;
extern char* remotePort;
extern char* remoteAddress;

#endif
