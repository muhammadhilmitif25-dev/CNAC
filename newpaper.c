#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "newpaper.h"
#include "buffer.h"

/* === FITUR NEW (LEMBAR BARU) ===
   Bebaskan semua node linked list, lalu buat ulang buffer kosong. */
void JalankanFiturNew(Buffer *b)
{
    /* bebaskan semua node yang ada */
    bebaskanBuffer(b);

    /* buat ulang buffer bersih */
    initBuffer(b);

    printf("\n>> Lembar kerja baru siap digunakan!\n");
}