#include "yodawg.h"

struct yonode *yodawg_create()
{
    struct yonode *start, *eow;
    start = yodawg_create_node(0);
    if(start == NULL) return NULL;
    eow = yodawg_create_node(YO_EOW);
    if(eow == NULL) {
        yodawg_free_node(start);
        return NULL;
    }
    if(yodawg_add_node(start, eow)) {
        yodawg_free_node(eow);
        yodawg_free_node(start);
        return NULL;
    }
    return start;
}

struct yonode *yodawg_create_node(char value)
{
    struct yonode *node;
    node = malloc(sizeof (struct yonode));
    if(node == NULL) return NULL;
    node->cursize = 0;
    node->maxsize = YO_STARTSIZE;
    node->value = value;
    node->edges = malloc(YO_STARTSIZE * sizeof (struct yonode *));
    if(node->edges == NULL) {
        free(node);
        return NULL;
    }
    return node;
}

int yodawg_add_node(struct yonode *parent, struct yonode *child)
{
    if (parent->edges == NULL) {
        parent->edges = malloc(YO_STARTSIZE * sizeof (struct yonode *));
        if(parent->edges == NULL) return -1;
    }
    else if (parent->cursize == parent->maxsize) {
        parent->maxsize *= 2;
        parent->edges = realloc(parent->edges, parent->maxsize);
        if(parent->edges == NULL) return -1;
    }
    parent->edges[parent->cursize++] = child;
    return 0;
}

int yodawg_value_in_dawg(struct yonode *parent, char value)
{
    int i;
    for(i = 0; i < parent->cursize; i++)
        if (value == parent->edges[i]->value)
            return i;
    return -1;
}

int yodawg_add_string(struct yonode *dawg, char *str)
{
    char c;
    struct yonode *cur, *buf;
    int i;

    cur = dawg;
    while((c = *str++) != '\0') {
        i = yodawg_value_in_dawg(cur, c);
        if(i < 0) {
            buf = yodawg_create_node(c);
            if(yodawg_add_node(cur, buf)) return -1;
            cur = buf;
        }
        else {
            cur = cur->edges[i];
        }
    }
    if(yodawg_value_in_dawg(cur, YO_EOW) < 0) {
        if(yodawg_add_node(cur, dawg->edges[0])) return -1;
    }
    return 0;
}

void yodawg_free_node(struct yonode *node)
{
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
                if(nodes == NULL) return NULL;
            }
            nodes[*cursize] = cur;
            ++*cursize;
        }
        if(cur->cursize > 0)
            nodes = yodawg_add_dawg_to_list(nodes, cursize, maxsize, cur);
    }
    return nodes;
}

int yodawg_free_dawg(struct yonode *dawg)
{
    int *cursize;
    int *maxsize;
    int _cursize = 0;
    int _maxsize = YO_STARTSIZE;
    int i;
    struct yonode **nodes;

    cursize = &_cursize;
    maxsize = &_maxsize;
    nodes = malloc(*maxsize * sizeof (struct yonode *));
    if(nodes == NULL) return -1;

    nodes = yodawg_add_dawg_to_list(nodes, cursize, maxsize, dawg);
    if(nodes == NULL) return -1;

    // Free the children.
    for(i = 0; i < *cursize; i++) {
        yodawg_free_node(nodes[i]);
    }
    // Free the root.
    yodawg_free_node(dawg);

    free(nodes);
    return 0;
}

int yodawg_build_strings(struct yonode *dawg, const char *prefix, struct yowordlist *strings)
{
    struct yonode *cur;
    int i;
    int plen = strlen(prefix);
    char *newprefix;
    newprefix = malloc(plen + 2);
    if(newprefix == NULL) return -1;

    for(i = 0; i < dawg->cursize; i++) {
        cur = dawg->edges[i];
        if(cur->value == YO_EOW) {
            // Allocate enough memory.
            if(strings->cursize >= strings->maxsize) {
                strings->maxsize *= 2;
                strings->words = realloc(strings->words, strings->maxsize * sizeof (char *));
                if(strings->words == NULL) return -1;
            }
            strings->words[strings->cursize] = malloc(plen + 1);
            if(strings->words[strings->cursize] == NULL) return -1;
            strcpy(strings->words[strings->cursize], prefix);
            strings->cursize++;
        }
        else {
            strcpy(newprefix, prefix);
            strncat(newprefix, &(cur->value), 1);
            if(yodawg_build_strings(cur, newprefix, strings)) return -1;
        }
    }
    free(newprefix);
    return 0;
}

struct yowordlist *yodawg_create_wordlist(int size)
{
    struct yowordlist *wordlist;
    wordlist = malloc(sizeof (struct yowordlist));
    if(wordlist == NULL) return NULL;
    wordlist->cursize = 0;
    wordlist->maxsize = YO_STARTSIZE;
    wordlist->words = malloc(YO_STARTSIZE * sizeof (char *));
    if(wordlist->words == NULL) {
        free(wordlist);
        return NULL;
    }
    return wordlist;
}

void yodawg_free_wordlist(struct yowordlist *wordlist)
{
    int i;
    if(wordlist == NULL) return;
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
