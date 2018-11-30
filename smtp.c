/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 *****************************************************************************/
 /*---------------------------------------------------------------------------
  I use curl example with a lots of modifications
 ----------------------------------------------------------------------------*/
#include "smtp.h"
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <time.h>

char *mytext[7]; //Global variable for the email header

struct upload_status { //How many lines read
    int lines_read;
};

// Reading function from the example code modified
static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }

    //data = payload_text[upload_ctx->lines_read];
    data = mytext[upload_ctx->lines_read];

    if(data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}

int curlSmtpEmailSending(void)
{
    time_t now; //Make a time variable
    time(&now); //Get the current time
    char date_buff[33]; //Make a buffer for the date and time
    strftime(date_buff, (33), "%a , %d %b %Y %H:%M:%S", localtime(&now)); //Convert time to string
    CURL *curl; //Make a curl
    CURLcode res = CURLE_OK; //Make curl OK
    struct curl_slist *recipients = NULL; //Make the list NULL
    struct upload_status upload_ctx; //Make an upload status struct

    mytext[0] = (char*)malloc(sizeof(date_buff) + 2); //First element is the time, get a space for it and for \r\n
    strcpy(mytext[0], date_buff); //Copy the date
    strcat(mytext[0], "\r\n"); //Put the ending sign to the end
    mytext[1] = (char*)malloc(sizeof(addressTo) + 2 + strlen("To: "));
    strcpy(mytext[1], "To: "); //Put before the email address to
    strcat(mytext[1], addressTo); //Put the email address
    strcat(mytext[1], "\r\n"); //Put the ending sign to the end
    mytext[2] = (char*)malloc(sizeof(UID) + 2 + strlen("From: "));
    strcpy(mytext[2], "From: "); //Put before the email address
    strcat(mytext[2], UID); //Put my email address
    strcat(mytext[2], "\r\n"); //Put the ending sign to the end
    mytext[3] = (char*)malloc(sizeof(subjectMail) + 2 + strlen("Subject: "));
    strcpy(mytext[3], "Subject: "); //Put the subject part
    strcat(mytext[3], subjectMail); //Put the subject part
    strcat(mytext[3], "\r\n"); //Put the ending sign to the end
    mytext[4] = (char*)malloc(2);
    strcpy(mytext[4], "\r\n"); //Put the ending sign to the end
    mytext[5] = (char*)malloc(sizeof(text) + 2);
    strcpy(mytext[5], text);
    strcat(mytext[5], "\r\n"); //Put the ending sign to the end
    mytext[6] = (char*)malloc(1); //Get a space for ending NULL pointer
    mytext[6] = NULL; //Put the ending NULL pointer the end of the header


    upload_ctx.lines_read = 0;

    //Curl example (modified)
    curl = curl_easy_init();
    if(curl) {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, UID/*"grosshausaufgabe2018@gmail.com"*/);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PWD/*"GHausaufgabe2018"*/);

        /* This is the URL for your mailserver. Note the use of port 587 here,
         * instead of the normal SMTP port (25). Port 587 is commonly used for
         * secure mail submission (see RFC4403), but you should use whatever
         * matches your server configuration. */
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");

        /* In this example, we'll start with a plain text connection, and upgrade
         * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
         * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
         * will continue anyway - see the security discussion in the libcurl
         * tutorial for more details. */
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);


        /* If your server doesn't have a valid certificate, then you can disable
         * part of the Transport Layer Security protection by setting the
         * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
         *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
         *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
         * That is, in general, a bad idea. It is still better than sending your
         * authentication details in plain text though.  Instead, you should get
         * the issuer certificate (or the host certificate if the certificate is
         * self-signed) and add it to the set of certificates that are known to
         * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
         * for more information. */
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");

        /* Note that this option isn't strictly required, omitting it will result
         * in libcurl sending the MAIL FROM command with empty sender data. All
         * autoresponses should have an empty reverse-path, and should be directed
         * to the address in the reverse-path which triggered them. Otherwise,
         * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
         * details.
         */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, UID);

        /* Add two recipients, in this particular case they correspond to the
         * To: and Cc: addressees in the header, but they could be any kind of
         * recipient. */
        recipients = curl_slist_append(recipients, addressTo);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* We're using a callback function to specify the payload (the headers and
         * body of the message). You could just use the CURLOPT_READDATA option to
         * specify a FILE pointer to read from. */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Since the traffic will be encrypted, it is very useful to turn on debug
         * information within libcurl to see what is happening during the transfer.
         */
        // curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, myfunct);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", //Todo export error in a file
                    curl_easy_strerror(res));

        /* Free the list of recipients */
        curl_slist_free_all(recipients);

        /* Always cleanup */
        curl_easy_cleanup(curl);
    }
    int i;
    for(i = 0; i < 7; ++i) free(mytext[i]); //Free my header

    return (int)res;
}

void connectToServer(int *statusFlag, int *exitFlag){
    char bufferForError[50]; //When error occured with curl I catch it here
    CURL *curl; //Make a curl
    CURLcode res = CURLE_OK; //Make curl state OK
    curl = curl_easy_init(); //Init curl
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, UID); //LogID
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PWD); //Log PWD

        //Set domain to google on 465 port, only ssl
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); //I dont want to see on stderr anything
        res = curl_easy_perform(curl);
        strcpy(bufferForError, curl_easy_strerror(res)); //Copy the backdata in the Buffer


        curl_easy_cleanup(curl); //Clean the curl, and delete
        //Error checking
        if (!strcmp(bufferForError, "No error")) {
            *statusFlag = 1; //Status will be: Logged
            *exitFlag = 0; //Exit this logining menu
        }
    }
}


