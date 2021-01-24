#include <string>
#include <stdlib.h>

#include "pico_explorer.hpp"

using namespace pimoroni;

uint16_t buffer[PicoExplorer::WIDTH * PicoExplorer::HEIGHT];
PicoExplorer pico_explorer(buffer);

/**
 * Draw an apple at the given coordinates.
 *     GGG
 *     G
 *     G 
 * RRRRRRRRRR
 * RRRRRRRRRR
 * RRRRRRRRRR
 * RRRRRRRRRR
 * RRRRRRRRRR
 */
void createApple(Point &p)
{
    int x = p.x;
    int y = p.y;

    // Apple core
    pico_explorer.set_pen(214, 58, 47);
    pico_explorer.circle(Point(x, y), 5);

    // Stem and Leaf colour
    pico_explorer.set_pen(11, 110, 26);

    // Stem
    Rect stem(x, y - 8, 1, 3);
    pico_explorer.rectangle(stem);

    // Leaf
    Rect leaf(x, y - 8, 3, 1);
    pico_explorer.rectangle(leaf);
}

Point createRandomPoint()
{
    int x = rand() % (PicoExplorer::WIDTH - 9) + 10;
    int y = rand() % (PicoExplorer::HEIGHT - 9) + 10;
    return Point(x, y);
}

int main()
{
    pico_explorer.init();
    pico_explorer.set_backlight(100);
    pico_explorer.set_audio_pin(pico_explorer.GP0);

    int x = 0;
    int y = 0;

    bool appleEaten = false;
    Point apple = createRandomPoint();

    int step = 5;
    int score = 0;

    while (true)
    {
        pico_explorer.clear();

        // Set background colour
        pico_explorer.set_pen(0, 0, 0);
        Rect screen(0, 0, PicoExplorer::WIDTH, PicoExplorer::HEIGHT);
        pico_explorer.rectangle(screen);

        // Display the current score
        pico_explorer.set_pen(255, 255, 255);
        pico_explorer.text(std::to_string(score), Point(PicoExplorer::WIDTH - 30, 5), false);

        // Set segment location
        Rect segment(x, y, 10, 10);
        pico_explorer.set_pen(255, 255, 255);
        pico_explorer.rectangle(segment);

        // Decide apple location
        if (appleEaten)
        {
            apple = createRandomPoint();
            appleEaten = false;
        }
        createApple(apple);

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

        if ((x + 10 >= apple.x) && (x <= apple.x) && (y + 10 >= apple.y) && (y <= apple.y))
        {
            pico_explorer.set_tone(440, 0.5);
            sleep_ms(100);
            appleEaten = true;
            score += 1;
            pico_explorer.set_tone(0, 0);
        }

        pico_explorer.update();
    }
}