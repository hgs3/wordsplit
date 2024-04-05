# Wordsplit

Perform word splitting as done by *nix shells.

[![Build Status](https://github.com/hgs3/wordsplit/actions/workflows/ci.yml/badge.svg)](https://github.com/hgs3/wordsplit/actions/workflows/ci.yml)

## What is "Word Splitting?"

Word splitting is the process of tokenizing a shell command (string) into words (tokens).
For example, the command `mv foo "bar baz"` is comprised of three words: `<mv>`, `<foo>`, and `<bar baz>`.
This library implements word splitting as a single function that behaves much like `strsep` and `strtok`.
See [wordsplit.h](wordsplit.h) for details.

## Installation

Copy `wordsplit.c` and `wordsplit.h` into your C project.
The implementation only depends upon libc.

## Usage

This example splits words on the space, tab, and new line characters.

```c
#include "wordsplit.h"

char *s = strdup("The --quick=\"brown fox\" jumps\"o\"ver the 'lazy d'og.");
char *word, *sp = s;
while ((word = wordsplit(&sp, " \t\n")) != NULL)
{
    puts(word);
}
free(s);
```

The above code would print:

```
The
--quick=brown fox
jumpsover
the
lazy dog.
```

## Prior Art

POSIX defines the [wordexp](https://man7.org/linux/man-pages/man3/wordexp.3.html) function which performs word splitting, but it also performs shell expansion, e.g. it will expand environment variables like $PATH.
Shell expansion may not be desirable depending on your needs.

This implementation of word splitting does not perform shell expansion, only tokenization.

## Security Notes

The implementation is unit tested, fuzz tested, and sanitized via UBSan, ASan, and MSan.

## License

This work is dedicated to the public domain.
