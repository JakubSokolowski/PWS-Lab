//
// Created by jakub on 07.03.18.
//

#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    int steps = argv[1] - '0';
    for(int i=1; i <= steps;i++) {
        printf("Potomny krok: %d \n",i);
        sleep(1);
    }
    exit(1);
}
