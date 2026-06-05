#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "buffer.h"

void CariKata(Buffer *b)
{
    char kata[100];
    printf("\n============================================\n");
    printf("              FITUR CARI KATA               \n");
    printf("============================================\n");
    printf("Masukkan kata yang ingin dicari: ");
    scanf("%99s", kata);

    NodeBaris *node = b->kepala;
    int indeksBaris = 0;
    int ketemu = 0;

    printf("\nHasil Pencarian:\n");
    printf("--------------------------------------------\n");

    while (node != NULL)
    {

        if (panjangNode(node) > 0)
        {
            char *posisi = strstr(node->teks, kata);
            if (posisi != NULL)
            {
                int indeksKolom = posisi - node->teks;
                printf("[Ketemu] Baris %d, Kolom %d: %s\n", indeksBaris + 1, indeksKolom + 1, node->teks);
                ketemu = 1;
            }
        }
        node = node->berikut;
        indeksBaris++;
    }

    if (!ketemu)
    {
        printf("Kata '%s' tidak ditemukan dalam dokumen.\n", kata);
    }
    printf("============================================\n");
}