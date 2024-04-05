/*
 *  This work is dedicated to the public domain.
 *  No rights reserved.
 *  http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "wordsplit.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define test_assert(X)                                                                  \
    do {                                                                                \
        if (!(X)) {                                                                     \
            fprintf(stderr, "assertion failed(%s,%d): %s\n", __FILE__, __LINE__, #X);   \
            exit(1);                                                                    \
        }                                                                               \
    } while (0)

#define COUNT_OF(array) (sizeof((array))/sizeof((array)[0]))
#define MAX_ARGV 128

static int argc;
static char *argv[MAX_ARGV];

static void parse(const char *string)
{
    argc = 0;
    char *copy = strdup(string);
    char *pos = copy;
    for (char *tok; (tok = wordsplit(&pos, " \t\n")) != NULL;)
    {
        assert(argc < MAX_ARGV);
        argv[argc] = strdup(tok);
        argc++;
    }
    free(copy);
}

static void single_char(void)
{
    parse("a");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "a") == 0);
}

static void single_char_opt(void)
{
    parse("-b");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "-b") == 0);
}

static void single_char_ifs(void)
{
    parse("  c  ");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "c") == 0);
}

static void multi_char(void)
{
    parse("fooBar");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "fooBar") == 0);
}

static void multi_char_opt(void)
{
    parse("--fooBar");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "--fooBar") == 0);
}

static void multi_char_ifs(void)
{
    parse("  fooBar  ");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "fooBar") == 0);
}

static void multi_word(void)
{
    parse("foo BAR Baz");
    test_assert(argc == 3);
    test_assert(strcmp(argv[0], "foo") == 0);
    test_assert(strcmp(argv[1], "BAR") == 0);
    test_assert(strcmp(argv[2], "Baz") == 0);
}

static void multi_word_ifs(void)
{
    parse("  foo  \t\n  12345  ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "foo") == 0);
    test_assert(strcmp(argv[1], "12345") == 0);
}

static void quotes(void)
{
    parse("  \"brown fox\"  ");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "brown fox") == 0);
}

static void quotes_merged(void)
{
    parse("  --quick=\"brown fox\"  ");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "--quick=brown fox") == 0);
}

static void quotes_merged_nested(void)
{
    parse("  a\"b\"c  z ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "abc") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void quotes_merged_multiple(void)
{
    parse("  a\"b\"c'd'e  z ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "abcde") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void quotes_merged_spaced(void)
{
    parse(" a'bc xy'z ");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "abc xyz") == 0);
}

static void quotes_mixed(void)
{
    parse("  a\"b'c\"d  z");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "ab'cd") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void quotes_escaped(void)
{
    parse("  a\"b\\\"c\"d  z  ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "ab\"cd") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void quotes_escaped_single(void)
{
    parse("  a'b\\'c'd  z  ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "ab'cd") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void quotes_unclosed(void)
{
    parse("  a\"b  c ");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "ab  c ") == 0);
}

static void escaped_chars(void)
{
    parse("  \\a  \\z  ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "a") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void escaped_quotes(void)
{
    parse(" \\\"abc z ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "\"abc") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void escaped_quotes_single(void)
{
    parse(" a\\'bc z ");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "a'bc") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void escaped_ifs(void)
{
    parse("a\\ b\\\tc\\\n z");
    test_assert(argc == 2);
    test_assert(strcmp(argv[0], "a b\tc\n") == 0);
    test_assert(strcmp(argv[1], "z") == 0);
}

static void escaped_none(void)
{
    parse("a\\");
    test_assert(argc == 1);
    test_assert(strcmp(argv[0], "a") == 0);
}

static void integration(void)
{
    parse(" The --quick=\"brown fox\" jumps\"o\"ver the 'lazy d'og. ");
    test_assert(argc == 5);
    test_assert(strcmp(argv[0], "The") == 0);
    test_assert(strcmp(argv[1], "--quick=brown fox") == 0);
    test_assert(strcmp(argv[2], "jumpsover") == 0);
    test_assert(strcmp(argv[3], "the") == 0);
    test_assert(strcmp(argv[4], "lazy dog.") == 0);
}

static void integration_null(void)
{
    test_assert(wordsplit(NULL, " \t\n") == NULL);
}

int main(int n, char *v[])
{
    typedef void (*test_fn)(void);

    static const test_fn tests[] = {
        single_char,
        single_char_opt,
        single_char_ifs,

        multi_char,
        multi_char_opt,
        multi_char_ifs,

        multi_word,
        multi_word_ifs,

        quotes,
        quotes_merged,
        quotes_merged_nested,
        quotes_merged_multiple,
        quotes_merged_spaced,
        quotes_mixed,
        quotes_escaped,
        quotes_escaped_single,
        quotes_unclosed,

        escaped_chars,
        escaped_quotes,
        escaped_quotes_single,
        escaped_ifs,
        escaped_none,

        integration,
        integration_null,
    };

    for (int i = 0; i < COUNT_OF(tests); i++)
    {
        // Run the unit test.
        tests[i]();

        // Cleanup after ourselves.
        for (int i = 0; i < argc; i++)
        {
            free(argv[i]);
            argv[i] = NULL;
        }
        argc = 0;
    }

    return 0;
}
