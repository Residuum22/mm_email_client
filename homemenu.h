#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "smtp.h"

#ifndef GHA2018_HOMEMENU_H
#define GHA2018_HOMEMENU_H


int ycoord, xcoord;

char text[1000];
char addressTo[50];
char subjectMail[50];
int statusFlag = 0;

char *choicesMenu[] = {
        "Logining in Mail Account",
        "Sending an E-mail",
        "Accounts",
        "Exit",
        (char *) NULL,
};

char *choicesLog[] = {
        "E-mail",
        "Password",
        "Exit",
        (char *) NULL,
};

void sendingEmail();

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

void logining(ITEM **loginItems, MENU *login, WINDOW *loginWindow, int countChoicesLogin, chtype color,
              ITEM **loggedAccount);

void menuing() {
    UID[0] = '\0';
    PWD[0] = '\0';
    ITEM **menuItems;
    ITEM **loginItems;
    ITEM **loggedAccount;
    int c;
    MENU *menu;
    MENU *login;
    WINDOW *menuWindow;
    WINDOW *loginWindow;
    WINDOW *createMailWindow;
    int countChoicesMenu, countChoicesLogin, i;


    /* Initialize curses */
    initscr(); //Initialize screen
    start_color(); //Start colors
    cbreak();
    noecho();
    keypad(stdscr, TRUE); //screen can be edited with keypad

    getmaxyx(stdscr, ycoord, xcoord); //Get the size of the terminal

    init_pair(1, COLOR_YELLOW, COLOR_BLACK); //Color setting

    /*Create items */
    countChoicesMenu = sizeof(choicesMenu) / sizeof(choicesMenu[0]);
    countChoicesLogin = sizeof(choicesLog) / sizeof(choicesLog[0]);

    menuItems = (ITEM **) calloc((unsigned) (countChoicesMenu + 1), sizeof(ITEM *));
    loginItems = (ITEM **) calloc((unsigned) (countChoicesLogin + 1), sizeof(ITEM *));
    loggedAccount = (ITEM **) calloc((unsigned) 4, sizeof(ITEM *));

    for (i = 0; i < countChoicesMenu; ++i)
        menuItems[i] = new_item(choicesMenu[i], choicesMenu[i]);
    for (i = 0; i < countChoicesLogin; ++i)
        loginItems[i] = new_item(choicesLog[i], choicesLog[i]);

    menuItems[countChoicesMenu] = (ITEM *) NULL;
    loginItems[countChoicesLogin] = (ITEM *) NULL;
    loggedAccount[3] = (ITEM *) NULL;

    /* Create menu */
    menu = new_menu((ITEM **) menuItems);
    login = new_menu((ITEM **) loginItems);

    /* Create the window to be associated with the menu */

    //ezzel a keretet állítom vagyis az ablakot
    menuWindow = newwin(ycoord - 2, xcoord - 2, 1, 1);
    loginWindow = newwin(ycoord - 2, xcoord - 2, 1, 1);
    createMailWindow = newwin(ycoord - 2, xcoord - 2, 1, 1);

    keypad(menuWindow, TRUE);
    keypad(loginWindow, TRUE);
    keypad(createMailWindow, TRUE);
    refresh();
    wrefresh(menuWindow);

    /* Set main window and sub window */
    set_menu_win(menu, menuWindow);
    //2koord a osszúság, 3. a windowstol a hossz
    //+2 az strlenhez mert ott a marker...
    set_menu_sub(menu, derwin(menuWindow, 10, (int) strlen(choicesMenu[0]) + 2, 5,
                              (xcoord - (int) strlen(choicesMenu[0])) / 2));
    set_menu_win(login, loginWindow);
    set_menu_sub(login, derwin(loginWindow, 10, (int) strlen(choicesLog[1]) + 2, 5,
                               (xcoord - (int) strlen(choicesLog[0])) / 2));

    /* Set menu mark to the string " * " */
    set_menu_mark(menu, "> ");
    set_menu_mark(login, "> ");

    /* Print a border around the main window and print a title */
    box(menuWindow, 0, 0);
    box(loginWindow, 0, 0);

    char stringMenu[] = {"Welcome in my SMTP Client"};
    print_in_middle(menuWindow, 1, (int) (xcoord - strlen(stringMenu)) / 2, (int) strlen(stringMenu), stringMenu,
                    COLOR_PAIR(1));
    refresh();

    /* Post the menu */
    post_menu(menu);
    wrefresh(menuWindow);

    char buffer[100];
    int exitFlag = 100000;
    mvprintw(LINES - 4, 4, "Status: Please Log In!");
    refresh();
    pos_menu_cursor(menu);
    while ((exitFlag) && (c = wgetch(menuWindow)) != KEY_F(1)) {
        switch (c) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10: /* Enter */
                strcpy(buffer, item_name(current_item(menu)));
                if (strcmp(choicesMenu[0], buffer) == 0) {
                    unpost_menu(menu);
                    wrefresh(menuWindow);
                    logining(loginItems, login, loginWindow, countChoicesLogin, COLOR_PAIR(1), loggedAccount);
                    post_menu(menu);
                    wrefresh(menuWindow);
                    refresh();
                } else if (strcmp(choicesMenu[1], buffer) == 0) {
                    unpost_menu(menu);
                    refresh();
                    sendingEmail();
                    curlsmtp();
                    post_menu(menu);
                    box(menuWindow, 0, 0);
                    refresh();
                    wrefresh(menuWindow);
                } else if (strcmp(choicesMenu[3], buffer) == 0) {
                    exitFlag = 0;
                }
                if (UID[0] != 0) mvprintw(LINES - 5, 4, "E-mail: %s", UID);
                if (statusFlag) {
                    //grosshausaufgabe2018@gmail.com
                    mvprintw(LINES - 4, 4, "Status: Logged");
                    refresh();
                }
                else mvprintw(LINES - 4, 4, "Status: Please Log in with a good account!");
                pos_menu_cursor(menu);
                refresh();
                break;

        }
        wrefresh(menuWindow);
    }

    /* Unpost and free all the memory taken up */
    unpost_menu(menu);
    unpost_menu(login);
    free_menu(menu);
    free_menu(login);
    for (i = 0; i < countChoicesMenu; ++i) {
        free_item(menuItems[i]);
        free_item(loginItems[i]);
    }
    endwin();
    return;
}

void logining(ITEM **loginItems, MENU *login, WINDOW *loginWindow, int countChoicesLogin, chtype color,
              ITEM **loggedAccount) {
    int c;
    int i;
    char character;
    char stringLogin[] = {"Authorization"};
    print_in_middle(loginWindow, 1, (xcoord - strlen(stringLogin)) / 2, strlen(stringLogin), stringLogin, color);
    refresh();

    /* Post the menu */
    post_menu(login);
    wrefresh(loginWindow);

    char buffer[100];
    char bufferForError[50];
    char str[100];
    int exitFlag = 10000;
    while ((exitFlag) && (c = wgetch(loginWindow)) != KEY_F(1)) {
        switch (c) {
            case KEY_DOWN:
                menu_driver(login, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(login, REQ_UP_ITEM);
                break;
            case 10: /* Enter */
                strcpy(buffer, item_name(current_item(login)));
                if (strcmp(choicesLog[0], buffer) == 0) {
                    char scan;
                    int xcoursor, ycursor, starty, startx, counter = 0;
                    ycursor = starty = ycoord / 2 - 4;
                    xcoursor = startx = xcoord / 2 - 5;
                    unpost_menu(login);
                    wrefresh(loginWindow);
                    clear();
                    mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("E-mail:")) / 2, "%s", "E-mail:");
                    move(starty, startx);
                    while ((scan = (char) getch()) != '\n') {
                        if (scan == 7) {
                            if (xcoursor != startx) {
                                mvdelch(ycursor, --xcoursor);
                                UID[--counter] = '\0';
                            }
                        } else {
                            mvaddch(ycursor, xcoursor++, scan);
                            UID[counter++] = scan;
                        }
                    }
                    clear();
                    print_in_middle(loginWindow, 1, (int) (xcoord - strlen(stringLogin)) / 2, (int) strlen(stringLogin),
                                    stringLogin, color);
                    box(loginWindow, 0, 0);
                    post_menu(login);
                    wrefresh(loginWindow);
                } else if (strcmp(choicesLog[1], buffer) == 0) {
                    char scan;
                    int xcoursor, ycursor, starty, startx, counter = 0;
                    ycursor = starty = ycoord / 2 - 4;
                    xcoursor = startx = xcoord / 2 - 5;
                    unpost_menu(login);
                    wrefresh(loginWindow);
                    clear();
                    mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("Password:")) / 2, "%s", "Password:");
                    move(starty, startx);
                    while ((scan = (char) getch()) != '\n') {
                        if (scan == 7) {
                            if (xcoursor != startx) {
                                mvdelch(ycursor, --xcoursor);
                                PWD[--counter] = '\0';
                            }
                        } else {
                            mvaddch(ycursor, xcoursor++, '*');
                            PWD[counter++] = scan;
                        }
                    }
                    CURL *curl;
                    CURLcode res = CURLE_OK;

                    curl = curl_easy_init();
                    if (curl) {
                        curl_easy_setopt(curl, CURLOPT_USERNAME, UID);
                        curl_easy_setopt(curl, CURLOPT_PASSWORD, PWD);


                        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
                        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
                        res = curl_easy_perform(curl);
                        clear();

                        strcpy(bufferForError, curl_easy_strerror(res));


                        curl_easy_cleanup(curl);

                        if (!strcmp(bufferForError, "No error")) statusFlag = 1;
                        if (statusFlag) mvprintw(LINES - 4, 4, "Status: Logged");
                        else mvprintw(LINES - 4, 4, "Wrong E-mail address orr password!");
                    }
                    clear();
                    print_in_middle(loginWindow, 1, (int) (xcoord - strlen(stringLogin)) / 2,
                                    (int) strlen(stringLogin),
                                    stringLogin, color);
                    box(loginWindow, 0, 0);
                    post_menu(login);
                    wrefresh(loginWindow);
                    //connectToServer();
                } else if (strcmp(choicesLog[2], buffer) == 0) {
                    exitFlag = 0;
                }
                if (UID[0] != 0) mvprintw(LINES - 5, 4, "E-mail: %s", UID);
                if (statusFlag) mvprintw(LINES - 4, 4, "Status: Logged");
                pos_menu_cursor(login);
                refresh();
                break;

        }
        wrefresh(loginWindow);
    }

    unpost_menu(login);
}

void sendingEmail() {
    int counter = 0;
    clear();
    mvprintw(LINES - ycoord + 3, 4, "%s", "TO: ");
    refresh();
    char scan;
    int ycursor = 3, startx, starty, xcoursor = 4 + (int) strlen("TO: ");
    startx = xcoursor;
    addressTo[counter++] = '<';
    while ((scan = (char) getch()) != '\n') {
        if (scan == 7) {
            if (xcoursor != startx) {
                mvdelch(ycursor, --xcoursor);
                addressTo[--counter] = '\0';
            }
        } else {
            mvaddch(ycursor, xcoursor++, scan);
            addressTo[counter++] = scan;
        }
    }
    addressTo[counter] = '>';

    mvprintw(LINES - ycoord + 4, 4, "%s", "Subject: ");
    refresh();
    ycursor = 4, xcoursor = 4 + (int) strlen("Subject: ");
    startx = xcoursor;
    counter = 0;
    while ((scan = (char) getch()) != '\n') {
        if (scan == 7) {
            if (xcoursor != startx) {
                mvdelch(ycursor, --xcoursor);
                subjectMail[--counter] = '\0';
            }
        } else {
            mvaddch(ycursor, xcoursor++, scan);
            subjectMail[counter++] = scan;
        }
    }
    mvprintw(LINES - ycoord + 6, 4, "%s", "Text:");
    refresh();
    ycursor = 7, xcoursor = 4;
    move(8, 4);
    startx = xcoursor;
    starty = ycursor;
    ycursor++;
    counter = 0;
    while ((scan = (char) getch()) != '\n') {
        if (scan == 7) {
            if (xcoursor > startx) {
                mvdelch(ycursor, --xcoursor);
                text[--counter] =  '\0';
            } else if (xcoursor == startx && ycursor > starty + 1) {
                ycursor--;
                xcoursor = xcoord - 4;
                mvdelch(ycursor, xcoursor);
                text[--counter] =  '\0';
            }
        } else {
            mvaddch(ycursor, xcoursor++, scan);
            text[counter++] = scan;

            if (xcoursor == xcoord - 4) {
                xcoursor = 4;
                ycursor++;
                text[counter++] = scan;

            }
        }
    }
    text[counter] = '\0';


}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = (int) strlen(string);
    temp = (width - length) / 2;
    x = startx + (int) temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}


#endif //GHA2018_HOMEMENU_H
