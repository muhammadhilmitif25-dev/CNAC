#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "buffer.h"

/* === FITUR NEW (LEMBAR BARU) ===
   Reset semua isi buffer jadi kosong dan kursor ke posisi awal. */
void JalankanFiturNew(Buffer *b)
{
    int i, j;

    // kosongkan semua sel
    for (i = 0; i < MAX_BARIS; i++)
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = '\0';

    // kursor kembali ke pojok kiri atas
    b->cur.brs = 0;
    b->cur.klm = 0;

    printf("\n>> Lembar kerja baru siap digunakan!\n");
}