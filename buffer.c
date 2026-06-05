#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

static NodeBaris *buatNode(int tipeBarisAda)
{
    NodeBaris *baru = (NodeBaris *)malloc(sizeof(NodeBaris));
    if (baru == NULL)
    {
        fprintf(stderr, "ERROR: malloc gagal!\n");
        return NULL;
    }
    memset(baru->teks, 0, MAX_KOLOM);
    baru->barisAda = tipeBarisAda;
    baru->berikut = NULL;
    return baru;
}

int panjangNode(NodeBaris *node)
{
    int p = 0;
    while (p < MAX_KOLOM && node->teks[p] != '\0')
        p++;
    return p;
}

NodeBaris *ambilBaris(Buffer *b, int i)
{
    if (i < 0)
        return NULL;
    NodeBaris *cur = b->kepala;
    int hitung = 0;
    while (cur != NULL && hitung < i)
    {
        cur = cur->berikut;
        hitung++;
    }
    return cur;
}

static void sisipSetelah(Buffer *b, NodeBaris *prev, NodeBaris *baru)
{
    if (prev == NULL)
    {

        baru->berikut = b->kepala;
        b->kepala = baru;
    }
    else
    {
        baru->berikut = prev->berikut;
        prev->berikut = baru;
    }
    b->jumlah++;
}

static void hapusSetelah(Buffer *b, NodeBaris *prev)
{
    NodeBaris *target;
    if (prev == NULL)
    {
        target = b->kepala;
        b->kepala = target->berikut;
    }
    else
    {
        target = prev->berikut;
        prev->berikut = target->berikut;
    }
    free(target);
    b->jumlah--;
}

void initBuffer(Buffer *b)
{
    b->kepala = buatNode(BARIS_WRAP);
    b->jumlah = 1;
    b->cur.brs = 0;
    b->cur.klm = 0;
}

void bebaskanBuffer(Buffer *b)
{
    NodeBaris *cur = b->kepala;
    while (cur != NULL)
    {
        NodeBaris *next = cur->berikut;
        free(cur);
        cur = next;
    }
    b->kepala = NULL;
    b->jumlah = 0;
    b->cur.brs = 0;
    b->cur.klm = 0;
}

void insertChar(Buffer *b, char c)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL)
        return;

    int panjang = panjangNode(node);
    int kolom = b->cur.klm;

    if (panjang < MAX_KOLOM - 1)
    {

        int j;
        for (j = panjang; j > kolom; j--)
            node->teks[j] = node->teks[j - 1];
        node->teks[kolom] = c;
        node->teks[panjang + 1] = '\0';
        b->cur.klm++;
    }
    else
    {

        NodeBaris *bawah = node->berikut;
        if (bawah == NULL || bawah->barisAda == BARIS_ENTER)
        {

            NodeBaris *wrap = buatNode(BARIS_WRAP);
            if (wrap == NULL)
                return;
            sisipSetelah(b, node, wrap);
            bawah = wrap;
        }

        int pBawah = panjangNode(bawah);
        int j;

        if (kolom >= panjang)
        {

            for (j = pBawah; j > 0; j--)
                bawah->teks[j] = bawah->teks[j - 1];
            bawah->teks[0] = c;
            b->cur.brs++;
            b->cur.klm = 1;
        }
        else
        {

            char overflow = node->teks[panjang - 1];

            for (j = panjang - 1; j > kolom; j--)
                node->teks[j] = node->teks[j - 1];
            node->teks[kolom] = c;
            node->teks[MAX_KOLOM - 1] = '\0';

            for (j = pBawah; j > 0; j--)
                bawah->teks[j] = bawah->teks[j - 1];
            bawah->teks[0] = overflow;

            b->cur.klm = kolom + 1;
        }
    }
}

void deleteChar(Buffer *b)
{

    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    if (baris == 0 && kolom == 0)
        return;

    NodeBaris *node = ambilBaris(b, baris);
    if (node == NULL)
        return;

    if (kolom > 0)
    {

        int j;
        for (j = kolom - 1; j < MAX_KOLOM - 1; j++)
            node->teks[j] = node->teks[j + 1];
        node->teks[MAX_KOLOM - 1] = '\0';
        b->cur.klm--;

        NodeBaris *cur = node;
        NodeBaris *next = cur->berikut;
        while (next != NULL && next->barisAda == BARIS_WRAP)
        {
            int pCur = panjangNode(cur);
            if (pCur >= MAX_KOLOM - 1)
                break;

            cur->teks[pCur] = next->teks[0];

            for (j = 0; j < MAX_KOLOM - 1; j++)
                next->teks[j] = next->teks[j + 1];
            next->teks[MAX_KOLOM - 1] = '\0';

            if (next->teks[0] == '\0')
            {

                hapusSetelah(b, cur);

                next = cur->berikut;
            }
            else
            {
                cur = next;
                next = cur->berikut;
            }
        }
    }
    else
    {

        NodeBaris *atas = ambilBaris(b, baris - 1);
        if (atas == NULL)
            return;

        int posAkhir = panjangNode(atas);
        int ruang = (MAX_KOLOM - 1) - posAkhir;
        int k = 0;

        while (k < ruang && node->teks[k] != '\0')
        {
            atas->teks[posAkhir + k] = node->teks[k];
            k++;
        }

        int j;
        for (j = 0; j < MAX_KOLOM; j++)
        {
            if (k + j < MAX_KOLOM)
                node->teks[j] = node->teks[k + j];
            else
                node->teks[j] = '\0';
        }

        if (node->teks[0] == '\0')
        {
            hapusSetelah(b, atas);
        }

        b->cur.brs = baris - 1;
        b->cur.klm = posAkhir;
    }
}

void newLine(Buffer *b)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL)
        return;

    int kolom = b->cur.klm;
    int j;

    char sisa[MAX_KOLOM];
    memset(sisa, 0, MAX_KOLOM);
    for (j = kolom; j < MAX_KOLOM && node->teks[j] != '\0'; j++)
        sisa[j - kolom] = node->teks[j];

    for (j = kolom; j < MAX_KOLOM; j++)
        node->teks[j] = '\0';

    NodeBaris *baru = buatNode(BARIS_ENTER);
    if (baru == NULL)
        return;
    memcpy(baru->teks, sisa, MAX_KOLOM);

    sisipSetelah(b, node, baru);

    b->cur.brs++;
    b->cur.klm = 0;
}

void resetDisplayState(void) {}
