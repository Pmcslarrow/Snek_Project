// client.c
#include "snek.h"
#include "plist.c"

int get_rand(int MAX) {
    srand(time(NULL));
    return rand() % MAX;
}

/*
https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
Professor. Calvin said that if I wanted to use a faster timer than 1 second to look it up,
rather than just doing a 1 second timer this implements a timer in miliseconds rather than seconds.
I provide the link to give credit where credit is due. 
*/
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
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
        plist snake_list = pl_new();
        int GAME_SCORE = 0;
        int WIDTH, HEIGHT;
        getmaxyx(stdscr, HEIGHT, WIDTH);
        int SNEK_X = (WIDTH / 2);
        int SNEK_Y = (HEIGHT / 2);
        int APPLE_X, APPLE_Y;
        APPLE_X = get_rand(WIDTH - 1);
        APPLE_Y = get_rand(HEIGHT - 1);
        WINDOW * win = newwin(HEIGHT - 1, WIDTH - 1, WINDOW_START_Y, WINDOW_START_X);
        char dir = 'd';


        while(1)
        {
            wclear(win);

            /* Borders */
            box(win, 0, 0);

            /* Placing Snake on Screen */
            mvwprintw(win, SNEK_Y, SNEK_X, SNAK);
            mvwprintw(win, APPLE_Y, APPLE_X, APPLE);
            wrefresh(win);

            /* User Input */
            fflush(stdout);
            fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
            msleep(500);

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

            /* Movement Directioning */
            if (ClientMessage[0] == 'd' || ClientMessage[0] == 'D') { SNEK_X += 1; }
            if (ClientMessage[0] == 'w' || ClientMessage[0] == 'W') { SNEK_Y -= 1; }
            if (ClientMessage[0] == 'a' || ClientMessage[0] == 'A') { SNEK_X -= 1; }
            if (ClientMessage[0] == 's' || ClientMessage[0] == 'S') { SNEK_Y += 1; }

            /* Collision Detection */
            if (SNEK_X == APPLE_X && SNEK_Y == APPLE_Y)
            {
                /*The snek and the apple intersect and we need to update the score, snake length, and apple location*/
                GAME_SCORE += 1;
                APPLE_X = get_rand(WIDTH - 1);
                APPLE_Y = get_rand(HEIGHT - 1);
                dir = ClientMessage[0];
            }

            /* Window Collision */
            if (SNEK_X < 2) { SNEK_X = WIDTH - 2; dir = 'a';  }
            if (SNEK_X > WIDTH - 3) { SNEK_X = 2; dir = 'd';  }
            if (SNEK_Y < 1) { SNEK_Y = HEIGHT - 3; dir = 'w'; }
            if (SNEK_Y > HEIGHT - 3) { SNEK_Y = 1; dir = 's'; }
        }
        endwin();
        
    }
    return 0;
}
