#include <video.h>
#include <terminal.h>
#include <graphics.h>

static uint32_t * video = (uint32_t *) 0x5c28;
static uint8_t cursorX = 0;
static uint8_t cursorY = 0;


void writeScreen(uint8_t x, uint8_t y, char ch, color_t background, color_t charColor) {
	if(ch == 0)
		drawChar(' ', x * CHAR_WIDTH, y * CHAR_HEIGHT, background, charColor);
	else
		drawChar(ch, x * CHAR_WIDTH, y * CHAR_HEIGHT, background, charColor);
}

void writeAtCursor(char ch, color_t background, color_t charColor) {
	if(ch == 0)
		drawChar(' ', cursorX * CHAR_WIDTH, cursorY * CHAR_HEIGHT, background, charColor);
	else
		drawChar(ch, cursorX * CHAR_WIDTH, cursorY * CHAR_HEIGHT, background, charColor);
}

void updateCursor(uint8_t x, uint8_t y) {
	cursorX = x;
	cursorY = y;
}

void writeStyle(uint8_t x, uint8_t y, color_t background, color_t charColor) {
}

void setScreen(video_row* buffer) {
	int x;
	int y;
	for(x = 0; x < COLS; x++) {
		for(y = 0; y < ROWS; y++) {
				drawChar(buffer[y][x].ch == 0 ? ' ' : buffer[y][x].ch , x * CHAR_WIDTH, y * CHAR_HEIGHT, buffer[y][x].background, buffer[y][x].charColor);
		}
	}
}
