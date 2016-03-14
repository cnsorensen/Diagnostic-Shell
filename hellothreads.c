#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count = 4;

void* hello( void* rank );

int main( int argc, char* argv[] )
{
    long thread;
    pthread_t thread_handles[4];

    for( thread = 0; thread < thread_count; thread++ )
            pthread_create( &thread_handles[thread], NULL, hello, (void*) thread );

    printf( "Hello from main!\n" );

    for( thread = 0; thread < thread_count; thread++ )
        pthread_join( thread_handles[thread], NULL );

    //free( thread_handles );

    return 0;
}

void* hello( void* rank )
{
    long my_rank = (long) rank;

    printf( "Hello from thread %ld of %d\n", my_rank, thread_count );

    return NULL;
}
