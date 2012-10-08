#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef _YODAWG
#define _YODAWG

#ifndef YO_STARTSIZE
#define YO_STARTSIZE 16
#endif

#ifndef YO_EOW
#define YO_EOW -1
#endif

struct yonode {
    char value;
    int cursize, maxsize;
    struct yonode **edges;
};

struct yowordlist {
    int cursize, maxsize;
    char **words;
};

struct yonode *yodawg_create();
struct yonode *yodawg_create_node(char);
int yodawg_add_node(struct yonode *, struct yonode *);
int yodawg_value_in_node(struct yonode *, char);
int yodawg_add_string(struct yonode *, char *);
void yodawg_free_node(struct yonode *);
struct yonode **yodawg_add_dawg_to_list(struct yonode **, int *, int *, struct yonode *);
int yodawg_free_dawg(struct yonode *);
int yodawg_build_strings(struct yonode *, const char *, struct yowordlist *);
struct yowordlist *yodawg_create_wordlist(int);
void yodawg_free_wordlist(struct yowordlist *);
struct yowordlist *yodawg_find_strings(struct yonode *, char *);

#endif
