#include "accounts.h"
#include <stdio.h>

#define FILE_NAME "AccountList.txt"
typedef struct accountMember {
    char name[30];
    char emailAddress[100];
    struct accountMember *next;
} accountMember;

int writeOutAccount() {
    char buffer[100] = "Szia#";
    int i;
    FILE *output;
    output = fopen(FILE_NAME, "w");
    for (i = 0; buffer[i] != '#'; ++i) putc(buffer[i], output);
    fclose(output);
    return 0;
}

int readInAccount() {

}

int addAccount() {

}

int deleteAccount() {

}

int listAccount() {

}

int readyAccounts() {
    char buffer[1000];
    FILE *input;
    int scan;
    int counter = 0;
    input = fopen(FILE_NAME, "r");
    if (input == NULL) {
        fclose(input);
        0;
    }
    while (((scan = getc(input)) != EOF) && counter < 1000) {
        buffer[counter++] = (char) scan;
    }

    return 0;
}
