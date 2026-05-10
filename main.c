#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "cursor.h"
#include "buffer.h"
#include "editor.h"
#include "fileio.h"
#include "searchfile.h"

int main()
{
    Buffer myBuffer;
    initBuffer(&myBuffer);

    int menu;

    while (1)
    {
        system("cls");

        printf("============================================\n");
        printf("       NOTEPAD CONSOLE - CKA EDITION       \n");
        printf("============================================\n\n");
        printf("  ISI DOKUMEN:\n");
        printf("  ------------\n");
        displayBuffer(&myBuffer);
        printf("\n============================================\n");
        printf("  1. Mode Ketik\n");
        printf("  2. Simpan File (Save)\n");
        printf("  3. Simpan Sebagai (Save As)\n");
        printf("  4. Buka File (Open)\n");
        printf("  5. Cari Kata (Search)\n");
        printf("  6. Lembar Baru (New)\n");
        printf("  7. Keluar\n");
        printf("============================================\n");
        printf("  Pilih menu (1-7): ");

        if (scanf(" %d", &menu) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }

        if (menu == 1)
        {
            resetDisplayState();

            while (1)
            {
                system("cls");
                printf("============================================\n");
                printf("  MODE KETIK | ESC=menu | Panah=gerak      \n");
                printf("  Backspace=hapus | Enter=baris baru       \n");
                printf("============================================\n\n");

                displayBuffer(&myBuffer);

                printf("\n--------------------------------------------\n");
                printf("[ ESC = kembali ke menu ]\n");
                fflush(stdout);

                int c = getch();

                if (c == 27) // ESC
                {
                    break;
                }
                else if (c == 224) // tombol panah (prefix dari getch di Windows)
                {
                    int arah = getch();
                    if (arah == 75)
                        movekiri(&myBuffer); // panah kiri
                    else if (arah == 77)
                        movekanan(&myBuffer); // panah kanan
                    else if (arah == 72)
                        moveatas(&myBuffer); // panah atas
                    else if (arah == 80)
                        movebawah(&myBuffer); // panah bawah
                }
                else if (c == 8) // Backspace
                {
                    deleteChar(&myBuffer);
                }
                else if (c == 13) // Enter
                {
                    newLine(&myBuffer);
                }
                else if (c >= 32 && c <= 126) // karakter biasa yang bisa dicetak
                {
                    insertChar(&myBuffer, (char)c);
                }
            }
        }
        else if (menu == 2)
        {
            SimpanKeFile(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit())
                getch(); // Bersihkan buffer
            while (getch() != 13)
                ; // Tunggu ENTER (ASCII 13)
        }
        else if (menu == 3)
        {
            SaveAs(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit())
                getch();
            while (getch() != 13)
                ;
        }
        else if (menu == 4)
        {
            BukaDariFile(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit())
                getch();
            while (getch() != 13)
                ;
        }
        else if (menu == 5)
        {
            CariKata(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit())
                getch();
            while (getch() != 13)
                ;
        }
        else if (menu == 6)
        {
            JalankanFiturNew(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit())
                getch();
            while (getch() != 13)
                ;
        }
        else if (menu == 7)
        {
            printf("\nSampai jumpa!\n");
            break;
        }
        else
        {
            printf("\nPilihan tidak valid, coba lagi.\n");
            system("pause");
        }
    }

    return 0;
}