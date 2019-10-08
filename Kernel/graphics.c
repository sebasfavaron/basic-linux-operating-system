#include <graphics.h>

uint32_t * base = (uint32_t *) 0x5C28;
uint16_t width;
uint16_t height;

static color_t white = {.r = 0xFF, .g = 0xFF , .b = 0xFF};
static color_t black = {.r = 0x00, .g = 0x00 , .b = 0x00};

void getResolution() {
  uint16_t * xres = (uint16_t *) 0x5C12;
	uint16_t * yres = (uint16_t *) 0x5C14;

  width = *xres;
  height = *yres;
}

void drawPixel(int x, int y) {
    drawColorPixel(x, y, white);
}

void drawColorPixel(int x, int y, color_t color) {
    char * vi = *base + width * COLOR_SIZE * y + COLOR_SIZE * x;
    vi[0] = color.b;
    vi[1] = color.g;
    vi[2] = color.r;
}

void drawChar(char c, int x, int y, color_t background, color_t charColor) {
	int i, j;
 	const unsigned char * font_char = (unsigned char *) pixel_map(c);

    // Draw pixels
    for (i=0; i<CHAR_WIDTH; i++) {
        for (j=0; j<CHAR_HEIGHT; j++) {
            if (font_char[j] & (1<<(CHAR_WIDTH-1-i)))
                drawColorPixel(x + i, y + j, charColor);
            else
            	drawColorPixel(x + i, y + j, background);
        }
    }
}

void drawSquare(int x, int y) {
    int i, j;
    // Draw pixels
    for (i=0; i<CHAR_WIDTH; i++) {
        for (j=0; j<CHAR_HEIGHT; j++) {
                drawPixel(x + i, y + j);
        }
    }
}
