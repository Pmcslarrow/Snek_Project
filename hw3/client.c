// client.c
#include "snek.h"

int get_rand(int MAX) {
    srand(time(NULL));
    return 1 + (rand() % MAX);
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

int isIn(int x, int y, int total)
{
    int i = 1;
    while (i++ < total)
    {
        if (array[i].x == x && array[i].y == y)
        {
            return 1;
        }
    }
    return 0;

}


void game (int sock)
{
	initscr();
        noecho();

        /* Defaulting Screen and Snake */
        int GAME_SCORE = 0, WIDTH, HEIGHT, APPLE_X, APPLE_Y;
        getmaxyx(stdscr, HEIGHT, WIDTH);
        int SNEK_X = (WIDTH / 2);
        int SNEK_Y = (HEIGHT / 2);
        int total = 2;
        int start_speed = 125;
        char ServerMessage[BUFF] = {0};
        char ClientMessage[BUFF] = {0};
        char dir = 'd';
        WINDOW * win = newwin(HEIGHT - 1, WIDTH - 1, WINDOW_START_Y, WINDOW_START_X);
        APPLE_X = get_rand(WIDTH - 15);
        APPLE_Y = get_rand(HEIGHT - 15);
        array[0].x = SNEK_X;
        array[0].y = SNEK_Y;
        array[1].x = SNEK_X - 1;
        array[1].y = SNEK_Y;
        

        while(1)
        {
            wclear(win);

            /* Borders */
            box(win, 0, 0);

            /* Prints the snake & the apple on the screen */
            for (int i=0; i < total; i++)
            {
                mvwprintw(win, array[i].y, array[i].x, SNEK);
                wrefresh(win);
            }
            mvwprintw(win, APPLE_Y, APPLE_X, APPLE);
            wrefresh(win);


            /* User Input */
            fflush(stdout);
            fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
            msleep(start_speed);

            char c = getch();
            if (c != -1)
            {
                dir = (char)c;
            }
            ClientMessage[0] = dir;

            /* Movement Directioning */
            if (ClientMessage[0] == 'd' || ClientMessage[0] == 'D') { array[0].x += 1; }
            if (ClientMessage[0] == 'w' || ClientMessage[0] == 'W') { array[0].y -= 1; }
            if (ClientMessage[0] == 'a' || ClientMessage[0] == 'A') { array[0].x -= 1; }
            if (ClientMessage[0] == 's' || ClientMessage[0] == 'S') { array[0].y += 1; }

          
            //Send character to the server
            send(sock, ClientMessage, strlen(ClientMessage), 0);

            //Recieve Response from server
            if (recv(sock, ServerMessage, BUFF, 0) == -1)
            {
                printf("[-] recv error \n");
                exit(1);
            }

            /* Window Collision */
            if (array[0].x < 1) { array[0].x = WIDTH - 3; dir = 'a';  }
            if (array[0].x > WIDTH - 3) { array[0].x = 2; dir = 'd';  }
            if (array[0].y < 1) { array[0].y = HEIGHT - 3; dir = 'w'; }
            if (array[0].y > HEIGHT - 3) { array[0].y = 1; dir = 's'; }

            if (isIn(array[0].x, array[0].y, total))
            {
                endwin();
                printf("\n\n\nYOU LOST!!! YOU SCORED: %d\n", total);
                exit(-1);
            }


            /* Collision Detection */
            if (array[0].x == APPLE_X && array[0].y == APPLE_Y)
            {
                APPLE_X = get_rand(WIDTH - 15);
                APPLE_Y = get_rand(HEIGHT - 15);
                if (isIn(APPLE_X, APPLE_Y, total))
                {
                	APPLE_X = get_rand(WIDTH - 15);
                	APPLE_Y = get_rand(HEIGHT - 15);
                }
                array[total].y = array[total - 1].y;  
                array[total].x = array[total - 1].x;    
                total += 1;       
                if ( !(start_speed - 25 < 25) ){
                    start_speed - 25;
                }     
            } 

            /* Shift Elements */
            for (int i = total - 1; i > 0; i--)
            {
                array[i].y = array[i - 1].y;
                array[i].x = array[i - 1].x;
            }

        endwin();
        }
}





int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in6 address; 
 

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
        game(sock);
    }
    return 0;
}
