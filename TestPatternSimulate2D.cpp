#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "TestPatternSimulate2D.h"

/// @brief Simulate T2001JA 2D TPG data
/// @param width 1~832
/// @param height 1~800
/// @param pData Need to allocate enough memory space by the user
void TestPatternSimulate2D(const int width, const int height, unsigned short *pData)
{
    const int w = width <= 832 ? width : 832;
    const int h = width <= 800 ? width : 800;
    int row = 0;
    int col = 0;
    for (int j = 0; j < h; j++)
    {
        row = col;
        for (int i = 0; i < w; i++)
        {
            pData[i + j * width] = row;
            row++;
            if (row > 1023)
                row = 0;
        }
        col++;
        if (col > 1023)
            col = 0;
    }
}

/// @brief Simulate T2001JA 2D-HDR TPG data
/// @param width 1~832
/// @param height 1~800
/// @param pData Need to allocate enough memory space by the user
void TestPatternSimulate2DHDR(const int width, const int height, unsigned short *pData)
{
    const int w = width <= 832 ? width : 832;
    const int h = width <= 800 ? width : 800;
    TestPatternSimulate2D(w, h, pData);
    for (int i = 0; i < w * h; i++)
    {
        pData[i] = ~pData[i];
    }
}
