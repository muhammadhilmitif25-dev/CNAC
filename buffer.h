#ifndef BUFFER_H
#define BUFFER_H

#include "cursor.h"

/* Batas maksimal karakter per baris */
#define MAX_KOLOM 1000


/* -------------------------------------------------------
   NODE BARIS  – satu baris teks dalam linked list
   ------------------------------------------------------- */
typedef struct NodeBaris {
    char teks[MAX_KOLOM];       /* isi teks baris ini          */
    int  barisAda;              /* 0=tidak ada, 1=wrap, 2=enter */
    struct NodeBaris *berikut;  /* pointer ke baris berikutnya */
} NodeBaris;

/* -------------------------------------------------------
   BUFFER  – linked list semua baris + posisi kursor
   ------------------------------------------------------- */
typedef struct Buffer {
    NodeBaris *kepala;   /* pointer ke baris pertama  */
    int        jumlah;   /* total jumlah baris        */
    Cursor     cur;      /* posisi kursor (brs, klm)  */
} Buffer;

/* -------------------------------------------------------
   FUNGSI BUFFER
   ------------------------------------------------------- */
void       initBuffer       (Buffer *b);
void       insertChar       (Buffer *b, char c);
void       deleteChar       (Buffer *b);
void       newLine          (Buffer *b);
void       resetDisplayState(void);

/* Helper: ambil pointer ke NodeBaris ke-i (0-based) */
NodeBaris *ambilBaris       (Buffer *b, int i);

/* Helper: bebaskan semua memori linked list */
void       bebaskanBuffer   (Buffer *b);

#endif /* BUFFER_H */