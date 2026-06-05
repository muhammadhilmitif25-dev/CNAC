#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "fileio.h"

void SimpanKeFile(Buffer *b)
{
    char namaFile[100];
    printf("\n============================================\n");
    printf("               SIMPAN FILE                  \n");
    printf("============================================\n");
    printf("Masukkan nama file (contoh: dokumen.txt): ");
    scanf("%99s", namaFile);

    FILE *file = fopen(namaFile, "w");
    if (file == NULL)
    {
        printf("ERROR: Gagal membuat atau membuka file!\n");
        return;
    }

    NodeBaris *cur = b->kepala;
    while (cur != NULL)
    {
        // Tulis teks dari node ke file
        fprintf(file, "%s", cur->teks);
        
        // Karena 1 node = 1 baris sejati, langsung kasih \n di setiap akhir node
        fprintf(file, "\n");
        
        cur = cur->berikut;
    }

    fclose(file);
    printf("Berhasil! Dokumen telah disimpan di '%s'.\n", namaFile);
    printf("============================================\n");
}

void SaveAs(Buffer *b)
{
    SimpanKeFile(b);
}

void BukaDariFile(Buffer *b)
{
    char namaFile[100];
    printf("\n============================================\n");
    printf("               BUKA FILE                    \n");
    printf("============================================\n");
    printf("Masukkan nama file yang ingin dibuka: ");
    scanf("%99s", namaFile);

    FILE *file = fopen(namaFile, "r");
    if (file == NULL)
    {
        printf("ERROR: File '%s' tidak ditemukan!\n", namaFile);
        return;
    }

    // Bersihkan buffer lama sebelum memuat file baru
    bebaskanBuffer(b);
    initBuffer(b);

    char barisTeks[MAX_KOLOM];
    NodeBaris *indeksNode = NULL;

    while (fgets(barisTeks, sizeof(barisTeks), file) != NULL)
    {
        // Hilangkan karakter newline (\n atau \r) bawaan file di ujung string
        barisTeks[strcspn(barisTeks, "\r\n")] = '\0';

        if (b->kepala->teks[0] == '\0' && b->jumlah == 1 && indeksNode == NULL)
        {
            // Isi node pertama jika masih kosong bawaan initBuffer
            strcpy(b->kepala->teks, barisTeks);
            indeksNode = b->kepala;
        }
        else
        {
            // Buat node baru untuk baris selanjutnya
            NodeBaris *baru = (NodeBaris *)malloc(sizeof(NodeBaris));
            if (baru != NULL)
            {
                strcpy(baru->teks, barisTeks);
                baru->berikut = NULL;
                indeksNode->berikut = baru;
                indeksNode = baru;
                b->jumlah++;
            }
        }
    }

    fclose(file);
    printf("Berhasil! File '%s' sukses dimuat ke editor.\n", namaFile);
    printf("============================================\n");
}