#include "input.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_INPUT 128

void get_string_input(char *prompt, char *input, int maxLength)
{
    printf("%s", prompt);
    fgets(input, maxLength, stdin);
    if (input[strlen(input) - 1] == '\n')
    {
        input[strlen(input) - 1] = '\0';
    }
}

int get_int_input(char *prompt)
{
    int num;
    printf("%s", prompt);
    while (scanf("%d", &num) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("Invalid input. Please enter an integer: ");
    }
    while (getchar() != '\n')
        ;
    return num;
}

double get_double_input(char *prompt)
{
    double num;
    printf("%s", prompt);
    while (scanf("%lf", &num) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("Invalid input. Please enter an integer: ");
    }
    while (getchar() != '\n')
        ;
    return num;
}

bool get_confirmation(void)
{
    char c;
    printf("Do you confirm changes? (y/n)\n");

    do
    {
        scanf("%c", &c);

        c = tolower(c);

        if (c != 'y' && c != 'n')
        {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
            while (getchar() != '\n')
                ;
        }

    } while (c != 'y' && c != 'n');

    if (c == 'y')
    {
        return true;
    }

    return false;
}

void get_names(char *prompt, char *input)
{
    int check;
    do
    {
        check = 0;
        get_string_input(prompt, input, MAX_INPUT);

        for (int i = 0; i < strlen(input); i++)
        {
            if (isdigit(input[i]))
            {
                check = 1;
            }
        }
        if (check == 1)
        {
            printf("Wrong input.\n Please don't use digits!\n");
        }

    } while (check == 1);
}

void get_pesel(Client *client)
{
    char buffor[MAX_INPUT];

    do
    {
        get_string_input("Enter PESEL: ", buffor, MAX_INPUT);

        if (!validate_pesel(buffor))
        {
            printf("Wrong pesel\n");
        }
    } while (!validate_pesel(buffor));

    strcpy(client->pesel, buffor);
}

bool validate_pesel(const char *buffor)
{
    int pesel_length = 11;
    if (strlen(buffor) != pesel_length)
    {
        return false;
    }

    for (int i = 0; i < pesel_length; i++)
    {
        if (!isdigit(buffor[i]))
        {
            return false;
        }
    }
    return true;
}

void get_user_choice_input(int *choice)
{
    do
    {
        *choice = get_int_input("Please enter your choice: ");
        printf("%d\n", *choice);
    } while (*choice < 1 || *choice > 9);
}
