#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h> 
#include "render.h"
#include "cursor.h"
#include "buffer.h"
#include "newpaper.h"
#include "fileio.h"
#include "searchfile.h"


int main()
{
    Buffer myBuffer;
    initBuffer(&myBuffer);
    int menu;

    while (1)
    {
        system("cls"); // Hapus layar khusus di menu utama saja

        printf("============================================\n");
        printf("       NOTEPAD CONSOLE - CNAC EDITION       \n");
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
            while (getchar() != '\n');
            continue;
        }

        if (menu == 1)
        {
            resetDisplayState();
            system("cls"); // Bersihkan layar sekali sebelum mulai mengetik

            
            while (1)
            {
                
             renderEditor(&myBuffer);
                
                fflush(stdout);
                int c = getch();

                if (c == 27) { // ESC keluar
                    break;
                }
                else if (c == 224) { // Deteksi tombol panah navigasi
                    int arah = getch();
                    if (arah == 75) movekiri(&myBuffer); 
                    else if (arah == 77) movekanan(&myBuffer); 
                    else if (arah == 72) moveatas(&myBuffer); 
                    else if (arah == 80) movebawah(&myBuffer); 
                }
                else if (c == 8) { // Backspace
                    deleteChar(&myBuffer);
                }
                else if (c == 13) { // Enter
                    newLine(&myBuffer);
                }
                else if (c >= 32 && c <= 126) { // Karakter teks biasa
                    insertChar(&myBuffer, (char)c);
                }
            }
        }
        else if (menu == 2)
        {
            SimpanKeFile(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit()) getch(); 
            while (getch() != 13); 
        }
        else if (menu == 3)
        {
            SaveAs(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit()) getch();
            while (getch() != 13);
        }
        else if (menu == 4)
        {
            BukaDariFile(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit()) getch();
            while (getch() != 13);
        }
        else if (menu == 5)
        {
            CariKata(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit()) getch();
            while (getch() != 13);
        }
        else if (menu == 6)
        {
            JalankanFiturNew(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit()) getch();
            while (getch() != 13);
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