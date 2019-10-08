#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <font.h>

/* from pure64.asm (118h = 1024x768x16M)
; Set video mode http://www.ctyme.com/intr/int-10.htm
    mov ax, 0x4F02
    mov bx, 0x118 
    int 0x10
*/

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

#define COLOR_SIZE 3
#define CELLSIZE 7

typedef struct {
	char r, g, b;
} color_t;

typedef struct {
	char ch;
	color_t charColor;
	color_t background;
} cell_t;

extern uint16_t height;
extern uint16_t width;

void getResolution();
void drawPixel(int x, int y);
void drawColorPixel(int x, int y, color_t color);
void drawChar(char c, int x, int y, color_t background, color_t charColor);
void drawSquare(int x, int y);

#endif
