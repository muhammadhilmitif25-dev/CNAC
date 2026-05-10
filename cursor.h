#ifndef CURSOR_H
#define CURSOR_H

// forward declaration supaya bisa pakai struct Buffer di sini
// (karena cursor.h di-include sebelum buffer.h)
struct Buffer;

// struct kursor untuk nyimpen posisi baris dan kolom
typedef struct {
    int brs; // posisi baris (row)
    int klm; // posisi kolom (column)
} Cursor;

// deklarasi fungsi gerak kursor
void movekiri(struct Buffer *b);
void movekanan(struct Buffer *b);
void moveatas(struct Buffer *b);
void movebawah(struct Buffer *b);

#endif