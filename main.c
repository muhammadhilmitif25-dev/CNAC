#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "cursor.h"
#include "buffer.h"
#include "newpaper.h"
#include "fileio.h"
#include "searchfile.h"
#include "render.h"
#include <windows.h>

static void runEditor(Buffer *b)
{
    resetDisplayState();

    while (1)
    {
        renderEditor(b);

        int c = getch();
        if (c == 27)
            break;
        else if (c == 224)
        {
            int arah = getch();
            if (arah == 75)
                movekiri(b);
            else if (arah == 77)
                movekanan(b);
            else if (arah == 72)
                moveatas(b);
            else if (arah == 80)
                movebawah(b);
        }
        else if (c == 8)
            deleteChar(b);
        else if (c == 13)
            newLine(b);
        else if (c >= 32 && c <= 126)
            insertChar(b, (char)c);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
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
            runEditor(&myBuffer);
        }
        else if (menu == 2)
        {
            SimpanKeFile(&myBuffer);
            printf("\nTekan Enter untuk kembali ke menu...");
            while (kbhit())
                getch();
            while (getch() != 13)
                ;
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

            system("cls");
            runEditor(&myBuffer);
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

    bebaskanBuffer(&myBuffer);
    return 0;
}