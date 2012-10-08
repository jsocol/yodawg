#include <stdio.h>
#include "src/yodawg.h"

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
    if(yodawg_string_in_dawg(dawg, argv[1]))
        printf("The string %s is in the dawg.\n\n", argv[1]);
    else
        printf("The string %s is not in the dawg.\n\n", argv[1]);

    strings = yodawg_find_strings(dawg, argv[1]);
    if(strings->cursize < 0)
        printf("Prefix not in dawg.\n");
    else
        printf("Found %d matching strings:\n", strings->cursize);

    yodawg_free_dawg(dawg);
    
    for(j = 0; j < strings->cursize; j++)
        printf("%s\n", strings->words[j]);

    yodawg_free_wordlist(strings);

    return 0;
}
