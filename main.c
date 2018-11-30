#include <stdlib.h>
#include "smtp.h"
#include "menus.h"
#include <time.h>

#define UID_MAX 256
#define PWD_MAX 100

char UID[256];
char PWD[100];

int main(){
    menuing();
    return (EXIT_SUCCESS);
}
