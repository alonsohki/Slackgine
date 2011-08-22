#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <cerrno>
#include <cstring>

void sighandler ( int signum )
{
    switch (signum)
    {
        case SIGCHLD:
            exit ( EXIT_SUCCESS );
            break;
    }
}

int main ( int argc, char* const * argv, char* const* envp )
{
    pid_t child;

    if ( argc < 3 )
    {
        fprintf ( stderr, "Syntax: %s [time interval] [command]\n", argv[0] );
        fprintf ( stderr, "Time interval must be in miliseconds.\n" );
        return EXIT_FAILURE;
    }

    if ( ( child = fork() ) == 0 )
    {
        execve ( argv[2], &argv[2], envp );
        fprintf ( stderr, "Error when executing the process: %s\n", strerror(errno) );
        return EXIT_FAILURE;
    }

    signal ( SIGCHLD, sighandler );
    usleep ( atoi(argv[1]) * 1000 );
    kill ( child, SIGKILL );
    return EXIT_SUCCESS;
}

