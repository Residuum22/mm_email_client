#include "accounts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "AccountList.txt"



int writeOutAccount(accountMember *head) {
    FILE *output;
    unsigned len;
    int i;
    output = fopen(FILE_NAME, "w");
    if(output == NULL){
        fclose(output);
        return 2;
    }
    accountMember *tmp;
    for(tmp = head; tmp != NULL; tmp = tmp->next){
        len = (unsigned)strlen(tmp->name);
        fputs("Name: ", output);
        for (i = 0; i < len; ++i){
            putc(tmp->name[i], output);
        }
        putc('\n', output);
        len = (unsigned)strlen(tmp->emailAddress);
        fputs("E-mail: ", output);
        for (i = 0; i < len; ++i){
            putc(tmp->emailAddress[i], output);
        }
        putc('\n', output);
        putc('\n', output);
    }
    putc('#', output);
    fclose(output);
    return 0;
}

//Working the reading
char *readInAccount() {
    int scan;
    unsigned memorySize = 100;
    char *buffer = (char *) malloc(memorySize);
    if (buffer == NULL) {
        free(buffer);
        return NULL;
    }
    char *temp;
    int counter = 0;
    FILE *input;
    input = fopen(FILE_NAME, "r");
    if (input == NULL) {
        fclose(input);
        return NULL;
    }
    while ((scan = getc(input)) != EOF) {
        if (counter < memorySize)
            buffer[counter++] = (char) scan;
        else {
            memorySize += 100;
            if ((temp = realloc(buffer, memorySize)) == NULL) {
                free(temp);
                free(buffer);
                return NULL;
            }
            buffer = temp;
            buffer[counter++] = (char) scan;
        }
    }
    fclose(input);
    return buffer;
}


int deleteAccount(int number, accountMember **head) {
    if (*head == NULL) return 0;
    accountMember *temp, *tempk;
    int i;
    if (number == 0){
        temp = *head;
        *head = (*head)->next;
        free(temp);
        return 0;
    }
    for(i = 0, temp = *head; i <= number && temp != NULL; tempk = temp, temp = temp->next, ++i){
        if (number == i){
            (tempk->next) = (temp->next);
            free(temp);
            return 0;
        }
    }
}


accountMember *makingLinkedListFromAccounts() {
    accountMember *list = NULL;
    char *work;
    char bufferName[30];
    char bufferEmail[100];
    int len = 0;
    work = readInAccount();
    if (work == NULL) {
        free(work);
        return NULL;
    }
    for (len = 0; work[len] != '#'; ++len) {
        copyName(bufferName, work, &len);
        copyEmail(bufferEmail, work, &len);
        addItemToList(&list, bufferName, bufferEmail);
    }
    return list;
}

int addItemToList(accountMember **head, char *bufname, char *bufemail) {
    accountMember *temp;
    if (*head == NULL) {
        *head = (accountMember *) malloc(sizeof(accountMember));
        strcpy((*head)->name, bufname);
        strcpy((*head)->emailAddress, bufemail);
        (*head)->next = NULL;
        return 0;
    }
    int i;
    for (temp = *head, i = 0; temp->next != NULL; temp = temp->next, ++i) if (i == 10) return 1;
    temp->next = (accountMember *) malloc(sizeof(accountMember));
    strcpy((temp->next)->name, bufname);
    strcpy((temp->next)->emailAddress, bufemail);
    (temp->next)->next = NULL;
    return 0;
}

void copyName(char *dest, char *source, int *currentCountOfSource) {
    int i;
    *currentCountOfSource += 6;
    for (i = 0; source[*currentCountOfSource] != '\n'; ++(*currentCountOfSource), ++i)
        dest[i] = source[*currentCountOfSource];
    dest[i] = '\0';
    ++(*currentCountOfSource);
}

void copyEmail(char *dest, char *source, int *currentCountOfSource) {
    int i;
    *currentCountOfSource += 8;
    for (i = 0; source[*currentCountOfSource] != '\n'; ++(*currentCountOfSource), ++i)
        dest[i] = source[*currentCountOfSource];
    dest[i] = '\0';
    ++(*currentCountOfSource);
}

void freeList(accountMember *head) {
    accountMember *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}