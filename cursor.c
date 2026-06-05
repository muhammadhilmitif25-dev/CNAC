#include <stdio.h>
#include "cursor.h"
#include "buffer.h"

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
            b->cur.klm = panjangNode(atas);
        }
    }
}

void movekanan(Buffer *b)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL)
        return;

    int panjang = panjangNode(node);

    if (b->cur.klm < panjang)
    {
        b->cur.klm++;
    }
    else
    {

        NodeBaris *bawah = node->berikut;
        if (bawah != NULL)
        {
            b->cur.brs++;
            b->cur.klm = 0;
        }
    }
}

void moveatas(Buffer *b)
{
    if (b->cur.brs == 0)
        return;

    NodeBaris *atas = ambilBaris(b, b->cur.brs - 1);
    if (atas == NULL)
        return;

    b->cur.brs--;
    int panjang = panjangNode(atas);
    if (b->cur.klm > panjang)
        b->cur.klm = panjang;
}

void movebawah(Buffer *b)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL)
        return;

    NodeBaris *bawah = node->berikut;
    if (bawah == NULL)
        return;

    b->cur.brs++;
    int panjang = panjangNode(bawah);
    if (b->cur.klm > panjang)
        b->cur.klm = panjang;
}
