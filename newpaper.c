#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "newpaper.h"
#include "buffer.h"

void JalankanFiturNew(Buffer *b)
{

    bebaskanBuffer(b);

    initBuffer(b);

    printf("\n>> Lembar kerja baru siap digunakan!\n");
}