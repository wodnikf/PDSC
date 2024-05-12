#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdio.h>

#define MAX_INPUT 128

typedef struct Client
{
    int id;
    char name[MAX_INPUT];
    char surname[MAX_INPUT];
    char address[MAX_INPUT];
    char pesel[MAX_INPUT];
    double balance;
    double balance_loan;
    double interest_rate;
} Client;

void create_account(int *id, FILE *file_path);
void print_account(Client *client);
void list_all_accounts(FILE *file_path, int id);
void count_active_accounts(FILE *file_path, int *active_accounts);

#endif /* ACCOUNT_H */
