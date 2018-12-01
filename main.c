#include <stdlib.h>
#include <time.h>
#include "smtp.h"
#include "menus.h"

char UID[100];
char PWD[60];

/*
 * Function: main
 * Input: void
 * Return: error codes
 *      0: No error
 *      1: Screen is too small
 *      2: Memory allocating error
 *      69: Other error, because there can be error occurred.
 */
/*!
 * @brief Das ist meine Main Funktion, wo sie HomeMenu lauft, und prüft Fehler.
 * @return Fehler Kode:\n
 * 0: Kein Fehler\n
 * 1: Bildschirm zu klein\n
 * 2: Memory allocating Fehler\n
 * 69: Anderer Fehler
 */
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
