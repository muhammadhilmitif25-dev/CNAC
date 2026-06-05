#ifndef CURSOR_H
#define CURSOR_H

struct Buffer;
struct NodeBaris;

typedef struct
{
    int brs;
    int klm;
} Cursor;

void movekiri(struct Buffer *b);
void movekanan(struct Buffer *b);
void moveatas(struct Buffer *b);
void movebawah(struct Buffer *b);

int panjangNode(struct NodeBaris *node);

#endif