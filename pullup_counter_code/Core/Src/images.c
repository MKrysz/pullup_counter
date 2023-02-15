
#include "images.h"
#include "ssd1306.h"

#define NEEDLE_SIZE 21

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param image 
 */
void Image_Draw(uint8_t x, uint8_t y, image_t image)
{
    for(size_t i = 0; i<image.size; i++){
        uint8_t tempX, tempY;
        tempX = image.data[i*2];
        tempY = image.data[i*2+1];
        ssd1306_DrawPixel(x+tempX, y+tempY, White);
    }
}


static const uint8_t img_meter_data[] = {
    0,26,  0,25,  0,24,  0,23,  0,22,  0,21,  1,20,  1,19,  1,18,  1,17,  2,16,  2,15,  
    2,14,  3,13,  3,12,  3,11,  4,10,  4,9,   5,8,   5,7,   6,6,   7,5,   8,5,   9,4,   
    10,4,  11,3,  12,3,  13,3,  14,2,  15,2,  16,2,  17,1,  18,1,  19,1,  20,1,  21,0, 
    22,0,  23,0,  24,0,  25,0,  26,0,  27,0,  28,0,  29,1,  30,1,  31,1,  32,1,  33,2, 
    34,2,  35,2,  36,3,  37,3,  38,3,  39,4,  40,4,  41,5,  42,5,  43,6,  44,7,  44,8,  
    45,9,  45,10, 46,11, 46,12, 46,13, 47,14, 47,15, 47,16, 48,17, 48,18, 48,19, 48,20, 
    49,21, 49,22, 49,23, 49,24, 49,25, 49,26, 23,25, 24,25, 24,24, 25,24, 25,25, 26,25,
    0,26,  1,26,  2,26,  3,26,  4,26,  5,26,  6,26,  7,26,  8,26,  9,26,  10,26, 11,26, 
    12,26, 13,26, 14,26, 15,26, 16,26, 17,26, 18,26, 19,26, 20,26, 21,26, 22,26, 23,26, 
    24,26, 25,26, 26,26, 27,26, 28,26, 29,26, 30,26, 31,26, 32,26, 33,26, 34,26, 35,26, 
    36,26, 37,26, 38,26, 39,26, 40,26, 41,26, 42,26, 43,26, 44,26, 45,26, 46,26, 47,26, 
    48,26, 49,26
};

image_t img_meter = {
    .data = img_meter_data,
    .Xsize = 50,
    .Ysize = 27,
    .size = sizeof(img_meter_data)/2
};


static const uint8_t img_clk_data[] = {
    5,12, 6,12, 7,12, 8,11, 9,11, 10,10, 11,9, 11,8, 12,7, 12,6, 12,5, 11,4,
    11,3, 10,2, 9,1,  8,1,  7,0,  6,0,   5,0,  4,1,  3,1,  2,2,  1,3,  1,4,  
    0,5,  0,6,  0,7,  1,8,  1,9,  2,10,  3,11, 4,11, 6,10, 6,9,  6,8,  6,7,  
    6,6,  7,5,  8,4,  9,3
};

image_t img_clk = {
    .data = img_clk_data,
    .Xsize = 12,
    .Ysize = 12,
    .size = sizeof(img_clk_data)/2
};


static const uint8_t img_sd_data[] = {
    0,11,  1,11, 2,11, 3,11, 4,11, 5,11, 6,11, 7,11, 8,11, 9,11, 10,11, 11,11, 
    11,10, 11,9, 11,8, 11,7, 11,6, 11,5, 11,4, 11,3, 10,2, 9,1,  8,0,   7,0, 
    6,0,   5,0,  4,0,  3,0,  2,0,  1,0,  0,0,  0,1,  0,2,  0,3,  0,4,   0,5, 
    0,6,   0,7,  0,8,  0,9,  0,10, 3,8,  4,8,  5,7,  4,6,  3,6,  2,5,   3,4, 
    4,4,   7,4,  7,5,  7,6,  7,7,  7,8,  8,8,  9,7,  9,6,  9,5,  8,4
};

image_t img_sd = {
    .data = img_sd_data,
    .Xsize = 11,
    .Ysize = 11,
    .size = sizeof(img_sd_data)/2
};


static const uint8_t img_proxOn_data[] = {
    0,0, 1,0, 2,1, 1,2, 0,3, 0,2, 0,1, 4,0, 5,0, 6,1, 5,2, 6,3, 
    4,3, 4,2, 4,1, 9,0, 10,0, 11,1, 11,2, 10,3, 9,3, 8,2, 8,1, 13,0, 
    15,0, 14,1, 14,2, 13,3, 15,3, 4,6, 5,6, 6,7, 6,8, 5,9, 4,9, 3,8, 
    3,7, 8,9, 8,8, 8,7, 8,6, 9,7, 10,8, 11,9, 11,8, 11,7, 11,6
};

image_t img_proxOn = {
    .data = img_proxOn_data,
    .Xsize = 15,
    .Ysize = 9,
    .size = sizeof(img_proxOn_data)/2
};


static const uint8_t img_autoUpdate_data[] = {
    0,1, 0,2, 0,3, 1,2, 2,2, 3,2, 3,3, 3,1, 2,0, 1,0, 5,0, 5,1, 
    5,2, 6,3, 7,3, 8,2, 8,1, 8,0, 10,0, 11,0, 12,0, 11,1, 11,2, 11,3, 
    15,0, 16,0, 17,1, 17,2, 16,3, 15,3, 14,2, 14,1, 0,6, 0,7, 0,8, 1,9, 
    2,9, 3,8, 3,7, 3,6, 5,6, 6,6, 7,7, 6,8, 5,9, 5,8, 5,7, 5,6, 
    9,6, 10,6, 11,7, 11,8, 10,9, 9,9, 9,8, 9,7, 13,9, 13,8, 13,7, 14,6, 
    15,6, 16,7, 16,8, 16,9, 15,8, 14,8, 18,6, 19,6, 20,6, 19,7, 19,8, 19,9, 
    23,6, 22,6, 22,7, 22,8, 22,9, 23,9
};

image_t img_autoUpdate = {
    .data = img_autoUpdate_data,
    .Xsize = 23,
    .Ysize = 9,
    .size = sizeof(img_autoUpdate_data)/2
};


static const uint8_t img_smallBattery_data[] = {
    0,9, 1,9, 2,9, 3,9, 4,9, 5,9, 6,9, 6,8, 6,7, 6,6, 6,5, 6,4, 
    6,3, 6,2, 6,1, 5,2, 5,1, 4,1, 3,1, 2,1, 1,1, 0,1, 0,2, 0,3, 
    0,4, 0,5, 0,6, 0,7, 0,8, 0,9, 1,2, 2,0, 3,0, 4,0, 3,3, 3,4, 
    3,5, 4,4, 2,4, 2,7, 3,7, 4,7
};

image_t img_smallBattery = {
    .data = img_smallBattery_data,
    .Xsize = 23,
    .Ysize = 9,
    .size = sizeof(img_smallBattery_data)/2
};

const uint8_t img_meterNeedle_data[][NEEDLE_SIZE*2] = {
    {2,24, 3,24, 4,24, 5,24,  6,24,  7,24,  8,24,  9,24,  10,24, 11,24, 12,24, 13,24, 14,24, 15,24, 16,24, 17,24, 18,24, 19,24, 20,24, 21,24, 22,24},
    {2,23, 3,23, 4,23, 5,23,  6,23,  7,23,  8,23,  9,23,  10,23, 11,23, 12,23, 13,24, 14,24, 15,24, 16,24, 17,24, 18,24, 19,24, 20,24, 21,24, 22,24},
    {2,22, 3,22, 4,22, 5,22,  6,22,  7,22,  8,22,  9,22,  10,23, 11,23, 12,23, 13,23, 14,23, 15,23, 16,23, 17,24, 18,24, 19,24, 20,24, 21,24, 22,24},
    {3,21, 4,21, 5,21, 6,21,  7,21,  8,21,  9,21,  10,22, 11,22, 12,22, 13,22, 14,22, 15,22, 16,22, 17,23, 18,23, 19,23, 20,23, 21,23, 22,23, 23,23},
    {4,20, 5,20, 6,20, 7,20,  8,20,  9,21,  10,21, 11,21, 12,21, 13,21, 14,22, 15,22, 16,22, 17,22, 18,22, 19,23, 20,23, 21,23, 22,23, 23,23, 23,23},
    {3,19, 4,19, 5,19, 6,19,  7,19,  8,20,  9,20,  10,20, 11,20, 12,21, 13,21, 14,21, 15,21, 16,22, 17,22, 18,22, 19,22, 20,23, 21,23, 22,23, 23,23},
    {3,18, 4,18, 5,18, 6,18,  7,19,  8,19,  9,19,  10,19, 11,20, 12,20, 13,20, 14,20, 15,21, 16,21, 17,21, 18,22, 19,22, 20,22, 21,23, 22,23, 23,23},
    {4,17, 5,17, 6,17, 7,18,  8,18,  9,18,  10,19, 11,19, 12,19, 13,20, 14,20, 15,20, 16,21, 17,21, 18,21, 19,22, 20,22, 21,22, 22,23, 23,23, 23,23},
    {4,16, 5,16, 6,17, 7,17,  8,17,  9,18,  10,18, 11,18, 12,19, 13,19, 14,20, 15,20, 16,20, 17,21, 18,21, 19,22, 20,22, 21,22, 22,23, 23,23, 23,23},
    {4,15, 5,15, 6,16, 7,16,  8,16,  9,17,  10,17, 11,18, 12,18, 13,19, 14,19, 15,20, 16,20, 17,20, 18,21, 19,21, 20,22, 21,22, 22,23, 23,23, 23,23},
    {5,14, 6,14, 7,15, 8,15,  9,16,  10,16, 11,17, 12,17, 13,18, 14,18, 15,19, 16,20, 17,20, 18,21, 19,21, 20,22, 21,22, 22,23, 23,23, 23,23, 23,23},
    {5,13, 6,13, 7,14, 8,14,  9,15,  10,16, 11,16, 12,17, 13,17, 14,18, 15,19, 16,19, 17,20, 18,20, 19,21, 20,22, 21,22, 22,23, 23,23, 23,23, 23,23},
    {5,12, 6,12, 7,13, 8,14,  9,14,  10,15, 11,16, 12,16, 13,17, 14,18, 15,19, 16,19, 17,20, 18,20, 19,21, 20,22, 21,22, 22,23, 23,23, 23,23, 23,23},
    {6,11, 7,11, 8,12, 9,13,  10,14, 11,14, 12,15, 13,16, 14,17, 15,17, 16,18, 17,19, 18,20, 19,20, 20,21, 21,22, 22,23, 23,23, 23,23, 23,23, 23,23},
    {6,10, 7,10, 8,11, 9,12,  10,13, 11,14, 12,14, 13,15, 14,16, 15,17, 16,18, 17,18, 18,19, 19,20, 20,21, 21,22, 22,22, 23,23, 23,23, 23,23, 23,23},
    {7,9,  8,10, 9,11, 10,12, 11,13, 12,13, 13,14, 14,15, 15,16, 16,17, 17,18, 18,18, 19,19, 20,20, 21,21, 22,22, 23,23, 23,23, 23,23, 23,23, 23,23},
    {7,8,  8,9,  9,10, 10,11, 11,12, 12,13, 13,14, 14,15, 15,16, 16,16, 17,17, 18,18, 19,19, 20,20, 21,21, 22,22, 23,23, 23,23, 23,23, 23,23, 23,23},
    {8,7,  9,8,  10,9, 11,10, 12,11, 13,12, 14,13, 15,14, 16,15, 16,16, 17,17, 18,18, 19,19, 20,20, 21,21, 22,22, 23,23, 23,23, 23,23, 23,23, 23,23},
    {9,7,  10,8, 11,9, 12,10, 13,11, 13,12, 14,13, 15,14, 16,15, 17,16, 18,17, 19,18, 19,19, 20,20, 21,21, 22,22, 23,23, 23,23, 23,23, 23,23, 23,23},
    {10,6, 11,7, 12,8, 12,9,  13,10, 14,11, 15,12, 15,13, 16,14, 17,15, 18,16, 18,17, 19,18, 20,19, 21,20, 21,21, 22,22, 23,23, 23,23, 23,23, 23,23},
    {11,6, 11,7, 12,8, 13,9,  14,10, 14,11, 15,12, 16,13, 17,14, 17,15, 18,16, 19,17, 20,18, 20,19, 21,20, 22,21, 23,22, 23,23, 23,23, 23,23, 23,23},
    {12,5, 13,6, 13,7, 14,8,  15,9,  15,10, 16,11, 17,12, 17,13, 18,14, 19,15, 19,16, 20,17, 21,18, 21,19, 22,20, 23,21, 23,22, 24,23, 24,23, 24,23},
    {13,5, 13,6, 14,7, 14,8,  15,9,  16,10, 16,11, 17,12, 17,13, 18,14, 19,15, 19,16, 20,17, 20,18, 21,19, 22,20, 22,21, 23,22, 23,23, 23,23, 23,23},
    {14,5, 14,6, 15,7, 15,8,  16,9,  16,10, 17,11, 18,12, 18,13, 19,14, 19,15, 20,16, 20,17, 21,18, 22,19, 22,20, 23,21, 23,22, 24,23, 24,23, 24,23},
    {15,4, 15,5, 16,6, 16,7,  17,8,  17,9,  18,10, 18,11, 19,12, 19,13, 20,14, 20,15, 21,16, 21,17, 22,18, 22,19, 23,20, 23,21, 24,22, 24,23, 24,23},
    {16,4, 16,5, 17,6, 17,7,  18,8,  18,9,  18,10, 19,11, 19,12, 20,13, 20,14, 21,15, 21,16, 21,17, 22,18, 22,19, 23,20, 23,21, 24,22, 24,23, 24,23},
    {17,4, 17,5, 17,6, 18,7,  18,8,  18,9,  19,10, 19,11, 19,12, 20,13, 20,14, 21,15, 21,16, 21,17, 22,18, 22,19, 22,20, 23,21, 23,22, 23,23, 24,23},
    {18,3, 18,4, 18,5, 19,6,  19,7,  19,8,  20,9,  20,10, 20,11, 21,12, 21,13, 21,14, 22,15, 22,16, 22,17, 23,18, 23,19, 23,20, 24,21, 24,22, 24,23},
    {19,3, 19,4, 19,5, 19,6,  20,7,  20,8,  20,9,  20,10, 21,11, 21,12, 21,13, 21,14, 22,15, 22,16, 22,17, 22,18, 23,19, 23,20, 23,21, 23,22, 24,23},
    {20,3, 20,4, 20,5, 20,6,  20,7,  20,8,  21,9,  21,10, 21,11, 21,12, 21,13, 22,14, 22,15, 22,16, 22,17, 22,18, 23,19, 23,20, 23,21, 23,22, 23,23},
    {21,3 ,21,4, 21,5, 21,6,  21,7,  21,8,  21,9,  21,10, 22,11, 22,12, 22,13, 22,14, 22,15, 22,16, 23,17, 23,18, 23,19, 23,20, 23,21, 23,22, 23,23},
    {22,3, 22,4, 22,5, 22,6,  22,7,  22,8,  22,9,  22,10, 22,11, 22,12, 23,13, 23,14, 23,15, 23,16, 23,17, 23,18, 23,19, 23,20, 23,21, 23,22, 23,23},
    {23,3, 23,4, 23,5, 23,6,  23,7,  23,8,  23,9,  23,10, 23,11, 23,12, 24,13, 24,14, 24,15, 24,16, 24,17, 24,18, 24,19, 24,20, 24,21, 24,22, 24,23},
    {24,3, 24,4, 24,5, 24,6,  24,7,  24,8,  24,9,  24,10, 24,11, 24,12, 24,13, 24,14, 24,15, 24,16, 24,17, 24,18, 24,19, 24,20, 24,21, 24,22, 24,23}
};


void _DrawNeedle(uint8_t x, uint8_t y, size_t idx, SSD1306_COLOR color)
{
    if(idx < 34){
        for(size_t i = 0; i<NEEDLE_SIZE; i++){
            uint8_t tempX, tempY;
            tempX = img_meterNeedle_data[idx][i*2];
            tempY = img_meterNeedle_data[idx][i*2+1];
            ssd1306_DrawPixel(x+tempX, y+tempY, color);
        }
    }
    else{
        idx = 67-idx;

        for(size_t i = 0; i<NEEDLE_SIZE; i++){
            uint8_t tempX, tempY;
            tempX = img_meter.Xsize - img_meterNeedle_data[idx][i*2];
            tempY = img_meterNeedle_data[idx][i*2+1];
            ssd1306_DrawPixel(x+tempX, y+tempY, color);
        }
    }
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param idx max 67
 */
void Image_DrawNeedle(uint8_t x, uint8_t y, size_t idx)
{
    static uint8_t last_idx = 0;

    if(idx >= 68)
        idx = 67;
        
    _DrawNeedle(x, y, last_idx, Black);
    _DrawNeedle(x, y, idx, White);
    last_idx = idx;
}