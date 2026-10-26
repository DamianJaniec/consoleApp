#include <iostream>
#include "include/graph.h"


int main()
{
    graph::consoleInit();


    graph::gotoxy(12,12);
    graph::setFg(120,90,60);
    graph::setBg(170,170,170);


    printf("Hello World! \n");
    fflush(stdout);

    while(true)
    {
        std::cin.get();
    }
    return 0;
}