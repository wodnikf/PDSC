#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

bool does_file_exist(char *path);
void count_active_accounts(FILE *file_path, int *active_accounts);
void initialize_database(FILE **file_path, int *id);

#endif /* FILE_H */
