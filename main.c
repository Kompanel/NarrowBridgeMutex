#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "bridge.h"
#include "queue.h"
#include <string.h>

int main(int argc, char **argv){
    //przepuszczenie dopiero jak posiada wiecej niz jeden argument
    assert(argc > 1);

    //pobranie ilości samochodów, wraz z pominięciem '-'
    int N = atoi(++argv[1]);

    

    //gdy jest niepoprawna liczba samochodów
    if(N <= 0){
        perror("Wrong number of cars");
        exit(-1);
    }

    if(argc > 2)
    {
        if(strcmp(argv[2], "-debug")==0);
            set_debugg(1);
    }

    initialization(N);

    return 0;
}