#include <stdio.h>
#include <windows.h>
#include "render.h"
#include "buffer.h"

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}

static void clearScreen(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE)
    {
        DWORD mode;
        if (GetConsoleMode(hConsole, &mode))
        {
            SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
                DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
                COORD homeCoord = {0, 0};
                DWORD written;

                FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoord, &written);
                FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoord, &written);
                SetConsoleCursorPosition(hConsole, homeCoord);
                return;
            }
        }
    }

    printf("\x1b[2J\x1b[H");
    fflush(stdout);
}

void renderEditor(Buffer *b)
{
    clearScreen();

    printf("===============================================================================\n");
    printf("  MODE KETIK | ESC=menu | Panah=gerak | Backspace=hapus | Enter=baris baru  \n");
    printf("===============================================================================\n\n");

    displayBuffer(b);
}

static void bersihkanSisaBaris(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        DWORD written;
        DWORD length = csbi.dwSize.X - csbi.dwCursorPosition.X;
        FillConsoleOutputCharacter(hConsole, ' ', length, csbi.dwCursorPosition, &written);
        return;
    }

    printf("\x1b[K");
    fflush(stdout);
}

void displayBuffer(Buffer *b)
{
    NodeBaris *node = b->kepala;
    int i = 0;
    int barisAkhir = b->jumlah - 1;

    while (node != NULL && i <= barisAkhir)
    {
        char cetak[1200];
        int c_idx = 0;
        int panjang = panjangNode(node);
        int j;

        for (j = 0; j <= panjang; j++)
        {
            if (i == b->cur.brs && j == b->cur.klm)
            {
                cetak[c_idx++] = '|';
            }
            if (j == panjang) break;
            cetak[c_idx++] = node->teks[j];
        }
        cetak[c_idx] = '\0';

        printf("%s", cetak);
        bersihkanSisaBaris();
        printf("\n");

        node = node->berikut;
        i++;
    }
}

