#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool fail = false;

char *read_line(bool *fail)
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
                *fail = true;
                return NULL;
            }

            newbuf = realloc(line, bufsize);

            if (!newbuf)
            {
                free(line);
                *fail = true;
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

    // file is empty
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
            *fail = true;
            return NULL;
        }
        newbuf = realloc(line, bufsize);
        if (!newbuf)
        {
            free(line);
            *fail = true;
            return NULL;
        }
        line = newbuf;
    }

    // end of line
    line[length++] = '\0';

    return line;
}

void reverse_words(char *line)
{
}

void reverse_lines()
{
    char **lines = NULL;
    int number_of_lines = 0;
    char *line;
    while ((line = read_line(&fail)))
    {
        char **new_lines = realloc(lines, (number_of_lines + 1) * sizeof(char *));
        if (!new_lines)
        {
            for (int i = 0; i < number_of_lines; i++)
            {
                free(lines[i]);
            }
            free(lines);
            exit(1);
        }
        lines = new_lines;
        lines[number_of_lines++] = line;
    }
    for (int i = number_of_lines; i > 0; i--)
    {
        printf("%s", lines[i - 1]);
        free(lines[i - 1]);
    }
    free(lines);
}
int main()
{
    reverse_lines(NULL); // Call the function with NULL argument as it doesn't use it.
    return 0;
}