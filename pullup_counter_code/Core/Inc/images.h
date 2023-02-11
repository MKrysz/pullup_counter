#ifndef __IMAGES_H__
#define __IMAGES_H__

#include <stdint.h>
#include <stdlib.h>


typedef struct {
    size_t size;
    uint8_t Xsize;
    uint8_t Ysize;
    const uint8_t **data;
} image_t;

typedef struct {
    image_t *data;
    size_t NrOfFrames;
    uint8_t Xsize;
    uint8_t Ysize;
} animation_t;

void Image_Draw(uint8_t x, uint8_t y, image_t image);

extern image_t img_meter;
extern image_t img_clk;
extern image_t img_sd;
extern image_t img_proxOn;
extern image_t img_autoUpdate;
extern image_t img_smallBattery;


#endif // __IMAGES_H__
