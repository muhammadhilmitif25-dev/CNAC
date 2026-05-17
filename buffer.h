#ifndef BUFFER_H
#define BUFFER_H

#include "cursor.h"

// batas maksimal baris dan kolom yang bisa ditampung
#define MAX_BARIS 1000
#define MAX_KOLOM 1000

// struct buffer untuk nyimpen semua teks dan posisi kursor
struct Buffer {
    char text[MAX_BARIS][MAX_KOLOM];
    Cursor cur;
    int barisAda[MAX_BARIS]; // 1 = baris ini pernah dibuat (Enter atau baris 0)
};

typedef struct Buffer Buffer;

// fungsi-fungsi buffer
void initBuffer(Buffer *b);
void insertChar(Buffer *b, char c);
void deleteChar(Buffer *b);
void newLine(Buffer *b);
void displayBuffer(Buffer *b);
void resetDisplayState(void);

#endif