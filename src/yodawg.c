#include "yodawg.h"

struct yonode *yodawg_create_node(char value)
{
    struct yonode *node;
    node = malloc(sizeof (struct yonode));
    node->cursize = 0;
    node->maxsize = YO_STARTSIZE;
    node->value = value;
    node->edges = malloc(YO_STARTSIZE * (sizeof (struct yonode *)));
    return node;
}

struct yonode *yodawg_create()
{
    struct yonode *start, *eow;
    start = yodawg_create_node(0);
    eow = yodawg_create_node(YO_EOW);
    yodawg_add_node(start, eow);
    return start;
}

void yodawg_add_node(struct yonode *parent, struct yonode *child)
{
    if (parent->edges == NULL) {
        parent->edges = malloc(YO_STARTSIZE * (sizeof (struct yonode *)));
    }
    else if (parent->cursize == parent->maxsize) {
        parent->maxsize *= 2;
        parent->edges = realloc(parent->edges, parent->maxsize);
    }
    parent->edges[parent->cursize++] = child;
}

int yodawg_value_in_dawg(struct yonode *parent, char value)
{
    int i;
    for(i = 0; i < parent->cursize; i++)
        if (value == parent->edges[i]->value)
            return i;
    return -1;
}

void yodawg_add_string(struct yonode *dawg, char *str)
{
    char c;
    struct yonode *cur, *buf;
    int i;

    cur = dawg;
    while((c = *str++) != '\0') {
        i = yodawg_value_in_dawg(cur, c);
        if(i < 0) {
            buf = yodawg_create_node(c);
            yodawg_add_node(cur, buf);
            cur = buf;
        }
        else {
            cur = cur->edges[i];
        }
    }
    if(yodawg_value_in_dawg(cur, YO_EOW) < 0) {
        yodawg_add_node(cur, dawg->edges[0]);
    }
}

void yodawg_free_node(struct yonode *node)
{
    if(node == NULL) {
        return;
    }
    free(node->edges);
    free(node);
}

struct yonode **yodawg_add_dawg_to_list(struct yonode **nodes, int *cursize, int *maxsize, struct yonode *dawg)
{
    int i, j, found;
    struct yonode *cur;

    for(i = 0; i < dawg->cursize; i++) {
        cur = dawg->edges[i];
        j = 0;
        found = 0;
        while(j < (*cursize)) {
            if(nodes[j] == cur) {
                found = 1;
                break;
            }
            j++;
        }
        if(!found) {
            while((*cursize) >= (*maxsize)) {
                (*maxsize) *= 2;
                nodes = realloc(nodes, (*maxsize) * sizeof (struct yonode *));
            }
            nodes[*cursize] = cur;
            ++*cursize;
        }
        if(cur->cursize > 0)
            nodes = yodawg_add_dawg_to_list(nodes, cursize, maxsize, cur);
    }
    return nodes;
}

void yodawg_free_dawg(struct yonode *dawg)
{
    int *cursize;
    int *maxsize;
    int _cursize = 0;
    int _maxsize = YO_STARTSIZE;
    int i;
    struct yonode **nodes;

    cursize = &_cursize;
    maxsize = &_maxsize;
    nodes = malloc((*maxsize) * sizeof (struct yonode *));

    nodes = yodawg_add_dawg_to_list(nodes, cursize, maxsize, dawg);

    // Free the children.
    for(i = 0; i < *cursize; i++) {
        yodawg_free_node(nodes[i]);
    }
    // Free the root.
    yodawg_free_node(dawg);

    free(nodes);
}

void yodawg_build_strings(struct yonode *dawg, const char *prefix, struct yowordlist *strings)
{
    struct yonode *cur;
    int i;
    int plen = strlen(prefix);
    char *newprefix;
    newprefix = malloc(plen + 2);

    for(i = 0; i < dawg->cursize; i++) {
        cur = dawg->edges[i];
        if(cur->value == YO_EOW) {
            // Allocate enough memory.
            if(strings->cursize >= strings->maxsize) {
                strings->maxsize *= 2;
                strings->words = realloc(strings->words, strings->maxsize * sizeof (char *));
            }
            strings->words[strings->cursize] = malloc(plen + 1);
            strcpy(strings->words[strings->cursize], prefix);
            strings->cursize++;
        }
        else {
            strcpy(newprefix, prefix);
            strncat(newprefix, &(cur->value), 1);
            yodawg_build_strings(cur, newprefix, strings);
        }
    }
    free(newprefix);
}

struct yowordlist *yodawg_create_wordlist(int size)
{
    struct yowordlist *wordlist;
    wordlist = malloc(sizeof (struct yowordlist));
    wordlist->cursize = 0;
    wordlist->maxsize = YO_STARTSIZE;
    wordlist->words = malloc(YO_STARTSIZE * sizeof (char *));
    return wordlist;
}

void yodawg_free_wordlist(struct yowordlist *wordlist)
{
    if(wordlist == NULL) return;
    int i;
    for(i = 0; i < wordlist->cursize; i++)
        free(wordlist->words[i]);
    free(wordlist->words);
    free(wordlist);
}

struct yowordlist *yodawg_find_strings(struct yonode *dawg, char *prefix)
{
    char c;
    char *p;
    int i;
    struct yowordlist *strings;

    // Create a wordlist.
    strings = yodawg_create_wordlist(YO_STARTSIZE);

    // Find deepest common point.
    p = prefix;
    while((c = *p++) != '\0') {
        i = yodawg_value_in_dawg(dawg, c);
        if(i < 0) { // Prefix not in dawg.
            strings->cursize = -1;
            return strings;
        }
        dawg = dawg->edges[i];
    }

    yodawg_build_strings(dawg, prefix, strings);
    return strings;
}
