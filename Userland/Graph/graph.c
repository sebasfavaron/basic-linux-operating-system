#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PIXEL_TO_NUMBER 0.1
#define DISTANCE (1/PIXEL_TO_NUMBER)

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int main() {
	int width, height;
	int pixelX, pixelY;
	double a = 0, b = 0, c = 0;
  int markX = DISTANCE, markY = DISTANCE;
	int buffer;

	int80(10,&width, &height, 0, 0);

	int80(2, 0, 0, 0, 0);

	printf("Graficador de funcion lineal o parabolica [ AX^2 + BX + C ]");

 	printf("\nIngrese el valor de A: ");
		scanf("%f", &a);

    printf("Ingrese el valor de B: ");
		scanf("%f", &b);

    printf("Ingrese el valor de C: ");
		scanf("%f", &c);

    int80(2, 0, 0, 0, 0);

    for (pixelX = 0; pixelX < width; pixelX++) {
    	double x = (pixelX - width/2)*PIXEL_TO_NUMBER;
    	double y = a * x * x + b * x + c;

    	int pixelY = height/2 - (y/PIXEL_TO_NUMBER);

        //Draw Function
    	if (pixelY >= 0 && pixelY < height)
            int80(9, pixelX, pixelY, 0, 0);

        //Draw X Axis
        int80(9, pixelX, height/2, 0, 0);
    }

    //Draw Y Axis
    for (pixelY = 0; pixelY < height; pixelY++) {
        int80(9, width/2, pixelY, 0, 0);
    }

    //Draw X Axis Marks
    while (markX < width/2) {
        int i;
        for (i=1; i < 3; i++) {
            int80(9, width/2 + markX, height/2 + i, 0, 0);
            int80(9, width/2 + markX, height/2 - i, 0, 0);
            int80(9, width/2 - markX, height/2 + i, 0, 0);
            int80(9, width/2 - markX, height/2 - i, 0, 0);
        }

        markX += DISTANCE;
    }

    //Draw Y Axis Marks
    while (markY < height/2) {
        int i;
        for (i=1; i < 3; i++) {
            int80(9, width/2 + i, height/2 + markY, 0, 0);
            int80(9, width/2 - i, height/2 + markY, 0, 0);
            int80(9, width/2 + i, height/2 - markY, 0, 0);
            int80(9, width/2 - i, height/2 - markY, 0, 0);
        }

        markY += DISTANCE;
    }

		printf("Y(X) = ");
		printf(a == 0 ? "" : a == 1 ? "X^2" : "%.1fX^2", a);
		printf(a != 0 && b != 0 ? " + " : "");
		printf(b == 0 ? "" : b == 1 ? "X" : "%.1fX", b);
		printf((a != 0 || b != 0) && c != 0 ? " + " : "");
		printf((a != 0 || b != 0) && c == 0 ? "" : "%.1f", c);
		printf(". Press enter to exit.");


    while(scanf("%s", &buffer) != 1);
		int80(2, 0, 0, 0, 0);

  	return 0;
}
