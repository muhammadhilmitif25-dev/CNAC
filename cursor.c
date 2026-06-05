#include <stdio.h>
#include "cursor.h"
#include "buffer.h"

/* =====================================================
   GERAK KURSOR KE KIRI
   ===================================================== */
void movekiri(Buffer *b)
{
    if (b->cur.klm > 0)
    {
        b->cur.klm--;
    }
    else if (b->cur.brs > 0)
    {
        NodeBaris *atas = ambilBaris(b, b->cur.brs - 1);
        if (atas != NULL)
        {
            b->cur.brs--;
            b->cur.klm = panjangNode(atas); /* pindah ke akhir baris atas */
        }
    }
}

/* =====================================================
   GERAK KURSOR KE KANAN
   ===================================================== */
void movekanan(Buffer *b)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL) return;

    int panjang = panjangNode(node);

    if (b->cur.klm < panjang)
    {
        b->cur.klm++;
    }
    else
    {
        /* coba turun ke baris berikutnya */
        NodeBaris *bawah = node->berikut;
        if (bawah != NULL)
        {
            b->cur.brs++;
            b->cur.klm = 0;
        }
    }
}

/* =====================================================
   GERAK KURSOR KE ATAS
   ===================================================== */
void moveatas(Buffer *b)
{
    if (b->cur.brs == 0) return;

    NodeBaris *atas = ambilBaris(b, b->cur.brs - 1);
    if (atas == NULL) return;

    b->cur.brs--;
    int panjang = panjangNode(atas);
    if (b->cur.klm > panjang)
        b->cur.klm = panjang;
}

/* =====================================================
   GERAK KURSOR KE BAWAH
   ===================================================== */
void movebawah(Buffer *b)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL) return;

    NodeBaris *bawah = node->berikut;
    if (bawah == NULL) return;

    b->cur.brs++;
    int panjang = panjangNode(bawah);
    if (b->cur.klm > panjang)
        b->cur.klm = panjang;
}

