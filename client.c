// client.c
#include "snek.h"
#include <ncurses.h>


int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in6 address; 
    char ServerMessage[BUFF] = {0};

    // Create Socket
    sock = socket(DOMAIN, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("[-] Could not create socket\n");
    }
    printf("[+] Socket created successfully\n");

    address.sin6_family = DOMAIN; 
    address.sin6_port = htons( PORT ); 
    inet_pton(DOMAIN, "::1", &address.sin6_addr); 


    // Connect to Remote Server
    int connection_status;
    connection_status = connect(sock, (struct sockaddr *)&address, sizeof(address));
    
    if (connection_status < 0)
    {
        printf("[-] Connection to server failed\n");
        return 1;
    }
    printf("[+] Client Connected.\n");


//Starting the game
    
    printf("Press k, then ENTER to start\n");
    char c;
    scanf("%c", &c);
    if (c == 'k')
    {

        char buf[20];
        buf[0] = 'd';
        fflush(stdout);
        while(1)
        {
            char ClientMessage[BUFF] = {0};
            
            fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
            sleep(1);
            int numRead = read(0, buf, 15);
            if (numRead > 0) {
                printf("You said: %s\n", buf);
                fflush(stdout);
            }
            printf("\n\n");
            fflush(stdout);


            //Sending the Message typed (or not typed) by the user
            send(sock, buf, strlen(buf), 0);

            //Recieving any replies from the server
            if (recv(sock, ServerMessage, BUFF, 0) == -1)
            {
                printf("[-] recv error \n");
                exit(1);
            }
            printf("Server Message: \t %s \n", ServerMessage);
        }
        
    }
    return 0;
}