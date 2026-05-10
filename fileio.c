#include <stdio.h>
#include <string.h>
#include "fileio.h"
#include "buffer.h"

// nama file yang sedang aktif dipakai (default: catatan.txt)
char namaFileDefault[100] = "catatan.txt";

/* === SIMPAN KE FILE ===
   Tulis semua isi buffer ke file teks. */
void SimpanKeFile(Buffer *b)
{
    FILE *fptr = fopen(namaFileDefault, "w");
    if (fptr == NULL)
    {
        printf(">> Gagal membuka file untuk disimpan!\n");
        return;
    }

    int i, j;
    for (i = 0; i < MAX_BARIS; i++)
    {
        // cek apakah baris ini ada isinya
        int adaIsi = 0;
        for (j = 0; j < MAX_KOLOM; j++)
            if (b->text[i][j] != '\0') { adaIsi = 1; break; }

        if (adaIsi)
        {
            // tulis karakter satu per satu sampai ketemu null
            for (j = 0; j < MAX_KOLOM; j++)
            {
                if (b->text[i][j] == '\0') break;
                fprintf(fptr, "%c", b->text[i][j]);
            }
            fprintf(fptr, "\n");
        }
        else
        {
            // cek apakah masih ada baris berisi di bawahnya
            int adaBawah = 0;
            int ii, jj;
            for (ii = i + 1; ii < MAX_BARIS && !adaBawah; ii++)
                for (jj = 0; jj < MAX_KOLOM; jj++)
                    if (b->text[ii][jj] != '\0') { adaBawah = 1; break; }

            if (adaBawah) fprintf(fptr, "\n"); // simpan baris kosong kalau masih ada isi di bawah
        }
    }

    fclose(fptr);
    printf(">> Berhasil disimpan ke '%s'\n", namaFileDefault);
}

/* === SAVE AS ===
   Simpan dengan nama file baru yang diinput user. */
void SaveAs(Buffer *b)
{
    // buang sisa newline di input buffer dulu
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("Masukkan nama file baru (contoh: dokumen.txt): ");
    fgets(namaFileDefault, sizeof(namaFileDefault), stdin);

    // hapus karakter newline dari fgets
    namaFileDefault[strcspn(namaFileDefault, "\n")] = '\0';

    if (strlen(namaFileDefault) == 0)
    {
        printf(">> Nama file tidak boleh kosong!\n");
        return;
    }

    SimpanKeFile(b);
}

/* === BUKA DARI FILE ===
   Minta user input nama file, lalu muat isinya ke buffer. */
void BukaDariFile(Buffer *b)
{
    char namaInput[100];

    // buang sisa newline di input buffer dulu
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("Masukkan nama file yang ingin dibuka (contoh: catatan.txt): ");
    fgets(namaInput, sizeof(namaInput), stdin);
    namaInput[strcspn(namaInput, "\n")] = '\0';

    if (strlen(namaInput) == 0)
    {
        printf(">> Nama file tidak boleh kosong!\n");
        return;
    }

    strncpy(namaFileDefault, namaInput, sizeof(namaFileDefault) - 1);

    FILE *fptr = fopen(namaFileDefault, "r");
    if (fptr == NULL)
    {
        printf(">> File '%s' tidak ditemukan!\n", namaFileDefault);
        return;
    }

    // reset buffer dulu sebelum diisi
    int i, j;
    for (i = 0; i < MAX_BARIS; i++)
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = '\0';
    b->cur.brs = 0;
    b->cur.klm = 0;

    // baca file baris per baris dan masukkan ke buffer
    char lineBuf[MAX_KOLOM + 2];
    int baris = 0;
    while (baris < MAX_BARIS && fgets(lineBuf, sizeof(lineBuf), fptr) != NULL)
    {
        int k = 0;
        for (j = 0; lineBuf[j] != '\0' && lineBuf[j] != '\n' && k < MAX_KOLOM; j++, k++)
            b->text[baris][k] = lineBuf[j];
        baris++;
    }

    fclose(fptr);
    printf(">> File '%s' berhasil dimuat!\n", namaFileDefault);
}