#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"

/* barisAda[i]:
   0 = baris tidak ada (kosong murni)
   1 = baris ada, hasil auto-wrap atau baris 0
   2 = baris ada, hasil Enter eksplisit user
*/
#define BARIS_WRAP  1
#define BARIS_ENTER 2

/* helper: panjang teks baris i */
static int panjangBaris(Buffer *b, int i)
{
    int p = 0;
    while (p < MAX_KOLOM && b->text[i][p] != '\0')
        p++;
    return p;
}

/* =====================================================
   INISIALISASI BUFFER
   ===================================================== */
void initBuffer(Buffer *b)
{
    int i, j;
    for (i = 0; i < MAX_BARIS; i++)
    {
        b->barisAda[i] = 0;
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = '\0';
    }
    b->barisAda[0] = BARIS_WRAP;
    b->cur.brs = 0;
    b->cur.klm = 0;
}

/* =====================================================
   INSERT KARAKTER
   Kalau baris penuh, auto-wrap ke baris bawah.
   ===================================================== */
void insertChar(Buffer *b, char c)
{
    int baris = b->cur.brs;
    int kolom  = b->cur.klm;

    if (baris >= MAX_BARIS) return;

    int panjang = panjangBaris(b, baris);

    if (panjang >= MAX_KOLOM - 1)
    {
        if (b->cur.brs >= MAX_BARIS - 1) return;

        int barisBawah = baris + 1;
        int i, j;

        // buat baris wrap baru di bawah kalau belum ada atau baris bawah adalah Enter
        if (!b->barisAda[barisBawah] || b->barisAda[barisBawah] == BARIS_ENTER)
        {
            for (i = MAX_BARIS - 1; i > barisBawah; i--)
            {
                b->barisAda[i] = b->barisAda[i - 1];
                for (j = 0; j < MAX_KOLOM; j++)
                    b->text[i][j] = b->text[i - 1][j];
            }
            b->barisAda[barisBawah] = BARIS_WRAP;
            for (j = 0; j < MAX_KOLOM; j++)
                b->text[barisBawah][j] = '\0';
        }

        int p = panjangBaris(b, barisBawah);

        if (kolom >= panjang)
        {
            // kursor di ujung baris: karakter baru langsung ke baris bawah kolom 0
            for (j = p; j > 0; j--)
                b->text[barisBawah][j] = b->text[barisBawah][j - 1];
            b->text[barisBawah][0] = c;
            b->cur.brs = barisBawah;
            b->cur.klm = 1;
        }
        else
        {
            // kursor di tengah baris: ambil karakter terakhir sebagai overflow,
            // sisipkan karakter baru di posisi kolom, overflow ke baris bawah
            char overflow = b->text[baris][panjang - 1];

            for (j = panjang - 1; j > kolom; j--)
                b->text[baris][j] = b->text[baris][j - 1];
            b->text[baris][kolom] = c;
            b->text[baris][MAX_KOLOM - 1] = '\0';

            // sisipkan overflow ke awal baris bawah
            for (j = p; j > 0; j--)
                b->text[barisBawah][j] = b->text[barisBawah][j - 1];
            b->text[barisBawah][0] = overflow;

            b->cur.klm = kolom + 1;
        }
    }
    else
    {
        int j;
        for (j = panjang; j > kolom; j--)
            b->text[baris][j] = b->text[baris][j - 1];
        b->text[baris][kolom] = c;
        b->cur.klm++;
    }
}

/* =====================================================
   DELETE KARAKTER (BACKSPACE)

   kolom > 0:
     - Hapus karakter sebelum kursor
     - Kalau baris bawah adalah WRAP: tarik semua karakter mengalir ke kiri
       (satu per satu sampai baris wrap habis/hilang)
     - Kalau baris bawah adalah ENTER atau tidak ada: stop di sini

   kolom == 0:
     - Kalau baris ini WRAP: gabung ke baris atas, sisa yang tidak muat tetap di sini
     - Kalau baris ini ENTER: hapus newline, gabung normal
   ===================================================== */
void deleteChar(Buffer *b)
{
    int baris = b->cur.brs;
    int kolom  = b->cur.klm;

    if (baris == 0 && kolom == 0) return;

    if (kolom > 0)
    {
        // hapus karakter sebelum kursor
        int j;
        for (j = kolom - 1; j < MAX_KOLOM - 1; j++)
            b->text[baris][j] = b->text[baris][j + 1];
        b->text[baris][MAX_KOLOM - 1] = '\0';
        b->cur.klm--;

        // Tarik karakter dari semua baris WRAP di bawah secara berantai
        // supaya teks mengalir ke kiri melewati batas baris
        int cur = baris;
        int next = cur + 1;
        while (next < MAX_BARIS && b->barisAda[next] == BARIS_WRAP)
        {
            int panjangCur = panjangBaris(b, cur);
            if (panjangCur >= MAX_KOLOM - 1) break; // baris cur penuh, stop

            // tarik karakter pertama dari baris next ke akhir baris cur
            b->text[cur][panjangCur] = b->text[next][0];

            // geser baris next ke kiri satu posisi
            for (j = 0; j < MAX_KOLOM - 1; j++)
                b->text[next][j] = b->text[next][j + 1];
            b->text[next][MAX_KOLOM - 1] = '\0';

            // kalau baris next jadi kosong, hapus baris itu
            if (b->text[next][0] == '\0')
            {
                int i;
                for (i = next; i < MAX_BARIS - 1; i++)
                {
                    b->barisAda[i] = b->barisAda[i + 1];
                    for (j = 0; j < MAX_KOLOM; j++)
                        b->text[i][j] = b->text[i + 1][j];
                }
                b->barisAda[MAX_BARIS - 1] = 0;
                for (j = 0; j < MAX_KOLOM; j++)
                    b->text[MAX_BARIS - 1][j] = '\0';
                // next tetap sama karena baris sudah digeser ke atas
            }
            else
            {
                // lanjut ke baris berikutnya
                cur = next;
                next = cur + 1;
            }
        }
    }
    else
    {
        // kolom == 0
        int barisAtas = baris - 1;
        int posAkhir  = panjangBaris(b, barisAtas);

        // hitung ruang yang tersisa di baris atas
        int ruang = (MAX_KOLOM - 1) - posAkhir;
        int k = 0;
        while (k < ruang && b->text[baris][k] != '\0')
        {
            b->text[barisAtas][posAkhir + k] = b->text[baris][k];
            k++;
        }

        // geser sisa yang tidak muat ke awal baris ini
        int j;
        for (j = 0; j < MAX_KOLOM; j++)
        {
            if (k + j < MAX_KOLOM)
                b->text[baris][j] = b->text[baris][k + j];
            else
                b->text[baris][j] = '\0';
        }

        // kalau baris ini jadi kosong, hapus baris ini
        if (b->text[baris][0] == '\0')
        {
            int i;
            for (i = baris; i < MAX_BARIS - 1; i++)
            {
                b->barisAda[i] = b->barisAda[i + 1];
                for (j = 0; j < MAX_KOLOM; j++)
                    b->text[i][j] = b->text[i + 1][j];
            }
            b->barisAda[MAX_BARIS - 1] = 0;
            for (j = 0; j < MAX_KOLOM; j++)
                b->text[MAX_BARIS - 1][j] = '\0';
        }

        b->cur.brs = barisAtas;
        b->cur.klm = posAkhir;
    }
}

/* =====================================================
   NEW LINE (ENTER)
   ===================================================== */
void newLine(Buffer *b)
{
    if (b->cur.brs >= MAX_BARIS - 1) return;

    int baris = b->cur.brs;
    int kolom  = b->cur.klm;

    char sisa[MAX_KOLOM];
    int j;
    for (j = 0; j < MAX_KOLOM; j++) sisa[j] = '\0';
    for (j = kolom; j < MAX_KOLOM; j++)
        sisa[j - kolom] = b->text[baris][j];
    for (j = kolom; j < MAX_KOLOM; j++)
        b->text[baris][j] = '\0';

    int i;
    for (i = MAX_BARIS - 1; i > baris + 1; i--)
    {
        b->barisAda[i] = b->barisAda[i - 1];
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = b->text[i - 1][j];
    }

    b->barisAda[baris + 1] = BARIS_ENTER; // tandai sebagai Enter eksplisit
    for (j = 0; j < MAX_KOLOM; j++)
        b->text[baris + 1][j] = sisa[j];

    b->cur.brs++;
    b->cur.klm = 0;
}

void resetDisplayState(void) {}

/* =====================================================
   TAMPILKAN BUFFER
   ===================================================== */
void displayBuffer(Buffer *b)
{
    int i, j;

    int barisAkhir = b->cur.brs;
    for (i = MAX_BARIS - 1; i > barisAkhir; i--)
        if (b->barisAda[i]) { barisAkhir = i; break; }

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