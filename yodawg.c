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
    if (new == NULL) return -1;
    new->cursize = 0;
    new->maxsize = STARTSIZE;
    new->value = value;
    new->edges = malloc(STARTSIZE * (sizeof (struct yonode *)));
    if (new->edges == NULL) return -2;
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


int main(int argc, char **argv)
{
    struct yonode *dawg = yodawg_create();
    printf("start: 0x%x\n", dawg);
    printf("end:   0x%x\n", dawg->edges);
    printf("eow:   0x%x\n", dawg->edges[0]);
    printf("size:  %d\n", dawg->cursize);
    printf("eow value: %d\n", dawg->edges[0]->value);
    return 0;
}
