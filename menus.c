#include "menus.h"
#include "accounts.h"

char text[1000]; //Text array Global variable
char addressTo[100]; //AddressTo global variable for sending
char subjectMail[50]; //A global variable for the mail subject

char *choicesMenu[] = { //Home menu choices list
        "Logining in Mail Account",
        "Sending an E-mail",
        "Accounts",
        "Exit",
        (char *) NULL,
};

char *choicesLog[] = { //Choices for logging menu
        "E-mail",
        "Password",
        "Exit",
        (char *) NULL,
};

char *choicesAccount[] = {
        "List accounts",
        "Add account",
        "Delete account",
        "Exit",
        (char *) NULL,
};

int homeMenu() {
    statusFlag = 0;
    /*Create the variables for homeMenu*/
    ITEM **menuItems;
    ITEM **loginItems;
    ITEM **accountItem;
    MENU *menu;
    MENU *login;
    MENU *account;
    WINDOW *menuWindow;
    WINDOW *loginWindow;
    WINDOW *accountWindow;
    int countChoicesMenu, countChoicesLogin, countChoicesAccount, i, character;


    /* Initialize curses */
    initscr(); //Initialize screen
    start_color(); //Start colors
    cbreak();
    noecho();
    keypad(stdscr, TRUE); //screen can be edited with keypad

    getmaxyx(stdscr, ycoord, xcoord); //Get the size of the terminal
    if (ycoord < 24 || xcoord < 80) {  //If it is too small return with error code 1
        endwin();
        return 1;
    }

    init_pair(1, COLOR_YELLOW, COLOR_BLACK); //Color setting

    /*Create items */
    //Give the sizes
    countChoicesMenu = sizeof(choicesMenu) / sizeof(choicesMenu[0]);
    countChoicesLogin = sizeof(choicesLog) / sizeof(choicesLog[0]);
    countChoicesAccount = sizeof(choicesAccount) / sizeof(choicesAccount[0]);

    //Dynamic memory allocation
    menuItems = (ITEM **) calloc((unsigned) (countChoicesMenu + 1), sizeof(ITEM *));
    loginItems = (ITEM **) calloc((unsigned) (countChoicesLogin + 1), sizeof(ITEM *));
    accountItem = (ITEM **) calloc((unsigned) (countChoicesAccount + 1), sizeof(ITEM *));

    if (menuItems == NULL || loginItems == NULL || accountItem == NULL) {
        free(menuItems);
        free(loginItems);
        free(accountItem);
        return 2;
    }

    //Dynamic memory allocation for chices
    for (i = 0; i < countChoicesMenu; ++i)
        menuItems[i] = new_item(choicesMenu[i], choicesMenu[i]);
    for (i = 0; i < countChoicesLogin; ++i)
        loginItems[i] = new_item(choicesLog[i], choicesLog[i]);
    for (i = 0; i < countChoicesAccount; ++i)
        accountItem[i] = new_item(choicesAccount[i], choicesAccount[i]);

    //Give its end a NULL pointer
    menuItems[countChoicesMenu] = (ITEM *) NULL;
    loginItems[countChoicesLogin] = (ITEM *) NULL;
    accountItem[countChoicesAccount] = (ITEM *) NULL;

    /* Create menu */
    menu = new_menu(menuItems);
    login = new_menu(loginItems);
    account = new_menu(accountItem);

    /* Create the window to be associated with the menu */

    //Set the menu size
    menuWindow = newwin(ycoord - 2, xcoord - 2, 1, 1);
    loginWindow = newwin(ycoord - 2, xcoord - 2, 1, 1);
    accountWindow = newwin(ycoord - 2, xcoord - 2, 1, 1);

    //Enabling keypad
    keypad(menuWindow, TRUE);
    keypad(loginWindow, TRUE);
    keypad(accountWindow, TRUE);


    /* Set main window and sub window */
    /* 2nd coord is the long, 3th is the long from the window and the marker*/
    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, derwin(menuWindow, 10, (int) strlen(choicesMenu[0]) + 2, 5,
                              (xcoord - (int) strlen(choicesMenu[0])) / 2));
    set_menu_win(login, loginWindow);
    set_menu_sub(login, derwin(loginWindow, 10, (int) strlen(choicesLog[1]) + 2, 5,
                               (xcoord - (int) strlen(choicesLog[0])) / 2));
    set_menu_win(account, accountWindow);
    set_menu_sub(account, derwin(accountWindow, 10, (int) strlen(choicesAccount[0]) + 2, 5,
                                 (xcoord - (int) strlen(choicesAccount[0])) / 2));

    /* Set menu mark to the string " > " */
    set_menu_mark(menu, "> ");
    set_menu_mark(login, "> ");
    set_menu_mark(account, "> ");

    /* Print a border around the main window and print a title */
    box(menuWindow, 0, 0);
    box(loginWindow, 0, 0);
    box(accountWindow, 0, 0);

    char stringMenu[] = {"Welcome in my SMTP Client"};
    print_in_middle(menuWindow, 1, (int) (xcoord - strlen(stringMenu)) / 2, (int) strlen(stringMenu), stringMenu,
                    COLOR_PAIR(1));
    refresh();

    /* Post the menu */
    post_menu(menu);
    wrefresh(menuWindow);

    char buffer[100]; //Buffer for the choices
    int exitFlag = 1; //Exit flag
    mvprintw(LINES - 4, 4, "Status: Please Log In!"); //Print to screen the logging status
    refresh(); //Refresh the screen
    wrefresh(menuWindow); //Refresh the window
    pos_menu_cursor(menu); //Move the cursor the the menu first item
    while ((exitFlag) && (character = wgetch(menuWindow)) != KEY_F(1)) { //Until F1 or exit choice it will run
        switch (character) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM); //Move down
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM); //Move up
                break;
            case 10: /* Enter */
                strcpy(buffer, item_name(current_item(menu))); //Copy to buffer the current list item
                if (strcmp(choicesMenu[0], buffer) == 0) { //If the login
                    unpost_menu(menu); //Unpost menu
                    wrefresh(menuWindow); //Refresh
                    loginMenuWindow(login, loginWindow, COLOR_PAIR(1)); //Call loginMenuWindow, that call other menu
                    clear(); //Clear the last state
                    print_in_middle(menuWindow, 1, (int) (xcoord - strlen(stringMenu)) / 2, (int) strlen(stringMenu),
                                    stringMenu, COLOR_PAIR(1)); //Print out the welcome message
                    box(menuWindow, 0, 0); //Print a box
                    post_menu(menu); //Post the menu
                    wrefresh(menuWindow); //Refresh the window
                    refresh(); //Refresh

                } else if (strcmp(choicesMenu[1], buffer) == 0 && statusFlag) { //E-mail send, if logged
                    int errorFlag = 0;
                    unpost_menu(menu); //Unpost menu
                    refresh(); //Refresh
                    creatingEmailText(); //Call sending email function
                    curlSmtpEmailSending(&errorFlag); //Call curl to send, if error occured flag will be 1
                    if (errorFlag) {
                        clear(); //Clear screen
                        mvprintw(ycoord / 2 - 5,
                                 (int) (xcoord - strlen("E-mail hasn't sent, press ENTER and try again")) / 2, "%s",
                                 "E-mail hasn't sent, press ENTER and try again"); //Print this out when ERROR
                        while (getch() != '\n'); //Loop until enter
                    } else {
                        clear(); //Clear screen
                        mvprintw(ycoord / 2 - 5,
                                 (int) (xcoord - strlen("E-mail has sent, press ENTER to continue")) / 2, "%s",
                                 "E-mail has sent, press ENTER to continue"); //Print this out when OK
                        while (getch() != '\n'); //Loop until enter
                    }
                    clear(); //Clear the last state
                    print_in_middle(menuWindow, 1, (int) (xcoord - strlen(stringMenu)) / 2, (int) strlen(stringMenu),
                                    stringMenu, COLOR_PAIR(1)); //Print out the welcome message
                    post_menu(menu); //Post menu again
                    box(menuWindow, 0, 0); //Create a box
                    refresh(); //Refresh
                    wrefresh(menuWindow); //Refresh the window
                } else if (strcmp(choicesMenu[2], buffer) == 0) {
                    unpost_menu(menu);
                    clear();
                    post_menu(account);
                    pos_menu_cursor(account);
                    wrefresh(accountWindow);
                    box(accountWindow, 0, 0);
                    int c;
                    refresh();
                    while ((c = wgetch(accountWindow)) != KEY_F(1)) {
                        switch (c) {
                            case KEY_DOWN:
                                menu_driver(account, REQ_DOWN_ITEM);
                                break;
                            case KEY_UP:
                                menu_driver(account, REQ_UP_ITEM);
                                break;
                            case 10: //Enter
                                //Copy the current item name in a buffer for use
                                strcpy(buffer, item_name(current_item(account)));

                                //Wenn the customer choose the first item, what is E-mail
                                if (strcmp(choicesAccount[0], buffer) == 0) { //List account
                                    //Todo list account making a linked list.
                                    readyAccounts();
                                } else if (strcmp(choicesAccount[1], buffer) == 0) { //Add account
                                    //TODO add account
                                } else if (strcmp(choicesAccount[2], buffer) == 0) { //Delete account
                                    //Delete accoun
                                    writeOutAccount();
                                }
                            default:
                                break;
                        }
                        wrefresh(accountWindow);
                    }
                    unpost_menu(account);
                    clear();
                    print_in_middle(menuWindow, 1, (int) (xcoord - strlen(stringMenu)) / 2, (int) strlen(stringMenu),
                                    stringMenu, COLOR_PAIR(1)); //Print out the welcome message
                    post_menu(menu); //Post menu again
                    box(menuWindow, 0, 0); //Create a box
                    refresh(); //Refresh
                    wrefresh(menuWindow); //Refresh the window
                } else if (strcmp(choicesMenu[3], buffer) == 0) { //When the customer choice exit
                    unpost_menu(menu); //Unpost menu
                    wrefresh(loginWindow); //Refresh the screen
                    int scan; //Init a character for scan
                    clear(); //Clear the screen
                    mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("Do you want to exit?(y/n)")) / 2, "%s",
                             "Do you want to exit?(y/n)"); //Move this to screen
                    while ((scan = getch()) != KEY_F(1)) { //While loop until key y or n
                        if (scan == 'y') { //If y,
                            exitFlag = 0; // exitFlag = 0 to exit
                            break;
                        } else if (scan == 'n') { //If n,
                            exitFlag = 1; //exit flag = 1 to no exit
                            break;
                        }
                    }
                    clear(); //Clear screen
                    box(menuWindow, 0, 0); //Make a box
                    post_menu(menu); //Post the menu
                    mvprintw(LINES - 4, 4, "Status: Please Log In!"); //
                    refresh(); //Refresh the screen
                    wrefresh(menuWindow); //Refresh the menu window

                }
                if (statusFlag) { //If the statusflag is 1, so logged, write this out
                    mvprintw(LINES - 5, 4, "E-mail: %s", UID); //Write current e-mail address
                    mvprintw(LINES - 4, 4, "Status: Logged"); //Write this message out
                    refresh(); //Refresh the screen
                }
                pos_menu_cursor(menu); //Move the cursor to menu
                refresh(); //Refresh the screen
                break;
            default:
                break;
        }
        if (!statusFlag) mvprintw(LINES - 4, 4, "Status: Please Log In!"); //If the status isnt logged, write this out
        refresh(); //Refresh the screen
        wrefresh(menuWindow); //Refresh the window
    }

    /* Unpost and free all the memory taken up */
    unpost_menu(menu); //Unpost the menu
    unpost_menu(login); //Unpost the login
    free_menu(menu); //Free menu
    free_menu(login); //Free login
    free_menu(account); //Free account
    for (i = 0; i < countChoicesMenu; ++i) { //Free all choices
        free_item(menuItems[i]);
        free_item(loginItems[i]);
        free_item(accountItem[i]);
    }
    endwin(); //Close the window
    return 0;
}


void loginMenuWindow(MENU *login, WINDOW *loginWindow, chtype color) {
    clear(); //Clear screen

    int scanForScreen; //Scan a caracter from screen, why int? Cause arrow a non ascii caracter
    int i; //Basic counter
    char scanForCharacter; //A char var. for ascii character
    char stringLogin[] = {"Authorization"}; //This is the stringlogin
    print_in_middle(loginWindow, 1, (int) (xcoord - strlen(stringLogin)) / 2, (int) strlen(stringLogin), stringLogin,
                    color); //Print out the stringLogin

    refresh();

    /* Post the menu */
    post_menu(login);
    wrefresh(loginWindow);

    char buffer[50]; //The working buffer for the current item
    int exitFlag = 1; //A flag for exit

    while ((exitFlag) && (scanForScreen = wgetch(loginWindow)) != KEY_F(1)) {
        switch (scanForScreen) {
            case KEY_DOWN:
                menu_driver(login, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(login, REQ_UP_ITEM);
                break;
            case 10: /* Enter */
                //Copy the current item name in a buffer for use
                strcpy(buffer, item_name(current_item(login)));

                //Wenn the customer choose the first item, what is E-mail
                if (strcmp(choicesLog[0], buffer) == 0) {
                    for (i = 0; i < 100; ++i) UID[i] = '\0'; //Clear the last e-mail address
                    int counter = 0; //This is a counter for the caracters in the screen

                    unpost_menu(login); //Unpost the login window menu
                    wrefresh(loginWindow); //Refresh the screen
                    clear(); //Clear the screen

                    mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("E-mail:")) / 2, "%s",
                             "E-mail:"); //Print to the screen this word:
                    move(ycoord / 2 - 4, xcoord / 2); //Move the cursor between the screen
                    while ((scanForCharacter = (char) getch()) != '\n') { //Waiting for a character
                        if (scanForCharacter == 7) { //If it is the back button
                            if (counter > 0) //Counter cant be minus number
                                UID[--counter] = '\0'; //If I clear fill with nulls
                        } else if (counter < 100) //The password can be max 256 character long (gmail)
                            UID[counter++] = scanForCharacter;
                        clear(); //Clear the screen
                        refresh(); //Refresh it
                        mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("E-mail:")) / 2, "%s",
                                 "E-mail:"); //Write to screen this string
                        if (strlen(UID) < xcoord - 15) { //If the screen is too small there will be wrong input
                            mvprintw(ycoord / 2 - 4, (int) (xcoord - strlen(UID)) / 2, "%s",
                                     UID); //Write to screen the current ID
                        } else
                            break;
                        refresh(); //Refresh the screen
                        move(ycoord / 2 - 4,
                             ((int) (xcoord - strlen(UID)) / 2) + counter); //Move the cursor after the last character
                    }
                    print_in_middle(loginWindow, 1, (int) (xcoord - strlen(stringLogin)) / 2,
                                    (int) strlen(stringLogin),
                                    stringLogin, color); //Print the login string in to the middle
                    box(loginWindow, 0, 0); //Draw a box
                    post_menu(login); //Post the login menu
                    wrefresh(loginWindow); //Refresh the window
                } else if (strcmp(choicesLog[1], buffer) == 0) { //When the customer choose password menu item
                    char secretPWD[50]; //This is a buffer for password's '*', because I dont want to write it out
                    for (i = 0; i < 60; ++i) PWD[i] = '\0'; //Write 0-s to the array
                    int counter = 0; //Basic counter for password

                    unpost_menu(login); //Unpost the menu
                    wrefresh(loginWindow); //Refresh the screen
                    clear(); //Clear the screen

                    mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("Password:")) / 2, "%s",
                             "Password:"); //Print to the screen this string
                    move(ycoord / 2 - 4, xcoord / 2); //Move the cursor into a new line and middle
                    while ((scanForCharacter = (char) getch()) != '\n') { //Waiting for a character
                        if (scanForCharacter == 7) { //If the character a backspace
                            if (counter > 0) //Cause I cant to go back, as 0
                                PWD[--counter] = '\0'; //Fill PWD with 0 when I delete
                        } else if (counter < 60) //Password can be max 60 character long (gmail)
                            PWD[counter++] = scanForCharacter;
                        clear(); //Clear the screen
                        refresh(); //Refresh it
                        mvprintw(ycoord / 2 - 5, (int) (xcoord - strlen("Password:")) / 2, "%s",
                                 "Password:"); //Print this to middle
                        for (i = 0; i < strlen(PWD); ++i) secretPWD[i] = '*'; //Put secretPWD *-s, as long as the PWD
                        secretPWD[i] = '\0'; //Terminating 0
                        mvprintw(ycoord / 2 - 4, (int) (xcoord - strlen(secretPWD)) / 2, "%s", secretPWD); //Write out *
                        refresh(); //Refresh the screen
                        move(ycoord / 2 - 4,
                             ((int) (xcoord - strlen(PWD)) / 2) + counter); //Move the cursor after the string
                    }
                    connectToServer(&statusFlag, &exitFlag); //Test the logining datas
                    clear();
                    if (statusFlag) mvprintw(LINES - 4, 4, "Status: Logged"); //If the status is logged print to scr

                    print_in_middle(loginWindow, 1, (int) (xcoord - strlen(stringLogin)) / 2,
                                    (int) strlen(stringLogin),
                                    stringLogin, color); //Than post the homemenu
                    box(loginWindow, 0, 0); //Make a box for it
                    post_menu(login); //Post it
                    wrefresh(loginWindow); //Refresh
                } else if (strcmp(choicesLog[2], buffer) == 0) { //If the customer want to exit
                    exitFlag = 0;
                }
                if (UID[0] != 0) mvprintw(LINES - 5, 4, "E-mail: %s", UID); //If the UID is not null, write out
                if (statusFlag) mvprintw(LINES - 4, 4, "Status: Logged"); //Write out Logged, when is logged
                else if (PWD[0] == '\0'); //Or write out nothing
                else
                    mvprintw(LINES - 4, 4, "Wrong E-mail address or password!"); //When the password or the ID incorrect
                pos_menu_cursor(login); //Move the cursor to menu list
                refresh(); //Refresh the screen
                break; //Break
            default: //Default part, do nothing, when other caracter comming
                break;

        }
        wrefresh(loginWindow); //Refresh the screen and menu
    }

    unpost_menu(login); //When I want to do nothing here unpost the login menu and go back
}

void creatingEmailText() {
    int counter = 0; //Basic counter for the caracters
    clear(); //Clear the screen
    mvprintw(LINES - ycoord + 3, 4, "%s", "TO: "); //Print to screen this
    refresh(); //Refresh
    char scanForCharacter; //A char for the variablen
    int ycursor = 3, xcoursor = 4 + (int) strlen("TO: "); //The setting for the To part
    addressTo[counter++] = '<'; //For sending need to be this the first caracter
    while ((scanForCharacter = (char) getch()) != '\n') {
        if (scanForCharacter == 7) { //When the customer press backspace
            if (counter > 1) { //Until the last number
                mvdelch(ycursor, --xcoursor); //Del this character at this position
                addressTo[--counter] = '\0'; //Put terminating 0 to addresTo, where I deleted
            }
        } else if (counter < 50 - 2 && counter <= xcoord - 4) {
            mvaddch(ycursor, xcoursor++, scanForCharacter); //Add the scanned character to screen
            addressTo[counter++] = scanForCharacter; //Add the scanned caracter to addressTo array
        }
    }
    addressTo[counter++] = '>'; //Add this character to end of string
    addressTo[counter] = '\0'; //Add safe terminating 0

    mvprintw(LINES - ycoord + 4, 4, "%s", "Subject: "); //Print the next part, the subject
    refresh(); //Refresh the screen
    ycursor = 4, xcoursor = 4 + (int) strlen("Subject: "); //Give to program the new coursor states
    counter = 0; //Set counter null
    while ((scanForCharacter = (char) getch()) != '\n') { //In this cycle get caracters
        if (scanForCharacter == 7) { //If the customer want to delete
            if (counter > 0) { //And the counter bigger as null
                mvdelch(ycursor, --xcoursor); //Delete caracter
                subjectMail[--counter] = '\0'; //Set the value to 0
            }
        } else if (counter < 50 - 1 && xcoursor <= xcoord -
                                                   4) { //If the counter smaller as the array, and the coursor state smaller as the window edge
            mvaddch(ycursor, xcoursor++, scanForCharacter); //Delete the caracter
            subjectMail[counter++] = scanForCharacter; //Put in the array the character
        }
    }
    subjectMail[counter] = '\0'; //Add terminating null

    mvprintw(LINES - ycoord + 6, 4, "%s", "Text:"); //Print the next part, to the screen

    ycursor = 8, xcoursor = 4; //Set the coursor
    move(ycursor, xcoursor); //Set the coursor position
    refresh(); //Refresh the window

    int startx = xcoursor; //The starting coordinate for x
    int starty = ycursor; //The starting coordinate for y

    counter = 0; //Set the counter to null

    while ((scanForCharacter = (char) getch()) != '\n') { //Waiting for a character
        if (scanForCharacter == 7) { //When the character is backspace delete a character
            if (counter > 0) { //Until counter bigger as 0
                if (xcoursor > startx) {
                    mvdelch(ycursor, --xcoursor); //Delete character
                } else if (ycursor > starty) { //Delete last row last character
                    xcoursor = xcoord - 4; //Set y-xoursor 4 character before the edge of the screen
                    mvdelch(--ycursor, --xcoursor); //Delete character
                }
                text[--counter] = '\0'; //Set 0 the array element
            }
        } else if (counter < 1000 - 1 &&
                   ycursor <= LINES - 3) { //This is the safe mode for upper index, and out of line
            mvaddch(ycursor, xcoursor++, scanForCharacter); //Add character
            text[counter++] = scanForCharacter; //Add to text the character
            if (xcoursor == xcoord - 4) { //When the cursor go to edge - 4
                xcoursor = startx; //Set add coursor to starting x pos.
                move(++ycursor, xcoursor); //Move the coursor to the right position
                refresh(); //Refresh the screen
            }
        }
    }
    text[counter] = '\0'; //Make a 0 to end of array
    //Return
}

//This comes from ncourses example code, so I dont want to write comment.
//Ok I must change a Line..
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = (int) strlen(string);
    temp = (float) (width - length) / 2;
    x = startx + (int) temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}


