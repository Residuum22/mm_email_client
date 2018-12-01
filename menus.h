#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "smtp.h"


#ifndef GHA2018_HOMEMENU_H
#define GHA2018_HOMEMENU_H

/*!
 * @var int ycoord
 * @brief y Koordinate von Bildschirm, muss es für automaten Anfangsbildschirm
 * @var int xcoord
 * @brief x Koordinate von Bildschirm, muss es für automaten Anfangsbildschirm
 */
int ycoord, xcoord;

/*!
 * @var int statusFlag
 * @brief Status von Anmeldung. Wenn der Benutzer angemeldet hat, kann er E-mail schreiben.
 */
int statusFlag;

/*!
 * @var char *choicesMenu[]
 * @brief Gibt es hier die Elementen von meinem Menu des Hausmenus
 */
extern char *choicesMenu[];

/*!
 * @var char *choicesLog[]
 * @brief Gibt es hier die Elementen von meinem Menu der Anmeldung
 */
extern char *choicesLog[];

/* Function: homeMenu
 * Input: void
 * Return: error code for main
 * This function is the first function, what the main calls. Here do the program everything.
 * First, there is a home menu, where the customer can choose from a list and the program work with the inputs, here the
 * program's formatting the screen with ncurses library
 * */
/*!
 * @return Fehlerkode:\n
 * 1: Bildschirm ist zu klein für das Programm\n
 * 2: Memory zuteilen Fehler
 * @brief Diese Funktion ist die erste Funktion, was main Funktion lauft. Mein Programm macht alles Hier. An dem
 * Bildschirm gibt es erstes Mal. Die Benutzern können aus einem Liste wählen, z.B. Anmelden, E-mail senden, oder
 * Kontakten auswählen.
 * */
int homeMenu();


/* Function: loginMenuWindow
 * Input: lot of things, but it is verstandbar
 * Output: void
 * This is the logging part of my program. Here the user can log in with a Gmail e-mail address, which is not longer,
 * than 100 character. Can give to program a password (smaller as 61 character). The function call an other function
 * from smtp.h, and check the data, if there are no error the status flag will be 1, exit from this menu, so the
 * program can log in, if the data is wrong status flag will be 0 and print to the screen that the e-mail or the
 * password was not good.
 * */
/*!
 *
 * @param login: Liste des Menus
 * @param loginWindow: Bildschirm pointer
 * @param color: Eine Farbe für Menu Titel
 * @return nichts
 * @brief Das ist einen Part von meniem Programm. Hier kann der Benutzer in Gmail anmelden. E-mail muss kleiner als 100
 * carater sein, und passwort muss kleiner als 60 caracter sein. Diese Function ruf eine andere Funktion von smtp.h und
 * prüft, ob kann man anmelden. Wenn ein Fehler kommen, macht statusFlag 0 und schreibt Fehéler zum Bildschirm aus,
 * aber wenn es OK ist, macht Ende und geht in
 * Hausemenu zurück.
 */
void loginMenuWindow(MENU *login, WINDOW *loginWindow, chtype color);


/*
 * Function: creatingEmailText
 * Input: void
 * Output: void
 * In this function the user can create an email, can add the "to" address, subject and the plain text in ASCII
 */
/*!
 * @brief In dieser Function kann der Benutzer ein E-mail machen, kann er den Empfänger, den Titel und den Text in ű
 * ASCII geben.
 */
void creatingEmailText();


/* Function: print_in_middle
 * Input: a window, start positions, the with of string, the string and a color
 * Return: void
 * This function is an example function from ncourses library. It prints a string in the middle of the screen
 * */
/*!
 *
 * @param win: Einen Bildschirm, woher kann das Programm schreiben
 * @param starty: Anfangsposition an der Y Achse
 * @param startx: Anfangsposition an der X Achse
 * @param width: Weite
 * @param string: Ein Text
 * @param color: Ein Farbentyp
 * @return nichts
 * @brief Diese Funktion ist ein Beispielfunktion von der Ncourses library webseite. Die schreibt ein String in der
 * Mitte des Bildschirmes
 */
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#endif //GHA2018_HOMEMENU_H
