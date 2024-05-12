#include "file.h"
#include "input.h"
#include "account.h"
#include "operations.h"

int main(void)
{
    int id = 0;
    FILE *file_path;

    initialize_database(&file_path, &id);
    main_loop(file_path, id);
    fclose(file_path);
    return 0;
}
