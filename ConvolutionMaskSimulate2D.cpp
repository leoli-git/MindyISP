#include <algorithm>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "ConvolutionMaskSimulate2D.h"

/// @brief
/// @param width image width
/// @param height image height
/// @param pIn input image array
/// @param pOut output image array
/// @param rshift 1 / 2^rshift
/// @param kernel
void ConvolutionMaskSimulate(const int width, const int height, const unsigned short *pIn, unsigned short *pOut, const unsigned char rshift, std::vector<std::vector<int>> kernel)
{
    const int ksize = kernel.size();
    const int dx = ksize / 2;
    const int dy = ksize / 2;

    /* x-flip */
    for (int j = 0; j < ksize; j++)
    {
        for (int i = 0; i < dx; i++)
        {
            int tmp = kernel[j][ksize - 1 - i];
            kernel[j][ksize - 1 - i] = kernel[j][i];
            kernel[j][i] = tmp;
        }
    }

    /* y-flip */
    for (int i = 0; i < ksize; i++)
    {
        for (int j = 0; j < dy; j++)
        {
            int tmp = kernel[ksize - 1 - j][i];
            kernel[ksize - 1 - j][i] = kernel[j][i];
            kernel[j][i] = tmp;
        }
    }

    /* convolution calculation */
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            /* kernel operation */
            int tmp = 0;
            for (int k = 0; k < ksize; k++)
            {
                for (int l = 0; l < ksize; l++)
                {
                    int x = i - dx + l;
                    int y = j - dy + k;
                    /* edge */
                    if (x >= 0 && x < width && y >= 0 && y < height)
                        tmp += pIn[x + y * width] * kernel[k][l];
                    else
                    {
                        /* TODO: how to process edge data */
                    }
                }
            }
            tmp = tmp / (std::pow(2, rshift));
            tmp = std::min(1023, tmp);
            tmp = std::max(0, tmp);
            pOut[i + j * width] = (unsigned short)(tmp & 0x3ff);
        }
    }
}
