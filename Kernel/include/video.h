#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <graphics.h>

#define ROWS (WIN_HEIGHT / CHAR_HEIGHT)
#define COLS (WIN_WIDTH / CHAR_WIDTH)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef  FALSE
#define FALSE 0
#endif

typedef cell_t video_row[COLS];

void writeAtCursor(char ch, color_t background, color_t charColor);
void writeScreen(uint8_t x, uint8_t y, char ch, color_t background, color_t charColor);
void updateCursor(uint8_t x, uint8_t y);
void writeStyle(uint8_t x, uint8_t y, color_t background, color_t charColor);
void setScreen(video_row* buffer);

#endif
