#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "smtp.h"


#ifndef GHA2018_HOMEMENU_H
#define GHA2018_HOMEMENU_H


void menuing();


void loginMenuWindow(MENU *login, WINDOW *loginWindow, chtype color);

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

void sendingEmail();

#endif //GHA2018_HOMEMENU_H
