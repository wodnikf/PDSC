#include "account.h"
#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void create_account(int *id, FILE *file_path)
{
    Client new_client;

    new_client.id = ++(*id);

    get_names("Enter name: ", new_client.name);
    get_names("Enter surname: ", new_client.surname);
    get_string_input("Enter address: ", new_client.address, MAX_INPUT);
    get_pesel(&new_client);

    new_client.balance = 0.0;
    new_client.balance_loan = 0.0;
    new_client.interest_rate = 0.0;

    if (!get_confirmation())
    {
        printf("Account creation canceled.\n");
        (*id)--;
        return;
    }

    if ((file_path = fopen("clients.dat", "r+b")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    fseek(file_path, 0L, SEEK_END);

    if (fwrite(&new_client, sizeof(Client), 1, file_path) != 1)
    {
        perror("Error during writing file");
        exit(3);
    }

    printf("Account succesfully created!\n");

    fclose(file_path);
}

void print_account(Client *client)
{
    printf("Account Number: %d\n", client->id);
    printf("First name: %s\n", client->name);
    printf("Surname: %s\n", client->surname);
    printf("Address: %s\n", client->address);
    printf("Pesel: %s\n", client->pesel);
    printf("Balance: %.2lf PLN\n", client->balance);
    printf("Loan: %.2lf PLN\n", client->balance_loan);
    printf("Interest rate: %.2lf %%\n", client->interest_rate);
}

void list_all_accounts(FILE *file_path, int id)
{

    if (id < 1)
    {
        printf("Unable to print accounts as none are yet saved\n");
    }
    else
    {
        Client to_print;
        if ((file_path = fopen("clients.dat", "r+b")) == NULL)
        {
            perror("Error opening clients.dat");
            exit(1);
        }
        for (int i = 0; i < id; i++)
        {
            if (fread(&to_print, sizeof(Client), 1, file_path) != 1)
            {
                perror("Error reading from clients.dat");
                exit(2);
            }
            print_account(&to_print);
        }
        fclose(file_path);
    }
}

void count_active_accounts(FILE *file_path, int *active_accounts)
{
    Client temp_client;
    *active_accounts = 0;

    rewind(file_path);
    while (fread(&temp_client, sizeof(Client), 1, file_path) == 1)
    {
        if (temp_client.id != 0)
        {
            (*active_accounts)++;
        }
    }
}
