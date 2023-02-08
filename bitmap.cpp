#include <iostream>
#include <stdlib.h>

#include "bitmap.h"

// 檔案結構
#pragma pack(2)
struct BitmapFileHeader
{
    unsigned short bfTybe;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};
struct BitmapInfoHeader
{
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes; // 1=defeaul, 0=custom
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPelsPerMeter; // 72dpi=2835, 96dpi=3780
    unsigned int biYPelsPerMeter; // 120dpi=4724, 300dpi=11811
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};
#pragma pack()

void Write2Bitmap(const char *name, const unsigned char *raw_img,
                  unsigned int width, unsigned int height, unsigned short bits)
{
    if (!(name && raw_img))
    {
        perror("Error Write2Bitmap.");
        return;
    }
    // 檔案資訊
    struct BitmapFileHeader file_h = {
        .bfTybe = 0x4d42,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = 54,
    };
    file_h.bfSize = file_h.bfOffBits + width * height * bits / 8;
    if (bits == 8)
    {
        file_h.bfSize += 1024, file_h.bfOffBits += 1024;
    }
    // 圖片資訊
    struct BitmapInfoHeader info_h = {
        .biSize = 40,
        .biPlanes = 1,
        .biCompression = 0,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = 0,
        .biClrImportant = 0,
    };
    info_h.biWidth = width;
    info_h.biHeight = height;
    info_h.biBitCount = bits;
    info_h.biSizeImage = width * height * bits / 8;
    if (bits == 8)
    {
        info_h.biClrUsed = 256;
    }
    // 寫入檔頭
    FILE *pFile = NULL;
    // pFile = fopen(name,"wb+");
    fopen_s(&pFile, name, "wb+");
    if (!pFile)
    {
        perror("Error opening file.");
        return;
    }
    fwrite((char *)&file_h, sizeof(char), sizeof(file_h), pFile);
    fwrite((char *)&info_h, sizeof(char), sizeof(info_h), pFile);
    // 寫調色盤
    if (bits == 8)
    {
        for (unsigned i = 0; i < 256; ++i)
        {
            unsigned char c = i;
            fwrite((char *)&c, sizeof(char), sizeof(unsigned char), pFile);
            fwrite((char *)&c, sizeof(char), sizeof(unsigned char), pFile);
            fwrite((char *)&c, sizeof(char), sizeof(unsigned char), pFile);
            fwrite("", sizeof(char), sizeof(unsigned char), pFile);
        }
    }
    // 寫入圖片資訊
    size_t alig = ((width * bits / 8) * 3) % 4;
    for (int j = height - 1; j >= 0; --j)
    {
        for (unsigned i = 0; i < width; ++i)
        {
            unsigned int idx = j * width + i;
            if (bits == 24)
            { // RGB圖片
                fwrite((char *)&raw_img[idx * 3 + 2],
                       sizeof(char), sizeof(unsigned char), pFile);
                fwrite((char *)&raw_img[idx * 3 + 1],
                       sizeof(char), sizeof(unsigned char), pFile);
                fwrite((char *)&raw_img[idx * 3 + 0],
                       sizeof(char), sizeof(unsigned char), pFile);
            }
            else if (bits == 8)
            { // 灰階圖
                fwrite((char *)&raw_img[idx],
                       sizeof(char), sizeof(unsigned char), pFile);
            }
        }
        // 對齊4byte
        for (size_t i = 0; i < alig; ++i)
        {
            fwrite("", sizeof(char), sizeof(unsigned char), pFile);
        }
    }
    fclose(pFile);
}

void BitmapWrite(const bitmap_t *_this, const char *name)
{
    const bitmap_t *p = _this;
    Write2Bitmap(name, p->data, p->width, p->height, p->bits);
}