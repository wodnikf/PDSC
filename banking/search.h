#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>

void search_by_id(FILE *file_path, int value);
void search_by_name(FILE *file_path, char *value);
void search_by_surname(FILE *file_path, char *value);
void search_by_pesel(FILE *file_path, char *value);
void search_by_address(FILE *file_path, char *value);

#endif /* SEARCH_H */
