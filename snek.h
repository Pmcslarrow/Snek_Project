#include <sys/socket.h> // for socket()
#include <arpa/inet.h>  // for add6
#include <stdio.h>      // for printf()
#include <unistd.h>     // for read()
#include <stdlib.h>     // for malloc()
#include <string.h>     // for strlen()
#include <time.h>       // for time()
#include <fcntl.h>      // for fcntl()
#include <curses.h>

// sockets
#define PORT 0xC399     // get it? CS 399?
#define DOMAIN AF_INET6 // ipv6
#define LOOPBACK "::1"  // "loopback" ipv6 - network locally

// debug
#define VERBOSE 1
#define BUFF 260

// booleans
#define TRUE 1
#define FALSE 0
//typedef int bool;

// gameplay
#define HIGH 23
#define WIDE 84

#define WINDOW_HEIGHT 20
#define WINDOW_START_X 1
#define WINDOW_START_Y 1

#define ROWS 23
#define COLS 76

#define SNAK '&'
#define SNEK 'O'

#define APPLE '@'

#define REDO 'r'
#define QUIT 'q'

#define FORE 'w'
#define BACK 's'
#define LEFT 'a'
#define RITE 'd'


// shorter names for addresses and casts
typedef struct sockaddr_in6 *add6;
typedef struct sockaddr *add4;

