#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "BMP.h"
#include "osd_stitch.h"

int main(int argc, char* argv[])
{
    char path[256];
    FILE* fi;
    FILE* fo;
    char ifname[256];
    char ofname[256];

    int index, len;
    int i, j, ptr, offset;
    char cindex[8];
    int row, col;

    BMPInfo_t bmpOri, bmpDst;
    uint8_t* buf_ori;
    uint8_t* buf_dst;

    buf_ori = (uint8_t*)malloc(0x4000);
    buf_dst = (uint8_t*)malloc(0x400000);
    bmpDst.bmpPixel = buf_dst;

    strcpy(path, argv[1]); // input bmp file path
    strcpy(ofname, argv[2]); // output bmp file name

    //create output file
    fo = fopen(ofname, "wb+");
    //exit if create output file error
    if (fo == NULL) {
        printf("\ncan not create file:%s", ofname);
        return 0;
    }

    for (index = 0; index < 256; index++) {
        //generate ifname
        ptr = 0;
        len = strlen(path);
        for (int i = 0; i < len; i++)
            ifname[ptr++] = path[i];
        ifname[ptr++] = '/';
        itoa(index, cindex, 10);
        len = strlen(cindex);
        for (i = 0; i < len; i++)
            ifname[ptr++] = cindex[i];
        ifname[ptr++] = '.';
        ifname[ptr++] = 'b';
        ifname[ptr++] = 'm';
        ifname[ptr++] = 'p';
        ifname[ptr++] = '\0';

        //open bmp file
        printf("\nRead file:%s", ifname);
        fi = fopen(ifname, "rb");
        //exit when open file error
        if (fi == NULL) {
            printf("\nCan not open file:%s", ifname);
            return 0;
        }
        //read bfType to buf_ori
        bmpOri.bfType = fgetc(fi);
        bmpOri.bfType = (bmpOri.bfType << 8) | fgetc(fi);
        if (bmpOri.bfType != 0x424d) { // "BM"
            printf("\nUnknow bfType");
            return 0;
        }
        //read bfSize to buf_ori
        bmpOri.bfSize = fgetc(fi);
        bmpOri.bfSize |= fgetc(fi) << 8;
        bmpOri.bfSize |= fgetc(fi) << 16;
        bmpOri.bfSize |= fgetc(fi) << 24;
        //reset fi'ptr location
        fseek(fi, 0L, SEEK_SET);
        fread(buf_ori, sizeof(uint8_t), bmpOri.bfSize, fi);
        //get all data to bmpOri
        bmpOri.bfReserved1 = buf_ori[bfReserved1_index] | (buf_ori[bfReserved1_index + 1] << 8);
        bmpOri.bfReserved2 = buf_ori[bfReserved1_index] | (buf_ori[bfReserved1_index + 1] << 8);
        bmpOri.bfOffBits = buf_ori[bfOffBits_index] | (buf_ori[bfOffBits_index+1] << 8) | (buf_ori[bfOffBits_index+2] << 16) | (buf_ori[bfOffBits_index+3] << 24);
        bmpOri.biSize = buf_ori[biSize_index] | (buf_ori[biSize_index+1] << 8) | (buf_ori[biSize_index+2] << 16) | (buf_ori[biSize_index+3] << 24);
        bmpOri.biWidth = buf_ori[biWidth_index] | (buf_ori[biWidth_index + 1] << 8) | (buf_ori[biWidth_index + 2] << 16) | (buf_ori[biWidth_index + 3] << 24);
        bmpOri.biHeight = buf_ori[biHeight_index] | (buf_ori[biHeight_index + 1] << 8) | (buf_ori[biHeight_index + 2] << 16) | (buf_ori[biHeight_index + 3] << 24);
        bmpOri.biPlanes = buf_ori[biPlanes_index] | (buf_ori[biPlanes_index + 1] << 8);
        bmpOri.biBitCount = buf_ori[biBitCount_index] | (buf_ori[biBitCount_index + 1] << 8);
        bmpOri.biCompression = buf_ori[biCompression_index] | (buf_ori[biCompression_index + 1] << 8) | (buf_ori[biCompression_index + 2] << 16) | (buf_ori[biCompression_index + 3] << 24);
        bmpOri.biSizeImage = buf_ori[biSizeImage_index] | (buf_ori[biSizeImage_index + 1] << 8) | (buf_ori[biSizeImage_index + 2] << 16) | (buf_ori[biSizeImage_index + 3] << 24);
        bmpOri.biXpelsperMeter = buf_ori[biXpelsperMeter_index] | (buf_ori[biXpelsperMeter_index + 1] << 8) | (buf_ori[biXpelsperMeter_index + 2] << 16) | (buf_ori[biXpelsperMeter_index + 3] << 24);
        bmpOri.biYpelsperMeter = buf_ori[biYpelsperMeter_index] | (buf_ori[biYpelsperMeter_index + 1] << 8) | (buf_ori[biYpelsperMeter_index + 2] << 16) | (buf_ori[biYpelsperMeter_index + 3] << 24);
        bmpOri.biClrUsed = buf_ori[biClrUsed_index] | (buf_ori[biClrUsed_index + 1] << 8) | (buf_ori[biClrUsed_index + 2] << 16) | (buf_ori[biClrUsed_index + 3] << 24);
        bmpOri.biClrImportant = buf_ori[biClrImportant_index] | (buf_ori[biClrImportant_index + 1] << 8) | (buf_ori[biClrImportant_index + 2] << 16) | (buf_ori[biClrImportant_index + 3] << 24);
        bmpOri.bmpPixel = buf_ori + bmpOri.bfOffBits;
        if (bmpOri.biWidth != OSD_WIDTH_ORI || bmpOri.biHeight != OSD_HEIGHT_ORI) {
            printf("\nUnsupport size %dx%d, only support %dx%d", bmpOri.biWidth, bmpOri.biHeight, OSD_WIDTH_ORI, OSD_HEIGHT_ORI);
            return 0;
        }
        if((bmpOri.biPlanes != 0x0001) || bmpOri.biBitCount != 0x0018) {
            printf("\nUnsupport color format, only support 24-bit Bitmap");
            return 0;
        }
        bmpOri.lineSize = bmpOri.biWidth * (bmpOri.biBitCount >> 3) + (4 - (bmpOri.biWidth * bmpOri.biBitCount >> 3) & 0x03);
        bmpDst.lineSize = bmpOri.biWidth * (bmpOri.biBitCount >> 3) << 4;

        //Copy bmpOri.bmpPixel to bmpDst.bmpPixel
        row = index >> 4;
        col = index & 0x0f;
        offset = row * bmpDst.lineSize * bmpOri.biHeight + col * bmpOri.biWidth * (bmpOri.biBitCount >> 3);
        for (i = 0; i < bmpOri.biHeight; i++) {
            for (j = 0; j < bmpOri.biWidth * (bmpOri.biBitCount >> 3); j++) {
                bmpDst.bmpPixel[offset + (bmpOri.biHeight - 1 - i) * bmpDst.lineSize + j] = bmpOri.bmpPixel[i * bmpOri.lineSize + j]; // line location convert
            }
        }

        //close fi
        fclose(fi);
    }

    //initial bmpDst info
    bmpDst.bfType = 0x4d42;
    bmpDst.bfSize = bmpOri.bfOffBits + bmpOri.biWidth * bmpOri.biHeight * 3 * 256;
    bmpDst.bfReserved1 = bmpOri.bfReserved1;
    bmpDst.bfReserved2 = bmpOri.bfReserved2;
    bmpDst.bfOffBits = bmpOri.bfOffBits;
    bmpDst.biSize = bmpOri.biSize;
    bmpDst.biWidth = bmpOri.biWidth * 16;
    bmpDst.biHeight = bmpOri.biHeight * 16;
    bmpDst.biPlanes = bmpOri.biPlanes;
    bmpDst.biBitCount = bmpOri.biBitCount;
    bmpDst.biCompression = bmpOri.biCompression;
    bmpDst.biSizeImage = bmpOri.biSizeImage;
    bmpDst.biXpelsperMeter = bmpOri.biXpelsperMeter;
    bmpDst.biYpelsperMeter = bmpOri.biYpelsperMeter;
    bmpDst.biClrUsed = bmpOri.biClrUsed;
    bmpDst.biClrImportant = bmpOri.biClrImportant;

    fwrite(&(bmpDst.bfType), sizeof(uint8_t), 2, fo);
    fwrite(&(bmpDst.bfSize), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.bfReserved1), sizeof(uint8_t), 2, fo);
    fwrite(&(bmpDst.bfReserved2), sizeof(uint8_t), 2, fo);
    fwrite(&(bmpDst.bfOffBits), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biSize), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biWidth), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biHeight), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biPlanes), sizeof(uint8_t), 2, fo);
    fwrite(&(bmpDst.biBitCount), sizeof(uint8_t), 2, fo);
    fwrite(&(bmpDst.biCompression), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biSizeImage), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biXpelsperMeter), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biYpelsperMeter), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biClrUsed), sizeof(uint8_t), 4, fo);
    fwrite(&(bmpDst.biClrImportant), sizeof(uint8_t), 4, fo);

    for (i = 0; i < bmpDst.biHeight; i++) {
        fwrite(bmpDst.bmpPixel + (bmpDst.biHeight - 1 - i) * bmpDst.lineSize, sizeof(uint8_t), bmpDst.lineSize, fo);
    }

    fclose(fo);

    return 1;
}