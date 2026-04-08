#include "gsl.h"
#include <cstdlib>   // для rand(), srand()
#include <ctime>     // для time()
#include <math.h>

int main() {
    int width = 800;
    int height = 600;

    int x = width / 2;
    int y = height / 2;

    srand(time(NULL));

    gsl_init("Test", width, height);
    while (gsl_handle_events()) {
        int color = rand() << 9;
        int xOffset = (rand() % 5)-2;
        int yOffset = (rand() % 5)-2;

        gsl_fill(color);
        gsl_draw_text(x, y, "LOL", oppositeColor(color));

        x = min(max(x + xOffset, 0), width);
        y = min(max(y + yOffset, 0), height);
    }
    gsl_shutdown();
    return 0;
}

int oppositeColor(int color) {
    int r = (color >> 0) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = (color >> 16) & 0xFF;

    return (b>>16)|(g<<8)|(r<<8);
}