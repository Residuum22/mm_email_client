#include <stdlib.h>
#include <time.h>
#include "smtp.h"
#include "menus.h"

char UID[100];
char PWD[60];

int main(){
    int errorCode;
    errorCode = homeMenu();
    switch (errorCode){
        case 0:
            return (EXIT_SUCCESS);
        case 1:
            printf("Screen is too small to start!\n");
            return 1;
        case 2:
            printf("Memory allocating failed");
            return 2;
        default:
            return 69;
    }
}
