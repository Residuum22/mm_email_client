#include <stdlib.h>

#ifndef GHA2018_SMTP_H
#define GHA2018_SMTP_H

extern char UID[100];
extern char PWD[60];
extern char text[1000];
extern char addressTo[100];
extern char subjectMail[50];

/*!
 * Funktion: payload_source
*/
/*
 * Function: payload_source
 * Input: a lot of things :) Everything which need to make a data
 * Output: lenght of the data
 */
static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);

/* Function: curlSmtpEmailSending
 * Input: void
 * Return: result of the connection
 * Here make the email a header and send the email
 */
int curlSmtpEmailSending(void);

/* Function: connectToServer
 * Input: stusFlag pounter, and exitFlag pointer
 * Return: void
 * Here the curl check the connection with the email address and the password.
 * If the password and the email is good set the status flag 1 and the exit flag 0
 */
void connectToServer(int *statusFlag, int *exitFlag);


#endif //GHA2018_SMTP_H