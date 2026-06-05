#ifndef BUFFER_H
#define BUFFER_H

#include "cursor.h"

#define MAX_KOLOM 1000
#define BARIS_WRAP 1
#define BARIS_ENTER 2

typedef struct NodeBaris
{
    char teks[MAX_KOLOM];
    int barisAda;
    struct NodeBaris *berikut;
} NodeBaris;

typedef struct Buffer
{
    NodeBaris *kepala;
    int jumlah;
    Cursor cur;
} Buffer;

void initBuffer(Buffer *b);
void insertChar(Buffer *b, char c);
void deleteChar(Buffer *b);
void newLine(Buffer *b);
void resetDisplayState(void);

NodeBaris *ambilBaris(Buffer *b, int i);

void bebaskanBuffer(Buffer *b);

#endif