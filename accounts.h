#ifndef GHA2018_ACCOUNTS_H
#define GHA2018_ACCOUNTS_H
typedef struct accountMember {
    char name[30];
    char emailAddress[100];
    struct accountMember *next;
} accountMember;

void copyName(char *dest, char *source, int *currentCountOfSource);

void copyEmail(char *dest, char *source, int *currentCountOfSource);

int addItemToList(accountMember **head, char *bufname, char *bufemail);

accountMember * makingLinkedListFromAccounts();

int writeOutAccount(accountMember *head);

char* readInAccount();

int addAccount();

int deleteAccount();

int listAccount();

void freeList(accountMember *head);

#endif //GHA2018_ACCOUNTS_H
