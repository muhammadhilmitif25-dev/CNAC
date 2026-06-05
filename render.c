#include <stdio.h>
#include <windows.h>
#include "render.h"
#include "buffer.h"

#define BARIS_HEADER 4

void gotoxy(int x, int y)
{
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static void clearScreen(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(h, &csbi))
        return;
    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    COORD home = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(h, ' ', cellCount, home, &written);
    FillConsoleOutputAttribute(h, csbi.wAttributes, cellCount, home, &written);
    SetConsoleCursorPosition(h, home);
}

static void hapusSisaBaris(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(h, &csbi))
        return;
    DWORD sisa = csbi.dwSize.X - csbi.dwCursorPosition.X;
    DWORD written;
    FillConsoleOutputCharacter(h, ' ', sisa, csbi.dwCursorPosition, &written);
    FillConsoleOutputAttribute(h, csbi.wAttributes, sisa, csbi.dwCursorPosition, &written);
}

static void hapusSampaiAkhirLayar(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(h, &csbi))
        return;
    DWORD sisa = (DWORD)(csbi.dwSize.X - csbi.dwCursorPosition.X) + (DWORD)(csbi.srWindow.Bottom - csbi.dwCursorPosition.Y) * csbi.dwSize.X;
    DWORD written;
    FillConsoleOutputCharacter(h, ' ', sisa, csbi.dwCursorPosition, &written);
    FillConsoleOutputAttribute(h, csbi.wAttributes, sisa, csbi.dwCursorPosition, &written);
}

void displayBuffer(Buffer *b)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);

    int tinggiWindow = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int maksBaris = tinggiWindow - BARIS_HEADER - 1;
    if (maksBaris < 1)
        maksBaris = 1;

    int scrollOffset = 0;
    if (b->cur.brs >= maksBaris)
        scrollOffset = b->cur.brs - maksBaris + 1;

    NodeBaris *node = b->kepala;
    int i = 0;
    while (node != NULL && i < scrollOffset)
    {
        node = node->berikut;
        i++;
    }

    int barisDicetak = 0;
    while (node != NULL && barisDicetak < maksBaris)
    {
        int barisAsli = i;
        int panjang = panjangNode(node);
        int j;

        for (j = 0; j <= panjang; j++)
        {
            if (barisAsli == b->cur.brs && j == b->cur.klm)
                putchar('|');
            if (j < panjang)
                putchar(node->teks[j]);
        }

        hapusSisaBaris();
        putchar('\n');

        node = node->berikut;
        i++;
        barisDicetak++;
    }

    hapusSampaiAkhirLayar();
}

void renderEditor(Buffer *b)
{

    gotoxy(0, 0);

    printf("===============================================================================\n");
    printf("  MODE KETIK | ESC=menu | Panah=gerak | Backspace=hapus | Enter=baris baru  \n");
    printf("===============================================================================\n\n");

    displayBuffer(b);
}