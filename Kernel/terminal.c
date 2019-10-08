#include <terminal.h>
#include <lib.h>
#include <video.h>
#include <graphics.h>


static video_row videoBuffer[ROWS];
static uint32_t * video = (uint32_t *) 0x5C28;
static char kbBuffer[BUFFER_SIZE];

static uint16_t startIndex = 0;
static uint16_t endIndex = 0;
static uint16_t writeIndex = 0;
static uint16_t readIndex = 0;
static uint16_t size = 0;

static uint8_t echo = TRUE;
static uint8_t cursorVisible = FALSE;
static uint8_t blink = TRUE;

static uint8_t cursorX = 0;
static uint8_t cursorY = ROWS - 1;
static char defaultStyle = 0;
static color_t defaultB = {0,0,0};
static color_t defaultC = {0xFF,0xFF,0xFF};

static char buffer[64];

//==============================================================================
//  VIDEO
//==============================================================================

void updateScreen();
void toggleCursors();

void printChar(char ch, color_t background, color_t charColor) {
	int x = cursorX;
	int y = cursorY;
	int prevX = -1;
	int prevY;
	while(videoBuffer[y][x].ch != 0) {
		x++;
		if(x == COLS) {
			x = 0;
			if(y == ROWS-1) {
				if(cursorY == 0)
					return;
				shiftScreen();
				cursorY--;
			}
			else
				y++;
		}
	}
	while(x != cursorX || y != cursorY) {
		prevX = x-1;
		prevY = y;
		if(prevX < 0) {
			prevX = COLS-1;
			prevY--;
		}
		videoBuffer[y][x] = videoBuffer[prevY][prevX];
		writeScreen(x,y,videoBuffer[prevY][prevX].ch, videoBuffer[prevY][prevX].background, videoBuffer[prevY][prevX].charColor);
		x = prevX;
		y = prevY;
	}
	videoBuffer[cursorY][cursorX].charColor = defaultC;
	videoBuffer[cursorY][cursorX].background = defaultB;
	videoBuffer[cursorY][cursorX].ch = ch;
	if(prevX == -1)
		writeAtCursor(ch, background, charColor);
	else
		writeAtCursor(ch, defaultB, defaultC);
		//updateScreen();
	incrementCursor();
}

void printc(char ch) {
	printcWithStyle(ch, defaultB, defaultC);
}

void printcWithStyle(char ch, color_t background, color_t charColor) {
	switch(ch) {
		case 8:
			backspace();
			break;
		case 10:
			newLine();
			break;
		default:
			toggleCursors();
			printChar(ch, background, charColor);
			toggleCursors();
			break;
	}
}

void print(char *s) {
	toggleCursors();
	int i = 0;
	while(s[i] != 0) {
		printChar(s[i], defaultB, defaultC);
		i++;
	}
	toggleCursors();
}

void println(char *s) {
	print(s);
	newLine();
}

void setStyle(color_t background, color_t charColor) {
	defaultB = background;
	defaultC = charColor;
}

void incrementCursor() {
	if(cursorX == COLS-1) {
		cursorX = 0;

		if(cursorY == ROWS-1)
			shiftScreen();
		else
			cursorY++;
	}
	else
		cursorX++;

	updateCursor(cursorX, cursorY);
}

void newLine() {
	toggleCursors();
	while (videoBuffer[cursorY][cursorX].ch != 0) {
		if(cursorX == COLS-1) {
			cursorX = 0;
			if(cursorY == ROWS-1)
				shiftScreen();
			else
				cursorY++;
		}
		else
			cursorX++;

	}

	cursorX = 0;

	if(cursorY == ROWS-1)
		shiftScreen();
	else
		cursorY++;

	updateCursor(cursorX, cursorY);
	//updateScreen();
	toggleCursors();
}

void backspace() {
	if(cursorY > 0 || cursorX > 0) {
		toggleCursors();
		int x = cursorX;
		int y = cursorY;
		int prevX;
		int prevY;
		if(x == 0) {
			prevX = COLS-1;
			prevY = y-1;
		}
		else {
			prevX = x-1;
			prevY = y;
		}
		int offset = 0;
		cursorX = prevX;
		cursorY = prevY;
		while(prevY >= 0 && videoBuffer[prevY][prevX].ch == 0) {
			offset++;
			cursorX = prevX;
			cursorY = prevY;
			prevX--;
			if(prevX < 0) {
				prevX = COLS-1;
				prevY--;
			}
		}
		if(offset == 0)
			offset = 1;

		do {
			prevX = x - offset;
			prevY = y;
			while (prevX < 0) {
				prevX += COLS;
				prevY--;
			}

			videoBuffer[prevY][prevX] = videoBuffer[y][x];
			writeScreen(prevX, prevY, videoBuffer[y][x].ch, videoBuffer[y][x].background, videoBuffer[y][x].charColor);
			videoBuffer[y][x].ch = 0;
			writeScreen(x, y, ' ', videoBuffer[y][x].background, videoBuffer[y][x].charColor);

			if(x == COLS-1) {
				x = 0;
				y++;
			}
			else
				x++;

		} while(y < ROWS && videoBuffer[prevY][prevX].ch != 0);

		updateCursor(cursorX, cursorY);
		//updateScreen();
		toggleCursors();
	}
}

void cursorUp() {
	toggleCursors();
	if(cursorY > 0)
		cursorY--;
	while (cursorX > 0 && videoBuffer[cursorY][cursorX].ch == 0)
		cursorX--;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorDown() {
	toggleCursors();
	if(cursorY < ROWS-1)
		cursorY++;
	while (cursorX > 0 && videoBuffer[cursorY][cursorX].ch == 0)
		cursorX--;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorLeft() {
	toggleCursors();
	if(cursorX > 0)
		cursorX--;
	else if(cursorY > 0) {
		cursorY--;
		while(cursorX < COLS-1 && videoBuffer[cursorY][cursorX].ch != 0)
			cursorX++;
	}
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorRight() {
	toggleCursors();
	if(cursorX == COLS-1 || videoBuffer[cursorY][cursorX].ch == 0) {
		if(cursorY < ROWS-1) {
			cursorY++;
			cursorX = 0;
		}
	}
	else
		cursorX++;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void updateScreen() {
	setScreen(videoBuffer);
}

void shiftScreen() {
	memcpy((uint8_t*) videoBuffer[0], (uint8_t*) videoBuffer[1], CELLSIZE*COLS*(ROWS-1));
	memcpy((uint8_t*) *video, (uint8_t*) *video + width * COLOR_SIZE * CHAR_HEIGHT, width * (height-CHAR_HEIGHT) * COLOR_SIZE);
	for(uint8_t x = 0; x < COLS; x++) {
		videoBuffer[ROWS-1][x].ch = 0;
		videoBuffer[ROWS-1][x].background = defaultB;
		videoBuffer[ROWS-1][x].charColor = defaultC;
		writeScreen(x, ROWS - 1, ' ', defaultB, defaultC);
	}
	//updateScreen();
}

void clearScreen() {
	toggleCursors();
	for(uint8_t y = 0; y < ROWS; y++) {
		for(uint8_t x = 0; x < COLS; x++) {
			videoBuffer[y][x].ch = 0;
			videoBuffer[y][x].background = defaultB;
			videoBuffer[y][x].charColor = defaultC;
			writeScreen(x, y, ' ', defaultB, defaultC);
		}
	}
	cursorX = 0;
	cursorY = ROWS - 1;
	updateCursor(0, ROWS - 1);
	//updateScreen();
	toggleCursors();
}

uint8_t getCharAt(uint8_t x, uint8_t y) {
    return videoBuffer[y][x].ch;
}

void printBase(uint64_t value, uint32_t base) {
	uintToBase(value, buffer, base);
	print(buffer);
}

void printDec(uint64_t value) {
	printBase(value, 10);
}

void printHex(uint64_t value) {
	printBase(value, 16);
}

void printBin(uint64_t value) {
	printBase(value, 2);
}

void toggleCursors() {
	if(cursorVisible) {
		videoBuffer[cursorY][cursorX].background.r = 255 - videoBuffer[cursorY][cursorX].background.r;
		videoBuffer[cursorY][cursorX].background.g = 255 - videoBuffer[cursorY][cursorX].background.g;
		videoBuffer[cursorY][cursorX].background.b = 255 - videoBuffer[cursorY][cursorX].background.b;
		videoBuffer[cursorY][cursorX].charColor.r = 255 - videoBuffer[cursorY][cursorX].charColor.r;
		videoBuffer[cursorY][cursorX].charColor.g = 255 - videoBuffer[cursorY][cursorX].charColor.g;
		videoBuffer[cursorY][cursorX].charColor.b = 255 - videoBuffer[cursorY][cursorX].charColor.b;
		writeScreen(cursorX, cursorY, videoBuffer[cursorY][cursorX].ch ,videoBuffer[cursorY][cursorX].background, videoBuffer[cursorY][cursorX].charColor);
	}
	if(blink)
		blink = FALSE;
	else
		blink = TRUE;
}

void blinkCursor() {
	if(blink) {
		videoBuffer[cursorY][cursorX].background.r = 255 - videoBuffer[cursorY][cursorX].background.r;
		videoBuffer[cursorY][cursorX].background.g = 255 - videoBuffer[cursorY][cursorX].background.g;
		videoBuffer[cursorY][cursorX].background.b = 255 - videoBuffer[cursorY][cursorX].background.b;
		videoBuffer[cursorY][cursorX].charColor.r = 255 - videoBuffer[cursorY][cursorX].charColor.r;
		videoBuffer[cursorY][cursorX].charColor.g = 255 - videoBuffer[cursorY][cursorX].charColor.g;
		videoBuffer[cursorY][cursorX].charColor.b = 255 - videoBuffer[cursorY][cursorX].charColor.b;
		writeScreen(cursorX, cursorY, videoBuffer[cursorY][cursorX].ch ,videoBuffer[cursorY][cursorX].background, videoBuffer[cursorY][cursorX].charColor);
		if(cursorVisible)
			cursorVisible = FALSE;
		else
			cursorVisible = TRUE;
	}
}

//==============================================================================
//  KEYBOARD
//==============================================================================

void shiftLeft() {
	if(writeIndex != startIndex) {
		uint16_t from = writeIndex-1;
		if(from < 0)
			from = BUFFER_SIZE;
		uint16_t to;
		while(from != endIndex) {
			to = from;
			from++;
			if(from == BUFFER_SIZE)
				from = 0;
			kbBuffer[to] = kbBuffer[from];
		}
	}
}

void shiftRight() {
	if(writeIndex != endIndex) {
		uint16_t from = endIndex;
		uint16_t to;
		do {
			to = from;
			from--;
			if(from < 0)
				from = BUFFER_SIZE;
			kbBuffer[to] = kbBuffer[from];
		}	while(from != writeIndex);
	}
}

void writeBuffer(char ch) {
	switch (ch) {
		case '\b':
			if(writeIndex != startIndex) {
				shiftLeft();
				writeIndex--;
				if(writeIndex < 0)
					writeIndex = BUFFER_SIZE;
				endIndex--;
				if(endIndex < 0)
					endIndex = BUFFER_SIZE;
				size--;
				if(echo)
					printc(ch);
			}
			break;
		case '\n':
			kbBuffer[endIndex] = ch;
			endIndex++;
			size++;
			if(endIndex == BUFFER_SIZE)
				endIndex = 0;
			writeIndex = startIndex = endIndex;
			if(echo)
				printc(ch);
			break;
		default:
			if(size < BUFFER_SIZE-1) {					//Dejar un espacio para \n
				shiftRight();
				kbBuffer[writeIndex] = ch;
				writeIndex++;
				if(writeIndex == BUFFER_SIZE)
					writeIndex = 0;
				endIndex++;
				if(endIndex == BUFFER_SIZE)
					endIndex = 0;
				size++;
			}
			if(echo)
				printc(ch);
			break;
	}
}

char readBuffer() {
	char ch = 0;
	if(readIndex < startIndex) {
		ch = kbBuffer[readIndex];
		readIndex++;
		if(readIndex == BUFFER_SIZE)
			readIndex = 0;
		size--;
	}
	return ch;
}

void keyboardLeft() {
	if(writeIndex != startIndex) {
		writeIndex--;
		if(writeIndex < 0)
			writeIndex = BUFFER_SIZE;
		if(echo)
			cursorLeft();
	}
}

void keyboardRight() {
	if(writeIndex != endIndex) {
		writeIndex++;
		if(writeIndex == BUFFER_SIZE)
			writeIndex = 0;
		if(echo)
			cursorRight();
	}
}

void setEcho(uint8_t boolean) {
	echo = boolean;
}
