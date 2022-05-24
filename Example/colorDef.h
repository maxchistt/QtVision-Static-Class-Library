#pragma once
#include <vsn_vision.h>
#include <vsn_color.h>

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#define BLACK        RGB(0,   0,   0)
#define BLUE         RGB(0,   0, 192)
#define GREEN        RGB(0, 192,   0)
#define CYAN         RGB(0, 128, 128)
#define RED          RGB(192,   0,   0)
#define MAGENTA      RGB(96,   0, 192)
#define BROWN        RGB(192, 128,   0)
#define LIGHTGRAY    RGB(192, 192, 192)
#define DARKGRAY     RGB(128, 158, 158)
#define LIGHTBLUE    RGB(0,   0, 255)
#define LIGHTGREEN   RGB(0, 255,   0)
#define LIGHTCYAN    RGB(0, 125, 255)
#define LIGHTRED     RGB(255,   0,   0)
#define LIGHTMAGENTA RGB(255,   0, 125)
#define YELLOW       RGB(128, 128,   0)
#define WHITE        RGB(255, 255, 255)
#define GOLDEN       RGB(222, 222, 125)
