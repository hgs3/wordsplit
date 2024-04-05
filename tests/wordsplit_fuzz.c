/*
 *  This work is dedicated to the public domain.
 *  No rights reserved.
 *  http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "wordsplit.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    char *copy = malloc(size + 1);
    memcpy(copy, data, size);
    copy[size] = '\0';
    
    char *it = copy;
    while (wordsplit(&it, " \t\n") != NULL)
    {
        // Continue iterating until there are no more words to process.
    }

    free(copy);
    return 0;
}
