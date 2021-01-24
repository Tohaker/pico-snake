#include <math.h>
#include <cstdlib>

#include "pico_explorer.hpp"

using namespace pimoroni;

uint16_t buffer[PicoExplorer::WIDTH * PicoExplorer::HEIGHT];
PicoExplorer pico_explorer(buffer);

int main()
{
    pico_explorer.init();

    int x = 0;
    int y = 0;
    int step = 5;

    while (true)
    {
        pico_explorer.clear();

        // Set background colour
        pico_explorer.set_pen(0, 0, 0);
        Rect screen(0, 0, PicoExplorer::WIDTH, PicoExplorer::HEIGHT);
        pico_explorer.rectangle(screen);

        // Set segment location
        Rect segment(x, y, 10, 10);
        pico_explorer.set_pen(255, 255, 255);
        pico_explorer.rectangle(segment);

        if (pico_explorer.is_pressed(pico_explorer.A))
        {
            if (x == 0)
                x = PicoExplorer::WIDTH;
            else
                x -= step;
        }

        if (pico_explorer.is_pressed(pico_explorer.B))
        {
            if (y == PicoExplorer::HEIGHT)
                y = 0;
            else
                y += step;
        }

        if (pico_explorer.is_pressed(pico_explorer.X))
        {
            if (x == PicoExplorer::WIDTH)
                x = 0;
            else
                x += step;
        }

        if (pico_explorer.is_pressed(pico_explorer.Y))
        {
            if (y == 0)
                y = PicoExplorer::HEIGHT;
            else
                y -= step;
        }

        pico_explorer.update();
    }
}