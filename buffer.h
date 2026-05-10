#ifndef BUFFER_H
#define BUFFER_H

#include "cursor.h"

// batas maksimal baris dan kolom yang bisa ditampung
#define MAX_BARIS 100
#define MAX_KOLOM 100

// struct buffer untuk nyimpen semua teks dan posisi kursor
struct Buffer {
    char text[MAX_BARIS][MAX_KOLOM];
    Cursor cur;
};

typedef struct Buffer Buffer;

// fungsi-fungsi buffer
void initBuffer(Buffer *b);
void insertChar(Buffer *b, char c);
void deleteChar(Buffer *b);
void newLine(Buffer *b);
void displayBuffer(Buffer *b);
void resetDisplayState(void); // reset state display saat masuk mode ketik

#endif