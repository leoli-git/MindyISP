// 圖像結構
typedef struct bitmap_t
{
    unsigned int width, height;
    unsigned short bits;
    unsigned char *data;
} bitmap_t;

void BitmapWrite(const bitmap_t *_this, const char *name);