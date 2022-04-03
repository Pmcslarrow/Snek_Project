// client.c
#include "snek.h"
#include <ncurses.h>

/* Takes user input and adjust snake direction */
void move_snake(int *x, int *y, char ServerMessage)
{
    //ONLY WORKS FOR LEFT AND DOWN??
    switch(ServerMessage)
    {
        case RIGHT:
            *x += 1;
        
        case LEFT:
            *x -= 1;
        
        case UP:
            *y -= 1;

        case DOWN:
            *y += 1;
    }
}


int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in6 address; 
    char ServerMessage[BUFF] = {0};
    char ClientMessage[BUFF] = {0};

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



/*Starting the game*/

    printf("Welcome to SNEK!\nWhen you start, the snake will default in the right direction!\n");
    printf("Press k, then ENTER to start\n");
    char c;
    scanf("%c", &c);
    if (c == 'k')
    {
        
        initscr();
        noecho();

        /* Defaulting Screen and Snake */
        int WIDTH, HEIGHT;
        getmaxyx(stdscr, HEIGHT, WIDTH);
        int SNEK_X = WIDTH / 2;
        int SNEK_Y = HEIGHT / 2;
        WINDOW * win = newwin(HEIGHT - 1, WIDTH - 1, WINDOW_START_Y, WINDOW_START_X);
        char dir = 'd';

        while(1)
        {
            clear();

            /* Borders */
            box(win, 0, 0);
            wrefresh(win);

            /* Placing Snake on Screen */
            mvwprintw(win, SNEK_Y, SNEK_X, SNAK);
            wrefresh(win);

            /* User Input */
            fflush(stdout);
            fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
            sleep(1);

            char c = getch();
            if (c != -1)
            {
                dir = (char)c;
            }
            ClientMessage[0] = dir;
        

            //Send character to the server
            send(sock, ClientMessage, strlen(ClientMessage), 0);

            //Recieve Response from server
            if (recv(sock, ServerMessage, BUFF, 0) == -1)
            {
                printf("[-] recv error \n");
                exit(1);
            }

            /* Movement */
            move_snake(&SNEK_X, &SNEK_Y, ServerMessage[0]);
        

            
        
            
        }
        endwin();
        
    }
    return 0;
}