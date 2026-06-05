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

        fprintf(file, "%s", cur->teks);

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

    bebaskanBuffer(b);
    initBuffer(b);

    char barisTeks[MAX_KOLOM];
    NodeBaris *indeksNode = NULL;

    while (fgets(barisTeks, sizeof(barisTeks), file) != NULL)
    {

        barisTeks[strcspn(barisTeks, "\r\n")] = '\0';

        if (b->kepala->teks[0] == '\0' && b->jumlah == 1 && indeksNode == NULL)
        {

            memset(b->kepala->teks, 0, MAX_KOLOM);
            strcpy(b->kepala->teks, barisTeks);

            b->kepala->barisAda = BARIS_ENTER;
            indeksNode = b->kepala;
        }
        else
        {

            NodeBaris *baru = (NodeBaris *)malloc(sizeof(NodeBaris));
            if (baru != NULL)
            {
                memset(baru->teks, 0, MAX_KOLOM);
                strcpy(baru->teks, barisTeks);

                baru->barisAda = BARIS_ENTER;
                baru->berikut = NULL;

                indeksNode->berikut = baru;
                indeksNode = baru;
                b->jumlah++;
            }
        }
    }

    b->cur.brs = 0;
    b->cur.klm = 0;

    fclose(file);
    printf("Berhasil! File '%s' sukses dimuat ke editor.\n", namaFile);
    printf("============================================\n");
}