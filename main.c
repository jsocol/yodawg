#include <stdio.h>
#include "yodawg.h"

int main(int argc, char **argv)
{
    struct yonode *dawg; 
    int j;
    struct yowordlist *strings;

    if (argc < 2) return -1;

    dawg = yodawg_create();

    yodawg_add_string(dawg, "foo0");
    yodawg_add_string(dawg, "bar");
    yodawg_add_string(dawg, "baz");
    yodawg_add_string(dawg, "bark");
    yodawg_add_string(dawg, "bars");
    yodawg_add_string(dawg, "barf");
    yodawg_add_string(dawg, "quxxxx");
    yodawg_add_string(dawg, "bbq");

    // Booyahkahsha.
    strings = yodawg_find_strings(dawg, argv[1]);
    if(strings->cursize < 0) {
        printf("prefix not in dawg.\n");
    }
    else {
        printf("found %d matching strings.\n", strings->cursize);
    }

    yodawg_free_dawg(dawg);
    printf("Freed dawg.\n");
    
    for(j = 0; j < strings->cursize; j++) {
        printf("string: %s\n", strings->words[j]);
    }

    yodawg_free_wordlist(strings);

    return 0;
}
