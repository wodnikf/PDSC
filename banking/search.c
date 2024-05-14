#include "search.h"
#include "account.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void search_by_id(FILE *file_path, int value)
{
    Client to_find;
    if ((file_path = fopen("clients.dat", "r")) == NULL)
    {

                perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&to_find, sizeof(Client), 1, file_path))
    {
        if (to_find.id == value)
        {
            print_account(&to_find);
        }
    }
    fclose(file_path);
}

void search_by_name(FILE *file_path, char *value)
{
    Client to_find;
    if ((file_path = fopen("clients.dat", "r")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&to_find, sizeof(Client), 1, file_path))
    {
        if (!strcmp(to_find.name, value))
        {
            print_account(&to_find);
        }
    }
    fclose(file_path);
}

void search_by_surname(FILE *file_path, char *value)
{
    Client to_find;
    if ((file_path = fopen("clients.dat", "r")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&to_find, sizeof(Client), 1, file_path))
    {
        if (!strcmp(to_find.surname, value))
        {
            print_account(&to_find);
        }
    }
    fclose(file_path);
}

void search_by_pesel(FILE *file_path, char *value)
{
    Client to_find;
    if ((file_path = fopen("clients.dat", "r")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&to_find, sizeof(Client), 1, file_path))
    {
        if (!strcmp(to_find.pesel, value))
        {
            print_account(&to_find);
        }
    }
    fclose(file_path);
}

void search_by_address(FILE *file_path, char *value)
{
    Client to_find;
    if ((file_path = fopen("clients.dat", "r")) == NULL)
    {
        perror("Error opening clients.dat");
        exit(1);
    }

    while (fread(&to_find, sizeof(Client), 1, file_path))
    {
        if (!strcmp(to_find.address, value))
        {
            print_account(&to_find);
        }
    }
    fclose(file_path);
}
