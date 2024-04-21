#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_line();


int main(int argc, char const *argv[])
{   
    
    return 0;
}

char *allocate_line(char *line, int size, int c)
{
    char *tmp = realloc(line, (size + 1) * sizeof(char));

    if (!tmp)
    {
        free(line);
        return NULL;
    }
    else
    {
        line = tmp;
        line[size] = (char)c;
    }
    return line;
}

char *get_line()
{
    char *line = NULL;
    int c;
    int size = 0;

    if ((c = getchar()) == EOF)
    {
        return NULL;
    }

    while((c = getchar()) != EOF)
    {
        
        if (!(line = allocate_line(line, size, c)))
        {
            return NULL;
        }
         
        size++;    
    
    }

    line[size] = '\0';
    return line;
}
