#include <stdio.h>
#include <string.h>
#include "searchfile.h"
#include "buffer.h"

/* === CARI KATA ===
   Minta user input kata, lalu cari di semua baris buffer.
   Kalau ketemu, tampilkan di baris keberapa. */
void CariKata(Buffer *b)
{
    char kata[50];

    // buang sisa newline dulu supaya fgets tidak langsung skip
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("Masukkan kata yang ingin dicari: ");
    fgets(kata, sizeof(kata), stdin);
    kata[strcspn(kata, "\n")] = '\0'; // hapus newline dari fgets

    if (strlen(kata) == 0)
    {
        printf(">> Kata tidak boleh kosong!\n");
        return;
    }

    int ditemukan = 0;
    int i, k;

    for (i = 0; i < MAX_BARIS; i++)
    {
        // salin isi baris ke string sementara supaya bisa pakai strstr
        char barisBuf[MAX_KOLOM + 1];
        k = 0;
        while (k < MAX_KOLOM && b->text[i][k] != '\0')
        {
            barisBuf[k] = b->text[i][k];
            k++;
        }
        barisBuf[k] = '\0';

        // cari kata di baris ini
        if (k > 0 && strstr(barisBuf, kata) != NULL)
        {
            ditemukan = 1;
            printf(">> Kata '%s' DITEMUKAN di baris %d!\n", kata, i + 1);
        }
    }

    if (!ditemukan)
        printf(">> Kata '%s' tidak ditemukan di dokumen.\n", kata);
}