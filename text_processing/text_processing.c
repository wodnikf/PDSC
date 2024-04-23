#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rand_malloc.h"

char *read_line()
{
    char *line = NULL;
    int c;
    size_t bufsize = 0;
    size_t length = 0;
    while ((c = getchar()) != EOF)
    {

        if (length >= bufsize)
        {
            char *newbuf;
            if (bufsize == 0)
            {
                bufsize = 2;
            }
            else if (bufsize <= ((size_t)-1) / 2)
            {
                bufsize = 2 * length;
            }
            else
            {
                free(line);

                return NULL;
            }

            newbuf = realloc(line, bufsize);

            if (!newbuf)
            {
                free(line);
                return NULL;
            }
            
            line = newbuf;
        }

        line[length++] = c;

        if (c == '\n')
        {
            break;
        }
    }

    if ((c == EOF) && (length == 0))
    {
        free(line);
        return NULL;
    }

    if (length >= bufsize)
    {
        char *newbuf;
        if (length < (size_t)-1)
            bufsize = length + 1;
        else
        {
            free(line);

            return NULL;
        }
        newbuf = realloc(line, bufsize);
        if (!newbuf)
        {
            free(line);

            return NULL;
        }
        line = newbuf;
    }

    line[length++] = '\0';

    return line;
}

void reverse_words(char *line)
{
    char *start = line;
    char *end = strchr(line, '\0') - 1;

    while (start < end)
    {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }

    start = line;
    while (*start != '\0')
    {
        while (*start == ' ' || *start == '\n')
        {
            start++;
        }
        char *word_end = start;
        while (*word_end != ' ' && *word_end != '\n' && *word_end != '\0')
        {
            word_end++;
        }
        end = word_end - 1;

        while (start < end)
        {
            char tmp = *start;
            *start = *end;
            *end = tmp;
            start++;
            end--;
        }

        start = word_end;
    }
}

void reverse_lines()
{
    char **lines = NULL;
    int number_of_lines = 0;
    char *line;
    while ((line = read_line()))
    {
        char **new_lines = realloc(lines, (number_of_lines + 1) * sizeof(char *));
        if (!new_lines)
        {
            for (int i = 0; i < number_of_lines; i++)
            {
                free(lines[i]);
            }
            free(line);
            free(lines);
            exit(1);
        }
        lines = new_lines;
        reverse_words(line);
        lines[number_of_lines++] = line;
    }
    for (int i = number_of_lines; i > 0; i--)
    {
        printf("%s", lines[i - 1]);
        free(lines[i - 1]);
    }
    free(lines);
    printf("\n");
}

int main()
{
    reverse_lines();
    return 0;
}