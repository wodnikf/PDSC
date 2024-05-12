#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdbool.h>

void print_operations(void);
void main_loop(FILE *file_path, int id);
void deposit(FILE *file_path, int account_number, double money, bool *flag);
void withdraw(FILE *file_path, int account_number, double money, bool *flag);
void transfer(FILE *file_path, int giver, int reciever, double money);
void take_loan(FILE *file_path, int account_number);
void pay_debt(FILE *file_path, int account_number);
void print_search(FILE *file_path);

#endif /* OPERATIONS_H */
