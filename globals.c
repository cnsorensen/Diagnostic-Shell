#include "header.h"

// for the pthreads
pthread_t dsh_threads[4];
int thread_count = 0;
int thread_flags[4] = {0, 0, 0, 0};
const int CMDNM_T = 0;
const int SYSTAT_T = 1;
const int HB_T = 2;
const int PWD_T = 3;

// for remote pipelining
int remoteFlag = 0;
int remoteClientFlag = 0;
int remoteServerFlag = 0;
char* remotePort = "";
char* remoteAddress = "";

// for pipelining
int pipeFlag = 0;

// hold the second set of arguments when pipelining
char* args2[64];

// for redirection
int redirectDirection = -1;
int redirectFlag = 0;
char* redirectFilename = "";
const int DIRECT_IN = 0;
const int DIRECT_OUT = 1;
