//  snek.c //
#include "snek.h"

// Function to start the server from the snake file as it is in the same directory
void server() {
    system("./server.out");
    return;
}

// Function to start the client from the snake file as it is in the same directory
void client(){
    system("./client.out");
    return;
}

// The main function accepts commands from the command line to start the server or client
int main(int argc, char const *argv[])
{
    printf("%s, expects (1) arg, %d provided", argv[0], argc-1);
    if (argc == 2)
    {
        printf(": \"%s\".\n", argv[1]);
    } else {
        printf(".\n");
        return 1;
    }
    
    if (argv[1][1] == 's')
    {
        printf("Starting server...\n");
        server();
    }
    
    if (argv[1][1] == 'c')
    {
        printf("Starting client...\n");
        client();
    }
    
    if (argv[1][1] == 'h')
    {
        printf("HELP: Usage:  -s for server, -c for client\n");
    }
    
    return 0;
}
