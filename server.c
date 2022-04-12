// server.c // 
#include "snek.h"

char choose_last_character(char* ClientMessage, int len)
{
    char c;
    if (len > 0)
    {
        c = ClientMessage[len - 1];
    } else {
        c = ClientMessage[len];
    }

    switch(c)
    {
        case 'w':
            return 'w';
        case 'a':
            return 'a';
        case 's':
            return 's';
        case 'd':
            return 'd';
    }
}

int main(int argc, char const *argv[])
{
    printf("Server connecting...\n");
    int sock, ret, connection; 
    struct sockaddr_in6 address; 
    struct sockaddr_in6 new_address;
    char ClientMessage[BUFF] = {0};
    char ServerMessage[BUFF] = {0};

    //Socket
    sock = socket(DOMAIN, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("[-] Socket creation FAILED\n");
        exit(-1);
    }
    printf("[+] Socket Successfully created\n");

    address.sin6_family = DOMAIN; 
    address.sin6_port = htons(PORT); 
    address.sin6_addr = in6addr_any; 

    int opt = 1, s = sizeof(opt);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, s);
        
    //Bind it
    ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (ret == -1){
        printf("[-] Bind error\n");
        exit(-1);
    }
    printf("[+] Bind Complete to port\n");

    // Listen()
    if(listen(sock, 10) == -1)
    {
        printf("[-] Failed to Listen\n");
        exit(-1);
    }
    printf("[+] Listening...\n");


    int addrlen = sizeof(address);
    char user_direction = 'd';

    //Accept()
    while (1)
    {
        connection = accept(sock, (struct sockaddr *)&address, &addrlen);
        if (connection == -1)
        {
            printf("[-] Accept Fail\n");
            exit(1);
        }
        printf("[+] Connection Accepted\n");

        while(1)
        {
            fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

            if(recv(connection, ClientMessage, BUFF, 0) == -1)
            {
                printf("[-] recv error \n");

            } else {
                user_direction = ClientMessage[0];
                printf("FROM USER: %c\n", user_direction);
                send(connection, ClientMessage, strlen(ClientMessage), 0);
            }
            
        }
    }
    printf("Client Disconnected\n");


    

    return 0;
}
