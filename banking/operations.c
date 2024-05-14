#include "operations.h"
#include "account.h"
#include "input.h"
#include "search.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#define INTEREST 0.1
#define INTEREST_MORE 0.05
#define MAX_DEPOSIT 100000
#define MAX_WITHDRAW 100000
#define MAX_LOAN 100000
#define MAX_MONEY (INT32_MAX - MAX_DEPOSIT)


void print_operations(void)
{
    printf("Available operations:\n");
    printf("1. Create Account\n");
    printf("2. List All Accounts\n");
    printf("3. Search Account\n");
    printf("4. Deposit\n");
    printf("5. Withdraw\n");
    printf("6. Take Loan\n");
    printf("7. Pay Debt\n");
    printf("8. Transfer\n");
    printf("9. Exit\n");
}

void main_loop(FILE *file_path, int id)
{

    int choice;
    double money;
    int giver, receiver, acc;

    while (true)
    {
        print_operations();
        get_user_choice_input(&choice);
        switch (choice)
        {
        case 1:
            create_account(&id, file_path);
            printf("\n\n");
            break;
        case 2:
            list_all_accounts(file_path, id);
            break;
        case 3:
            print_search(file_path);
            break;
        case 4:
            acc = get_int_input("Enter your account number: ");
            money = get_double_input("Please enter how much money do you want to deposit: ");
            if (!get_confirmation())
            {
                printf("Operation canceled\n\n");
                break;
            }
            bool succesful_deposit = false;
            deposit(file_path, acc, money, &succesful_deposit);
            if (succesful_deposit)
            {
                printf("Succesful deposit of %.2lf PLN\n\n", money);
            }
            else
            {
                printf("Unsuccesful deposit of %.2lf PLN\n\n", money);
            }
            break;
        case 5:
            acc = get_int_input("Enter your account number: ");
            money = get_double_input("Please enter how much money do you want to withdraw: ");
            if (!get_confirmation())
            {
                printf("Operation canceled\n\n");
                break;
            }
            bool succesful_withdraw = false;
            withdraw(file_path, acc, money, &succesful_withdraw);
            if(succesful_withdraw)
            {
                printf("Succesful withdraw of %.2lf PLN\n\n", money);
            }
            else
            {
                printf("Unsuccesful withdraw of %.2lf PLN\n\n", money);
            }
            break;
        case 6:
            acc = get_int_input("Enter your account number: ");
            take_loan(file_path, acc);
            break;
        case 7:
            acc = get_int_input("Enter your account number: ");
            if (!get_confirmation())
            {
                printf("Operation canceled\n\n");
                break;
            }
            pay_debt(file_path, acc);
            break;
        case 8:
            giver = get_int_input("Enter giver account number: ");
            receiver = get_int_input("Enter reciever account number: ");
            money = get_double_input("Please enter how much money do you want to transfer: ");
            if (!get_confirmation())
            {
                printf("Operation canceled\n\n");
                break;
            }
            transfer(file_path, giver, receiver, money);
            break;
        case 9:
            printf("Exiting\n\n");
            return;

        default:
            printf("Invalid choice\n\n");
            break;
        }
    }
}

void deposit(FILE *file_path, int account_number, double money, bool *flag)
{
    Client client;
    bool found = false;
    
    if (money > MAX_DEPOSIT)
    {
        printf("Value exceeds maximum\n");
        return;
    }

    else if (money <= 0)
    {
        printf("Value must be positive\n");
        return;
    }

    if ((file_path = fopen("clients.dat", "r+b")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&client, sizeof(Client), 1, file_path))
    {
        if (client.id == account_number)
        {
            if(client.balance + money > MAX_MONEY)
            {
                printf("Value exceeds maximum account value\n");
                return;
            }
            client.balance += money;
            *flag = true;
            fseek(file_path, -sizeof(Client), SEEK_CUR);
            fwrite(&client, sizeof(Client), 1, file_path);
            found = true;
            break;
        }
    }

    if (!found)
    {
        printf("Client with account number %d not found.\n", account_number);
    }

    fclose(file_path);
}

void withdraw(FILE *file_path, int account_number, double money, bool *flag)
{
    Client client;
    bool found = false;

    if (money > MAX_WITHDRAW)
    {
        printf("Value exceeds maximum\n");
        return;
    }

    else if (money <= 0)
    {
        printf("Value must be positive\n");
        return;
    }

    if ((file_path = fopen("clients.dat", "r+b")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&client, sizeof(Client), 1, file_path))
    {
        if (client.id == account_number)
        {
            found = true;

            if (client.balance - money < 0)
            {
                printf("Not enough money in the account to perform this operation\n");
                break;
            }
            else
            {
                client.balance -= money;
                *flag = true;
                fseek(file_path, -sizeof(Client), SEEK_CUR);
                fwrite(&client, sizeof(Client), 1, file_path);
                break;
            }
        }
    }

    if (!found)
    {
        printf("Client with account number %d not found.\n", account_number);
    }

    fclose(file_path);
}

void transfer(FILE *file_path, int giver, int reciever, double money)
{
    bool succesful_withdraw = false;
    bool succesful_deposit = false;
    withdraw(file_path, giver, money, &succesful_withdraw);
    
    if (succesful_withdraw)
    {
        deposit(file_path, reciever, money, &succesful_deposit);
        if (succesful_deposit)
        {
            printf("Succesful transaction\n");
        }
    }
    else
    {
        printf("Unsuccesful transaction\n");
    }
}

void take_loan(FILE *file_path, int account_number)
{
    Client client;
    double loan = get_double_input("Enter amount of money that you want to borrow: ");
    printf("\n");

    if (loan > MAX_LOAN)
    {
        printf("Value exceeds maximum\n");
        return;
    }

    else if (loan <= 0)
    {
        printf("Value must be positive\n");
        return;
    }

    if (!get_confirmation())
    {
        printf("Operation canceled\n");
        return;
    }

    bool found = false;

    if ((file_path = fopen("clients.dat", "r+b")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&client, sizeof(Client), 1, file_path))
    {
        if (client.id == account_number)
        {
            if (client.balance + loan > MAX_MONEY)
            {
                printf("Value exceeds maximum account value\n");
                return;
            }
            found = true;
            client.balance += loan;
            if (client.interest_rate == 0)
            {
                client.interest_rate = INTEREST;
            }
            else if (client.interest_rate >= INTEREST)
            {
                client.interest_rate += INTEREST_MORE;
            }

            client.balance_loan += (1 + client.interest_rate) * loan;

            fseek(file_path, -sizeof(Client), SEEK_CUR);
            fwrite(&client, sizeof(Client), 1, file_path);
            break;
        }
    }
    bool succesful_deposit = false;
    deposit(file_path, account_number, loan, &succesful_deposit);

    if (succesful_deposit)
    {
        printf("Succesful operation\n");
    }
    else if(!succesful_deposit)
    {
        printf("Unsuccesful operation\n");
    }

    if (!found)
    {
        printf("Client with account number %d not found.\n", account_number);
    }

    fclose(file_path);
}

void pay_debt(FILE *file_path, int account_number)
{
    Client client;
    bool found = false;

    if ((file_path = fopen("clients.dat", "r+b")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&client, sizeof(Client), 1, file_path))
    {
        if (client.id == account_number)
        {
            found = true;

            if (client.balance - client.balance_loan < 0)
            {
                printf("Not enough money in the account to perform this operation\n");
                break;
            }
            else if (client.balance_loan == 0)
            {
                printf("You cannot perform this operation\n");
                break;
            }
            else
            {
                client.balance -= client.balance_loan;
                client.balance_loan = 0;
                client.interest_rate = 0;
                fseek(file_path, -sizeof(Client), SEEK_CUR);
                fwrite(&client, sizeof(Client), 1, file_path);
                break;
            }
        }
    }

    if (!found)
    {
        printf("Client with account number %d not found.\n", account_number);
    }

    fclose(file_path);
}

void print_search(FILE *file_path)
{
    int choice;
    char buffor[MAX_INPUT];
    printf("Search by: \n");
    printf("1 - account number\n");
    printf("2 - name\n");
    printf("3 - surname\n");
    printf("4 - address\n");
    printf("5 - pesel\n");

    choice = get_double_input("Enter your choice: ");

    switch (choice)
    {
    case 1:
        printf("Enter account number: ");
        int acc_num;
        scanf("%d", &acc_num);
        search_by_id(file_path, acc_num);
        break;
    case 2:
        get_string_input("Enter name: ", buffor, MAX_INPUT);
        search_by_name(file_path, buffor);
        break;
    case 3:
        get_string_input("Enter surname: ", buffor, MAX_INPUT);
        search_by_surname(file_path, buffor);
        break;
    case 4:
        get_string_input("Enter address: ", buffor, MAX_INPUT);
        search_by_address(file_path, buffor);
        break;
    case 5:
        get_string_input("Enter PESEL: ", buffor, MAX_INPUT);
        search_by_pesel(file_path, buffor);
        break;
    default:
        printf("Invalid choice\n");
        break;
    }
}
