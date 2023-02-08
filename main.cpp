#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "bitmap.h"
#include "TestPatternSimulate2D.h"
#include "ConvolutionMaskSimulate2D.h"

// #define TPG
#define CONV_MSK

#ifdef TPG
int main(int argc, char *argv[])
{
    std::cout << "Test Pattern Simulate Start ..." << std::endl;

    std::cout << "executable path: " << argv[0] << std::endl;
    std::filesystem::path p = argv[0];
    std::cout << "executable file parent path: " << p.parent_path().string() << std::endl;

    const int width = 832;
    const int height = 800;
    unsigned short *pData = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
    unsigned char *u8_ptr = (unsigned char *)malloc(sizeof(unsigned char) * width * height);
    bitmap_t bitmap = {0, 0, 0, NULL};

    /* 2D TPG */
    TestPatternSimulate2D(width, height, pData);
    for (int i = 0; i < width * height; i++)
    {
        u8_ptr[i] = pData[i] / 4;
    }

    bitmap.width = width;
    bitmap.height = height;
    bitmap.bits = 8;
    bitmap.data = u8_ptr;
    BitmapWrite(&bitmap, p.parent_path().string().append("\\tpg_2d.bmp").c_str());

    /* 2D-HDR TPG */
    TestPatternSimulate2DHDR(width, height, pData);
    for (int i = 0; i < width * height; i++)
    {
        u8_ptr[i] = pData[i] / 4;
    }

    bitmap.width = width;
    bitmap.height = height;
    bitmap.bits = 8;
    bitmap.data = u8_ptr;
    BitmapWrite(&bitmap, p.parent_path().string().append("\\tpg_2d-hdr.bmp").c_str());

    std::cout << "Test Pattern Simulate End" << std::endl;
}
#endif

#ifdef CONV_MSK
int main(int argc, char *argv[])
{
    std::cout << "Convolution Mask Simulate Start ..." << std::endl;

    std::cout << "executable path: " << argv[0] << std::endl;
    std::filesystem::path p = argv[0];
    std::cout << "executable file parent path: " << p.parent_path().string() << std::endl;

    const int width = 832;
    const int height = 800;
    unsigned short *pData = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
    unsigned short *pData2 = (unsigned short *)malloc(sizeof(unsigned short) * width * height);
    unsigned char *u8_ptr = (unsigned char *)malloc(sizeof(unsigned char) * width * height);
    bitmap_t bitmap = {0, 0, 0, NULL};

    /* 2D CONV-MSK */
    std::vector<std::vector<int>> kernel_7x7 = {{-1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, 24, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1}};

    TestPatternSimulate2D(width, height, pData);
    ConvolutionMaskSimulate(width, height, pData, pData2, 0, kernel_7x7);
    for (int i = 0; i < width * height; i++)
    {
        u8_ptr[i] = pData2[i] / 4;
    }

    bitmap.width = width;
    bitmap.height = height;
    bitmap.bits = 8;
    bitmap.data = u8_ptr;
    BitmapWrite(&bitmap, p.parent_path().string().append("\\conv-msk_2d.bmp").c_str());

    /* 2D-HDR CONV-MSK */
    std::vector<std::vector<int>> kernal_5x5 = {{-1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1},
                                                {-1, -1, 24, -1, -1},
                                                {-1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1}};

    TestPatternSimulate2DHDR(width, height, pData);
    ConvolutionMaskSimulate(width, height, pData, pData2, 0, kernal_5x5);
    for (int i = 0; i < width * height; i++)
    {
        u8_ptr[i] = pData2[i] / 4;
    }

    bitmap.width = width;
    bitmap.height = height;
    bitmap.bits = 8;
    bitmap.data = u8_ptr;
    BitmapWrite(&bitmap, p.parent_path().string().append("\\conv-msk_2d-hdr.bmp").c_str());

    std::cout << "Convolution Mask Simulate End" << std::endl;
    return 0;
}
#endif