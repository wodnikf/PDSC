#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdio.h>
#include "account.h"

void get_string_input(char *prompt, char *input, int maxLength);
int get_int_input(char *prompt);
double get_double_input(char *prompt);
bool get_confirmation(void);
void get_names(char *prompt, char *input);
void get_pesel(Client *client);
bool validate_pesel(const char *buffor);
void get_user_choice_input(int *choice);

#endif /* INPUT_H */
