#include <stdlib.h>

#ifndef GHA2018_SMTP_H
#define GHA2018_SMTP_H

extern char UID[40];
extern char PWD[40];
extern char text[1000];
extern char addressTo[50];
extern char subjectMail[50];
extern char text[1000];

struct upload_status;
static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
int curlsmtp(void);
int connectToServer();


#endif //GHA2018_SMTP_H