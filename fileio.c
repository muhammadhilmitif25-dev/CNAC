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
    {
        b->barisAda[i] = 0; // reset barisAda juga!
        for (j = 0; j < MAX_KOLOM; j++)
            b->text[i][j] = '\0';
    }
    b->cur.brs = 0;
    b->cur.klm = 0;
    b->barisAda[0] = 2; // baris 0 selalu ada (pakai nilai BARIS_ENTER=2 supaya dianggap ada)

    // baca file baris per baris dan masukkan ke buffer
    // kalau satu baris file lebih panjang dari MAX_KOLOM-1,
    // pecah jadi beberapa baris wrap (barisAda=1) secara otomatis
    char lineBuf[MAX_KOLOM * MAX_BARIS]; // ikut MAX dari buffer.h
    int baris = 0;
    int pertamaDiBaris = 1; // apakah ini baris pertama dari satu baris file
    while (baris < MAX_BARIS && fgets(lineBuf, sizeof(lineBuf), fptr) != NULL)
    {
        int pos = 0;
        int panjangLine = 0;
        while (lineBuf[panjangLine] != '\0' && lineBuf[panjangLine] != '\n')
            panjangLine++;

        pertamaDiBaris = 1;
        while (pos < panjangLine && baris < MAX_BARIS)
        {
            int ambil = panjangLine - pos;
            if (ambil > MAX_KOLOM - 1) ambil = MAX_KOLOM - 1;

            for (j = 0; j < ambil; j++)
                b->text[baris][j] = lineBuf[pos + j];

            // baris pertama dari satu baris file = BARIS_ENTER (2)
            // baris lanjutan (wrap) = BARIS_WRAP (1)
            b->barisAda[baris] = pertamaDiBaris ? 2 : 1;
            pertamaDiBaris = 0;
            pos += ambil;
            baris++;
        }

        // kalau baris file kosong (hanya newline), tetap buat satu baris kosong
        if (panjangLine == 0 && baris < MAX_BARIS)
        {
            b->barisAda[baris] = 2;
            baris++;
        }
    }

    fclose(fptr);
    printf(">> File '%s' berhasil dimuat!\n", namaFileDefault);
}