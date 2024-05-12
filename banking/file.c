#include "file.h"
#include "account.h"
#include <stdlib.h>

bool does_file_exist(char *path)
{
    if (access(path, F_OK) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void initialize_database(FILE **file_path, int *id)
{
    if (!does_file_exist("clients.dat"))
    {
        if ((*file_path = fopen("clients.dat", "w+b")) == NULL)
        {
            perror("Error opening clients.dat");
            exit(1);
        }
    }
    else
    {
        if ((*file_path = fopen("clients.dat", "r+b")) == NULL)
        {
            perror("Error opening clients.dat");
            exit(1);
        }

        count_active_accounts(*file_path, id);
    }
    fclose(*file_path);
}
