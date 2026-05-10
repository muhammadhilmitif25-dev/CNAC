#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

/* =====================================================
   INISIALISASI BUFFER
   Kosongkan semua sel, kursor mulai dari pojok kiri atas
   ===================================================== */
void initBuffer(Buffer *b)
{
    int i, j;
    for (i = 0; i < MAX_BARIS; i++)
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = '\0';

    b->cur.brs = 0;
    b->cur.klm = 0;
}

/* =====================================================
   INSERT KARAKTER
   Masukkan karakter di posisi kursor, geser yang lain ke kanan
   ===================================================== */
void insertChar(Buffer *b, char c)
{
    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    if (baris >= MAX_BARIS) return;

    // hitung panjang teks di baris ini
    int panjang = 0;
    while (panjang < MAX_KOLOM && b->text[baris][panjang] != '\0')
        panjang++;

    if (panjang >= MAX_KOLOM - 1) return; // baris penuh

    // geser karakter setelah kursor ke kanan
    int j;
    for (j = panjang; j > kolom; j--)
        b->text[baris][j] = b->text[baris][j - 1];

    // taruh karakter baru
    b->text[baris][kolom] = c;
    b->cur.klm++;
}

/* =====================================================
   DELETE KARAKTER (BACKSPACE)
   Hapus karakter sebelum kursor.
   Kalau kursor di awal baris, gabungkan dengan baris atas.
   ===================================================== */
void deleteChar(Buffer *b)
{
    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    if (baris == 0 && kolom == 0) return; // sudah di posisi paling awal

    if (kolom > 0)
    {
        // hapus karakter sebelum kursor, geser sisanya ke kiri
        int j;
        for (j = kolom - 1; j < MAX_KOLOM - 1; j++)
            b->text[baris][j] = b->text[baris][j + 1];
        b->text[baris][MAX_KOLOM - 1] = '\0';

        b->cur.klm--;
    }
    else
    {
        // kursor di kolom 0 -> gabungkan baris ini ke baris atas
        int barisAtas = baris - 1;

        // cari posisi akhir baris atas
        int posAkhir = 0;
        while (posAkhir < MAX_KOLOM && b->text[barisAtas][posAkhir] != '\0')
            posAkhir++;

        // salin isi baris sekarang ke akhir baris atas
        int k = 0;
        while (k < MAX_KOLOM && b->text[baris][k] != '\0' && posAkhir + k < MAX_KOLOM)
        {
            b->text[barisAtas][posAkhir + k] = b->text[baris][k];
            k++;
        }

        // geser semua baris ke atas (hapus baris yang sudah digabung)
        int i, j;
        for (i = baris; i < MAX_BARIS - 1; i++)
            for (j = 0; j < MAX_KOLOM; j++)
                b->text[i][j] = b->text[i + 1][j];

        for (j = 0; j < MAX_KOLOM; j++)
            b->text[MAX_BARIS - 1][j] = '\0';

        // pindah kursor ke akhir baris atas
        b->cur.brs = barisAtas;
        b->cur.klm = posAkhir;
    }
}

/* =====================================================
   NEW LINE (ENTER)
   Buat baris baru. Teks setelah kursor dipindah ke baris berikutnya.

   BUG FIX: sisa[] harus diinisialisasi ke '\0' dulu!
   Kalau tidak, nilai sampah dari stack bisa masuk ke buffer
   dan muncul jadi karakter aneh seperti |@╘_z
   ===================================================== */
void newLine(Buffer *b)
{
    if (b->cur.brs >= MAX_BARIS - 1) return;

    int baris = b->cur.brs;
    int kolom = b->cur.klm;

    // inisialisasi sisa ke kosong dulu - ini wajib!
    char sisa[MAX_KOLOM];
    int j;
    for (j = 0; j < MAX_KOLOM; j++)
        sisa[j] = '\0';

    // simpan teks sebelah kanan kursor
    for (j = kolom; j < MAX_KOLOM; j++)
        sisa[j - kolom] = b->text[baris][j];

    // hapus teks kanan kursor dari baris ini
    for (j = kolom; j < MAX_KOLOM; j++)
        b->text[baris][j] = '\0';

    // geser semua baris bawah ke bawah satu baris
    int i;
    for (i = MAX_BARIS - 1; i > baris + 1; i--)
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = b->text[i - 1][j];

    // kosongkan baris baru, lalu isi dengan sisa teks
    for (j = 0; j < MAX_KOLOM; j++)
        b->text[baris + 1][j] = '\0';
    for (j = 0; j < MAX_KOLOM; j++)
        b->text[baris + 1][j] = sisa[j];

    // pindah kursor ke awal baris baru
    b->cur.brs++;
    b->cur.klm = 0;
}

void resetDisplayState(void) {}

/* =====================================================
   TAMPILKAN BUFFER
   Cetak dari baris 0 sampai baris terakhir yang ada isi,
   minimal sampai baris kursor. Tidak ada baris kosong ekstra.

   Kursor '|' dicetak SATU KALI dengan flag kursorSudahCetak.
   ===================================================== */
void displayBuffer(Buffer *b)
{
    int i, j;

    // cari baris paling bawah yang ada isinya
    int barisAkhir = b->cur.brs;
    for (i = MAX_BARIS - 1; i > barisAkhir; i--)
    {
        for (j = 0; j < MAX_KOLOM; j++)
            if (b->text[i][j] != '\0') { barisAkhir = i; goto ditemukan; }
    }
    ditemukan:;

    for (i = 0; i <= barisAkhir; i++)
    {
        int kursorSudahCetak = 0;

        for (j = 0; j <= MAX_KOLOM; j++)
        {
            if (i == b->cur.brs && j == b->cur.klm && !kursorSudahCetak)
            {
                printf("|");
                kursorSudahCetak = 1;
            }
            if (j == MAX_KOLOM || b->text[i][j] == '\0') break;
            printf("%c", b->text[i][j]);
        }

        printf("\n");
    }
}