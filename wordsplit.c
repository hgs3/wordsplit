/*
 *  This work is dedicated to the public domain.
 *  No rights reserved.
 *  http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "wordsplit.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>

char *wordsplit(char **stringp, const char *ifs)
{
    if (stringp == NULL)
    {
        return NULL;
    }
    assert(ifs != NULL);

    // Skip past all input field separators (IFS).
    char *src = *stringp;
    for (;; src++)
    {
        if (*src == '\0')
        {
            *stringp = src;
            return NULL;
        }
        else if (strchr(ifs, *src) == NULL)
        {
            break;
        }
    }

    // Parse the current word.
    char quote = 0x0;
    char *tok = src;
    char *dst = src;
    while (*src)
    {
        // If an input field separator is found, then terminate the loop.
        if ((quote == 0x0) && (strchr(ifs, *src) != NULL))
        {
            src++;
            break;
        }

        // Open or close a quoted portion of the input.
        if ((*src == '"') || (*src == '\''))
        {
            if (quote == 0x0) // Open quoted.
            {
                quote = *src++;
                continue;
            }

            if (quote == *src) // Close quoted.
            {
                quote = 0x0;
                src++;
                continue;
            }
        }

        // Always consume the next character after a backslash unless it's a null terminator.
        // This allows escaping double and single quotes.
        if (*src == '\\')
        {
            src++;
            if (!*src)
            {
                break;
            }
        }

        *dst++ = *src++;
    }

    *stringp = src;
    *dst = '\0';
    return tok;
}
