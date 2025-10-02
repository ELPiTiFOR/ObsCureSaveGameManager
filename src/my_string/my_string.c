#include "my_string.h"

#include <stdlib.h>
#include <string.h>

char *my_strchrnul(char *str, int c)
{
   while(*str && *str != c)
   {
      str++;
   }

   return str;
}

char *duplicate_string(char *str)
{
    size_t len = strlen(str);
    char *res = calloc(len + 1, 1);
    if (!res)
    {
        return NULL;
    }

    strcpy(res, str);
    return res;
}

char *duplicate_string_until(char *str, int c, size_t *index)
{
    char *mark = my_strchrnul(str, c);
    if (!mark)
    {
        /*
        char buf_log[512];
        sprintf(buf_log, "Character %c not found in %s\n", c, str);
        log(ERROR, buf_log);
        */
        return NULL;
    }
    if (*mark == '\n' && *(mark - 1) == '\r')
    {
        *(mark - 1) = 0;
    }

    *mark = 0;
    char *key = duplicate_string(str);
    if (!key)
    {
        *mark = c;
        return NULL;
    }

    if (index)
    {
        *index += mark - str;
    }

    if (c == '\n')
    {
        *(mark - 1) = '\r';
    }
    *mark = c;
    return key;
}