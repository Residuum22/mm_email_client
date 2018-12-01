#include <stdlib.h>

#ifndef GHA2018_SMTP_H
#define GHA2018_SMTP_H

/// @var char UID[100]
/// @brief Array von E-mail Address
extern char UID[100];

/// @var char PWD[60]
/// @brief Passwort von E-mail
extern char PWD[60];

/// @var char text[1000]
/// @brief die Texte von der E-mail Addresse
extern char text[1000];

/// @var char addressTo[100]
/// @brief die Adresse von Empfänger
extern char addressTo[100];

/// @var char subjectMail[50]
/// @brief der Titel von E-mail
extern char subjectMail[50];

/*!
 * @var char *mytext[7]
 * @brief Das ist mein 2D array für E-mail header. Es gibt hier die Date, Empfänger, Titel und E-mail Texte dann gibt es
 * an der Ende einen Null pointer.
 */
char *mytext[7]; //Global variable for the email header

/*!
 * @struct upload_status
 * @brief Das ist ein Struct von Curl Beispiel. Speicher das Programm, welche Linie von mytext 2D array lesen
 */
struct upload_status;

/*
 * Function: payload_source
 * Input: a lot of things :) Everything which need to make a data
 * Output: lenght of the data
 */
/*!
 * Diese Funktion kommt vom Beispiel des Curls
 * Macht einen Datei von meine header
 */
static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);

/* Function: curlSmtpEmailSending
 * Input: void
 * Return: result of the connection
 * Here make the email a header and send the email
 */
/*!
 * @brief Hier macht das Program einen "Header" datei für email. Es gibt hier die Stunde, Empfänger, Sender, Titel und
 * einen Null Pointer an der Ende.
 * @return Ergebnis von Konnektion
 */
int curlSmtpEmailSending(int *errorFlag);

/* Function: connectToServer
 * Input: stusFlag pounter, and exitFlag pointer
 * Return: void
 * Here the curl check the connection with the email address and the password.
 * If the password and the email is good set the status flag 1 and the exit flag 0
 */
/*!
 * @param statusFlag: Status von Anmeldung
 * @param exitFlag: Status von Ende diese Teil von Programm
 * @brief Diese Funktion kann Kommunitieren mit Gmail via Internet und kann einen Status fragen. Wenn E-mail Addresse
 * und Passwort gut ist, macht StatusFlag 1, und exitFlag 0, und geht in "homeMenu" zürück. Wenn etwas schlecht
 * war, bleibt das Proggramm in LoginMenuWindow.
 */
void connectToServer(int *statusFlag, int *exitFlag);


#endif //GHA2018_SMTP_H