#include <stdlib.h>
#include <stdio.h>

#define STARTSIZE 16
#define EOW -1

struct yonode {
    char value;
    int cursize, maxsize;
    struct yonode **edges;
};

void yodawg_add_node(struct yonode *, struct yonode *);
int yodawg_value_in_dawg(struct yonode *, char);

struct yonode *yodawg_create_node(char value)
{
    struct yonode *new;
    new = malloc(sizeof (struct yonode));
    new->cursize = 0;
    new->maxsize = STARTSIZE;
    new->value = value;
    new->edges = malloc(STARTSIZE * (sizeof (struct yonode *)));
    return new;
}

struct yonode *yodawg_create()
{
    struct yonode *start, *eow;
    start = yodawg_create_node(0);
    eow = yodawg_create_node(EOW);
    yodawg_add_node(start, eow);
    return start;
}

void yodawg_add_node(struct yonode *parent, struct yonode *child)
{
    if (parent->edges == NULL) {
        parent->edges = malloc(STARTSIZE * (sizeof (struct yonode *)));
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
            cur = cur->edges[cur->cursize - 1];
        }
        else {
            cur = cur->edges[i];
        }
    }
    if(yodawg_value_in_dawg(cur, EOW) < 0) {
        yodawg_add_node(cur, dawg->edges[0]);
    }
}

void yodawg_free_node(struct yonode *node)
{
    free(node);
}

void yodawg_free_dawg(struct yonode *dawg)
{
    while(dawg->cursize > 0) {
        dawg->cursize--;
        yodawg_free_dawg(dawg->edges[dawg->cursize]);
        dawg->edges[dawg->cursize] = NULL;
    }
    yodawg_free_node(dawg);
}

char *yodawg_find_strings(char *prefix)
{
}


int main(int argc, char **argv)
{
    struct yonode *dawg = yodawg_create();
    struct yonode *cur;
    int i;
    yodawg_add_string(dawg, "foo0");
    yodawg_add_string(dawg, "bar");
    yodawg_add_string(dawg, "baz");
    yodawg_add_string(dawg, "quxxxx");

    if (argc < 2) return -1;
    printf("find: %s\n", argv[1]);
    i = yodawg_value_in_dawg(dawg, argv[1][0]);
    printf("start at: %d\n", i);
    cur = dawg->edges[i];
    while(cur->value != EOW) {
        printf("cur: 0x%x\n", cur);
        printf("val: %c\n", cur->value);
        cur = cur->edges[cur->cursize - 1]; 
    }
    cur = NULL;
    yodawg_free_dawg(dawg);
    printf("Freed dawg.\n");
    return 0;
}
