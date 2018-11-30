#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "smtp.h"


#ifndef GHA2018_HOMEMENU_H
#define GHA2018_HOMEMENU_H

/* Function: homeMenu
 * Input: void
 * Return: void
 * This function is the first function, what the main calls. Here do the program everything.
 * First, there is a home menu, where the customer can choose from a list and the program work with the inputs, here the
 * program's formatting the screen with ncurses library
 * */
void homeMenu();


/* Function: loginMenuWindow
 * Input: void
 * Output: void
 * This is the logging part of my program. Here the user can log in with a Gmail e-mail address, which is not longer,
 * than 100 character. Can give to program a password (smaller as 61 character). The function call an other function
 * from smtp.h, and check the data, if there are no error the status flag will be 1, exit from this menu, so the
 * program can log in, if the data is wrong status flag will be 0 and print to the screen that the e-mail or the
 * password was not good.
 * */
void loginMenuWindow(MENU *login, WINDOW *loginWindow, chtype color);



void sendingEmail();


/* Function: print_in_middle
 * Input: a window, start positions, the with of string, the string and a color
 * Return: void
 * This function is an example function from ncourses library. It prints a string in the middle of the screen
 * */
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#endif //GHA2018_HOMEMENU_H
