/*
 *  This work is dedicated to the public domain.
 *  No rights reserved.
 *  http://creativecommons.org/publicdomain/zero/1.0/
 */

#pragma once

/*
 *  This function implements the word splitting algorithm as performed
 *  by *nix shells. It does not perform shell expansion.
 *
 *  When invoked this function extracts the next word starting from
 *  the string 'sp' and returns it as a null-terminated string.
 *  If there are no more words, then the function returns NULL.
 *
 *  The implementation will set 'sp' to point after the most
 *  recently scanned word. It will also modify the memory
 *  pointed to by 'sp' to add a null terminator much
 *  like strtok().
 *
 *  The 'ifs' paramter specifies the input field separators.
 *  This is a string of characters to delimit words on. By default
 *  the input field seperators used by *nix shells are space,
 *  tab, and new line (i.e. " \t\n").
 * 
 *  If 'sp' is NULL then NULL is returned an no action is performed.
 *  If 'ifs' is NULL then the behavior is undefined.
 */
char *wordsplit(char **sp, const char *ifs);
