#pragma once

#include <stdint.h>

#define ORI_BF_TYPE         (uint16_t)0x4d42
#define ORI_BF_SIZE         (uint32_t)0x00000a56
#define ORI_BF_RESERVED1    (uint16_t)0x0000
#define ORI_BF_RESERVED2    (uint16_t)0x0000
#define ORI_BF_OFFBITS      (uint32_t)0x00000036
#define ORI_BI_SIZE         (uint32_t)0x00000028
#define ORI_BI_WIDTH        (uint32_t)0x00000018
#define ORI_BI_HEIGHT       (uint32_t)0x00000024
#define ORI_BI_PLANES       (uint16_t)0x0001
#define ORI_BI_BITCOUNT     (uint16_t)0x0018
#define ORI_BI_COMPRESSION  (uint32_t)0x00000000
#define ORI_BI_XPELSPERMETER (int32_t)0x00000000
#define ORI_BI_YPELSPERMETER (int32_t)0x00000000
#define ORI_BI_CLRUSED      (uint32_t)0x00000000
#define ORI_BI_CLRIMPORTANT (uint32_t)0x00000000

typedef struct {
    //BMP file info
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
    //BMP header info
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXpelsperMeter;
    uint32_t biYpelsperMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
    uint8_t* bmpPixel;
    uint32_t lineSize;
}BMPInfo_t;

typedef enum {
    //BMP file info
    bfType_index = 0x0000,
    bfSize_index = 0x0002,
    bfReserved1_index = 0x0006,
    bfReserved2_index = 0x0008,
    bfOffBits_index = 0x000a,
    //BMP header info
    biSize_index = 0x000e,
    biWidth_index = 0x0012,
    biHeight_index = 0x0016,
    biPlanes_index = 0x001a,
    biBitCount_index = 0x001c,
    biCompression_index = 0x001e,
    biSizeImage_index = 0x0022,
    biXpelsperMeter_index = 0x0026,
    biYpelsperMeter_index = 0x002a,
    biClrUsed_index = 0x002e,
    biClrImportant_index = 0x0032,
}BMPInfoOffset_e;


