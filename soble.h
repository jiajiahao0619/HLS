#ifndef SOBLE_H_
#define SOBLE_H_


#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ap_int.h>

typedef short             coef;
typedef unsigned char     uint8;
typedef unsigned short    uint16;
typedef char              int8;
typedef short             int16;
typedef long long         lint;

#define H  240
#define W  320
#define WIDTH 64

void soble( lint in_img[H+H/2][W/8], lint out_img[H][W*3/8],coef setH, coef setW);


#endif
