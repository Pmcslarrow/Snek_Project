//Simply tired of running all the compiling and running commands so I do it here for one line

#include "snek.h"

int main()
{
    system("gcc client.c -lncurses && mv a.out client.out");
    system("gcc server.c -lncurses && mv a.out server.out");
    system("gcc snek.c && mv a.out snek.out");
    return 0;
}
