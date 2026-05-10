#ifndef FILEIO_H
#define FILEIO_H

#include "buffer.h"

extern char namaFileDefault[100];

void SimpanKeFile(Buffer *b);
void SaveAs(Buffer *b);
void BukaDariFile(Buffer *b);

#endif