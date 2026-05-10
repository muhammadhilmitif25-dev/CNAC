#include "cursor.h"
#include "buffer.h"

/* === GERAK KURSOR KE KIRI ===
   Kalau masih bisa ke kiri di baris ini, geser saja.
   Kalau sudah di kolom 0, naik ke baris atas (ke akhir baris itu). */
void movekiri(Buffer *b)
{
    if (b->cur.klm > 0)
    {
        b->cur.klm--;
    }
    else if (b->cur.brs > 0)
    {
        // naik satu baris, pindah ke akhir baris atas
        b->cur.brs--;

        // hitung panjang baris atas
        int panjang = 0;
        while (panjang < MAX_KOLOM && b->text[b->cur.brs][panjang] != '\0')
            panjang++;

        b->cur.klm = panjang;
    }
}

/* === GERAK KURSOR KE KANAN ===
   Kalau masih ada karakter di sebelah kanan, geser ke sana.
   Kalau sudah di akhir baris, turun ke awal baris bawah. */
void movekanan(Buffer *b)
{
    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    // cek apakah masih ada karakter di posisi kursor sekarang
    if (kolom < MAX_KOLOM - 1 && b->text[baris][kolom] != '\0')
    {
        b->cur.klm++;
    }
    else if (baris < MAX_BARIS - 1)
    {
        // turun ke baris berikutnya, kolom 0
        b->cur.brs++;
        b->cur.klm = 0;
    }
}

/* === GERAK KURSOR KE ATAS ===
   Naik satu baris. Kalau kolom sekarang melebihi panjang baris atas,
   kursor digeser ke akhir baris atas biar tidak out of bounds. */
void moveatas(Buffer *b)
{
    if (b->cur.brs > 0)
    {
        b->cur.brs--;

        // hitung panjang baris yang baru
        int panjang = 0;
        while (panjang < MAX_KOLOM && b->text[b->cur.brs][panjang] != '\0')
            panjang++;

        // pastikan kolom tidak melebihi panjang baris
        if (b->cur.klm > panjang)
            b->cur.klm = panjang;
    }
}

/* === GERAK KURSOR KE BAWAH ===
   Turun satu baris, tapi HANYA kalau baris bawah memang ada isinya.
   Kalau kosong, diam saja supaya tidak muncul baris hantu. */
void movebawah(Buffer *b)
{
    int barisBawah = b->cur.brs + 1;
    if (barisBawah >= MAX_BARIS) return;

    // cek apakah baris bawah ada isinya
    int adaIsi = 0;
    int j;
    for (j = 0; j < MAX_KOLOM; j++)
        if (b->text[barisBawah][j] != '\0') { adaIsi = 1; break; }

    if (!adaIsi) return; // baris kosong, jangan turun

    b->cur.brs = barisBawah;

    // sesuaikan kolom kalau baris bawah lebih pendek
    int panjang = 0;
    while (panjang < MAX_KOLOM && b->text[b->cur.brs][panjang] != '\0')
        panjang++;

    if (b->cur.klm > panjang)
        b->cur.klm = panjang;
}