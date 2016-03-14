#include "header.h"

pthread_t dsh_threads[4];
int thread_count = 0;
int thread_flags[4] = {0, 0, 0, 0};
const int CMDNM_T = 0;
const int SYSTAT_T = 1;
const int HB_T = 2;
const int PWD_T = 3;

int pipeFlag = 0;
int redirectFlag = 0;
int remoteFlag = 0;

char* args2[64];

int TEST = 4;

int redirectDirection = 0;
char* redirectFilename = "";

const int DIRECT_IN = 0;
const int DIRECT_OUT = 1;
