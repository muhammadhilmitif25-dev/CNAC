#ifndef CURSOR_H
#define CURSOR_H

/* forward declaration */
struct Buffer;
struct NodeBaris;

typedef struct {
    int brs; /* posisi baris  */
    int klm; /* posisi kolom  */
} Cursor;

/* fungsi gerak kursor */
void movekiri  (struct Buffer *b);
void movekanan (struct Buffer *b);
void moveatas  (struct Buffer *b);
void movebawah (struct Buffer *b);

/* helper panjang node – dipakai di cursor.c dan buffer.c */
int panjangNode(struct NodeBaris *node);

#endif /* CURSOR_H */