#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

/* Nilai barisAda */
#define BARIS_WRAP  1
#define BARIS_ENTER 2

/* =====================================================
   HELPER: buat node baris baru (kosong)
   ===================================================== */
static NodeBaris *buatNode(int tipeBarisAda)
{ // <--- TANDA KURUNG INI TADI HILANG DI KODE KAMU
    NodeBaris *baru = (NodeBaris *)malloc(sizeof(NodeBaris));
    if (baru == NULL) {
        fprintf(stderr, "ERROR: malloc gagal!\n");
        return NULL;
    }
    memset(baru->teks, 0, MAX_KOLOM);
    baru->barisAda = tipeBarisAda;
    baru->berikut  = NULL;
    return baru;
}

/* =====================================================
   HELPER: panjang teks sebuah node
   ===================================================== */
int panjangNode(NodeBaris *node)
{
    int p = 0;
    while (p < MAX_KOLOM && node->teks[p] != '\0')
        p++;
    return p;
}

/* =====================================================
   HELPER: ambil node ke-i (0-based), NULL kalau tidak ada
   ===================================================== */
NodeBaris *ambilBaris(Buffer *b, int i)
{
    if (i < 0) return NULL;
    NodeBaris *cur = b->kepala;
    int hitung = 0;
    while (cur != NULL && hitung < i) {
        cur = cur->berikut;
        hitung++;
    }
    return cur;
}

/* =====================================================
   HELPER: sisipkan node baru SETELAH node 'prev'
   ===================================================== */
static void sisipSetelah(Buffer *b, NodeBaris *prev, NodeBaris *baru)
{
    if (prev == NULL) {
        /* sisip di depan */
        baru->berikut = b->kepala;
        b->kepala     = baru;
    } else {
        baru->berikut  = prev->berikut;
        prev->berikut  = baru;
    }
    b->jumlah++;
}

/* =====================================================
   HELPER: hapus node SETELAH node 'prev' (atau kepala kalau prev=NULL)
   ===================================================== */
static void hapusSetelah(Buffer *b, NodeBaris *prev)
{
    NodeBaris *target;
    if (prev == NULL) {
        target    = b->kepala;
        b->kepala = target->berikut;
    } else {
        target        = prev->berikut;
        prev->berikut = target->berikut;
    }
    free(target);
    b->jumlah--;
}

/* =====================================================
   INISIALISASI BUFFER
   ===================================================== */
void initBuffer(Buffer *b)
{
    b->kepala     = buatNode(BARIS_WRAP); /* baris 0 selalu ada */
    b->jumlah     = 1;
    b->cur.brs    = 0;
    b->cur.klm    = 0;
}

/* =====================================================
   BEBASKAN SEMUA MEMORI
   ===================================================== */
void bebaskanBuffer(Buffer *b)
{
    NodeBaris *cur = b->kepala;
    while (cur != NULL) {
        NodeBaris *next = cur->berikut;
        free(cur);
        cur = next;
    }
    b->kepala  = NULL;
    b->jumlah  = 0;
    b->cur.brs = 0;
    b->cur.klm = 0;
}

/* =====================================================
   INSERT KARAKTER
   Kalau baris penuh → auto-wrap ke baris bawah.
   ===================================================== */
void insertChar(Buffer *b, char c)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL) return;

    int panjang = panjangNode(node);
    int kolom   = b->cur.klm;

    if (panjang < MAX_KOLOM - 1)
    {
        /* --- baris masih muat: sisipkan di kolom kursor --- */
        int j;
        for (j = panjang; j > kolom; j--)
            node->teks[j] = node->teks[j - 1];
        node->teks[kolom] = c;
        node->teks[panjang + 1] = '\0';
        b->cur.klm++;
    }
    else
    {
        /* --- baris penuh: perlu wrap --- */


        /* pastikan ada node baris bawah bertipe WRAP */
        NodeBaris *bawah = node->berikut;
        if (bawah == NULL || bawah->barisAda == BARIS_ENTER)
        {
            /* buat node wrap baru dan sisipkan setelah node ini */
            NodeBaris *wrap = buatNode(BARIS_WRAP);
            if (wrap == NULL) return;
            sisipSetelah(b, node, wrap);
            bawah = wrap;
        }

        int pBawah = panjangNode(bawah);
        int j;

        if (kolom >= panjang)
        {
            /* kursor di ujung: karakter baru langsung ke baris bawah posisi 0 */
            for (j = pBawah; j > 0; j--)
                bawah->teks[j] = bawah->teks[j - 1];
            bawah->teks[0] = c;
            b->cur.brs++;
            b->cur.klm = 1;
        }
        else
        {
            /* kursor di tengah: overflow karakter terakhir ke baris bawah */
            char overflow = node->teks[panjang - 1];

            for (j = panjang - 1; j > kolom; j--)
                node->teks[j] = node->teks[j - 1];
            node->teks[kolom] = c;
            node->teks[MAX_KOLOM - 1] = '\0';

            /* sisipkan overflow di awal baris bawah */
            for (j = pBawah; j > 0; j--)
                bawah->teks[j] = bawah->teks[j - 1];
            bawah->teks[0] = overflow;

            b->cur.klm = kolom + 1;
        }
    }
}

/* =====================================================
   DELETE KARAKTER (BACKSPACE)
   ===================================================== */
void deleteChar(Buffer *b)
{
       
    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    if (baris == 0 && kolom == 0) return;

    NodeBaris *node = ambilBaris(b, baris);
    if (node == NULL) return;

    if (kolom > 0)
    {
        /* --- hapus karakter sebelum kursor di baris ini --- */
        int j;
        for (j = kolom - 1; j < MAX_KOLOM - 1; j++)
            node->teks[j] = node->teks[j + 1];
        node->teks[MAX_KOLOM - 1] = '\0';
        b->cur.klm--;

        /* tarik karakter dari baris WRAP di bawah secara berantai */
        NodeBaris *cur  = node;
        NodeBaris *next = cur->berikut;
        while (next != NULL && next->barisAda == BARIS_WRAP)
        {
            int pCur = panjangNode(cur);
            if (pCur >= MAX_KOLOM - 1) break;

            /* tarik karakter pertama dari next ke akhir cur */
            cur->teks[pCur] = next->teks[0];

            /* geser next ke kiri */
            for (j = 0; j < MAX_KOLOM - 1; j++)
                next->teks[j] = next->teks[j + 1];
            next->teks[MAX_KOLOM - 1] = '\0';

            if (next->teks[0] == '\0')
            {
                /* next jadi kosong, hapus node itu */
                hapusSetelah(b, cur);
                /* cur->berikut sudah diupdate oleh hapusSetelah */
                next = cur->berikut;
            }
            else
            {
                cur  = next;
                next = cur->berikut;
            }
        }
    }
    else
    {
        /* --- kolom == 0: gabung ke baris atas --- */
        NodeBaris *atas = ambilBaris(b, baris - 1);
        if (atas == NULL) return;

        int posAkhir = panjangNode(atas);
        int ruang    = (MAX_KOLOM - 1) - posAkhir;
        int k        = 0;

        while (k < ruang && node->teks[k] != '\0') {
            atas->teks[posAkhir + k] = node->teks[k];
            k++;
        }

        /* geser sisa yang tidak muat ke awal node ini */
        int j;
        for (j = 0; j < MAX_KOLOM; j++) {
            if (k + j < MAX_KOLOM)
                node->teks[j] = node->teks[k + j];
            else
                node->teks[j] = '\0';
        }

     if (node->teks[0] == '\0')
{
    hapusSetelah(b, atas);
}

/* hitung ulang posisi setelah linked list berubah */
b->cur.brs = baris - 1;
b->cur.klm = posAkhir;
    }
   
}


/* =====================================================
   NEW LINE (ENTER)
   ===================================================== */
void newLine(Buffer *b)
{
    NodeBaris *node = ambilBaris(b, b->cur.brs);
    if (node == NULL) return;

    int kolom = b->cur.klm;
    int j;

    /* ambil sisa teks setelah kursor */
    char sisa[MAX_KOLOM];
    memset(sisa, 0, MAX_KOLOM);
    for (j = kolom; j < MAX_KOLOM && node->teks[j] != '\0'; j++)
        sisa[j - kolom] = node->teks[j];

    /* potong baris ini di posisi kursor */
    for (j = kolom; j < MAX_KOLOM; j++)
        node->teks[j] = '\0';

    /* buat node baru bertipe ENTER dan isi dengan sisa */
    NodeBaris *baru = buatNode(BARIS_ENTER);
    if (baru == NULL) return;
    memcpy(baru->teks, sisa, MAX_KOLOM);

    /* sisipkan setelah node saat ini */
    sisipSetelah(b, node, baru);

    b->cur.brs++;
    b->cur.klm = 0;
}

/* =====================================================
   RESET DISPLAY STATE
   ===================================================== */
void resetDisplayState(void) {}


