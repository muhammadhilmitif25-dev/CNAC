#include "cursor.h"
#include "buffer.h"

/* === GERAK KURSOR KE KIRI ===
   Kalau masih bisa ke kiri di baris ini, geser saja.
   Kalau sudah di kolom 0, naik ke baris atas (ke akhir baris itu).
   Tidak naik kalau tidak ada baris aktif di atas. */
void movekiri(Buffer *b)
{
    if (b->cur.klm > 0)
    {
        b->cur.klm--;
    }
    else if (b->cur.brs > 0 && b->barisAda[b->cur.brs - 1])
    {
        b->cur.brs--;

        int panjang = 0;
        while (panjang < MAX_KOLOM && b->text[b->cur.brs][panjang] != '\0')
            panjang++;

        b->cur.klm = panjang;
    }
}

/* === GERAK KURSOR KE KANAN ===
   Kalau masih ada karakter di sebelah kanan, geser ke sana.
   Kalau sudah di akhir baris, turun ke baris bawah HANYA kalau baris
   bawah memang ada (barisAda=1). */
void movekanan(Buffer *b)
{
    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    if (kolom < MAX_KOLOM - 1 && b->text[baris][kolom] != '\0')
    {
        b->cur.klm++;
    }
    else if (baris < MAX_BARIS - 1 && b->barisAda[baris + 1])
    {
        // turun ke baris berikutnya, kolom 0
        b->cur.brs++;
        b->cur.klm = 0;
    }
    // kalau baris bawah tidak ada, diam saja
}

/* === GERAK KURSOR KE ATAS ===
   Naik satu baris. Hanya kalau baris atas ada (barisAda=1). */
void moveatas(Buffer *b)
{
    if (b->cur.brs > 0 && b->barisAda[b->cur.brs - 1])
    {
        b->cur.brs--;

        int panjang = 0;
        while (panjang < MAX_KOLOM && b->text[b->cur.brs][panjang] != '\0')
            panjang++;

        if (b->cur.klm > panjang)
            b->cur.klm = panjang;
    }
}

/* === GERAK KURSOR KE BAWAH ===
   Turun satu baris HANYA kalau baris bawah ada (barisAda=1). */
void movebawah(Buffer *b)
{
    int barisBawah = b->cur.brs + 1;
    if (barisBawah >= MAX_BARIS) return;
    if (!b->barisAda[barisBawah]) return; // baris bawah tidak ada, diam

    b->cur.brs = barisBawah;

    int panjang = 0;
    while (panjang < MAX_KOLOM && b->text[b->cur.brs][panjang] != '\0')
        panjang++;

    if (b->cur.klm > panjang)
        b->cur.klm = panjang;
}