
#ifndef _DSH_H_
#define _DSH_H_

#define _GNU_SOURCE
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

// Function prototpyes
// Extra functions
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
int dsh( char* );
char* splitInstructions( char* );
char* findRedirect( char* );

// globals
extern int redirectDirection;
extern char* redirectFilename;

// [cmdnm, systat, hb, pwd]
extern pthread_t dsh_threads[4];
extern int thread_count;
extern int thread_flags[4];
extern const int CMDNM_T;
extern const int SYSTAT_T;
extern const int HB_T;
extern const int PWD_T;


extern int TEST;

extern int pipeFlag;
extern int redirectFlag;
extern int remoteFlag; 

extern char* args2[64];

extern const int DIRECT_IN;
extern const int DIRECT_OUT;

#endif
