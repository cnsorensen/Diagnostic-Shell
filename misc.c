// misc.c

#include "header.h"

// Takes a string and converts it to all lowercase
// Params: string token to be converted
// Return: string of the token lowercased
char* toLowerCase(char* token)
{
    int i = 0;
    char c;

    // go through each letter in the string
    while(token[i])
    {
        c = token[i];
        token[i] = tolower(c);
        i++;
    }

    // return the newly lower-cased token
    return token;
}

// Checks to see if a character is an int by comparing the ascii values
// is between 48 (0) and 57 (9). Also checks for negative number
// Param: char* num - character string to be check if an integer
// Return: -1 - not an int, 1 - it's an int
int isInt(char* num)
{
    int i = 0;

    // if it's a negative number, ignore the - sign
    if(num[0] == 45)
    {
        i = 1;
    }

    // go through each decimal in the number passed in
    while(num[i])
    {
        // if it's in the boundaries of an int
        if(num[i] < 48 || num[i] > 57)
        {
            return -1;
        }
        
        i++;
    }

    // else, it's an int
    return 1;
}

char* removeNewLine(char* line)
{
    size_t len = strlen(line);
    
    if(len > 0 && line[len-1] == '\n')
    {
        line[--len] = '\0';
    }

    return line;
}
