#include <string>
#include <vector>
#include <stdlib.h>

#include "pico_explorer.hpp"
#include "hardware/pwm.h"
#include "pico/multicore.h"

using namespace pimoroni;

uint16_t buffer[PicoExplorer::WIDTH * PicoExplorer::HEIGHT];
PicoExplorer pico_explorer(buffer);

Point screenPositionFromGrid_corner(Point &gridIndex, int &blockSize)
{
    int length = PicoExplorer::WIDTH / blockSize;
    float x = (float)gridIndex.x / (float)length * PicoExplorer::WIDTH;
    float y = (float)gridIndex.y / (float)length * PicoExplorer::HEIGHT;

    return Point((int)x, (int)y);
}

Point screenPositionFromGrid_centre(Point &gridIndex, int &blockSize)
{
    Point corner = screenPositionFromGrid_corner(gridIndex, blockSize);
    float x = corner.x + blockSize / 2;
    float y = corner.y + blockSize / 2;

    return Point((int)x, (int)y);
}

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
void createApple(Point &p, int &blockSize)
{
    Point pos = screenPositionFromGrid_centre(p, blockSize);
    int x = pos.x;
    int y = pos.y;

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

std::vector<std::vector<int>> initGrid(int &blockSize)
{
    int length = PicoExplorer::WIDTH / blockSize;
    return std::vector<std::vector<int>>(
        length,
        std::vector<int>(length));
}

Point createRandomGridPoint()
{
    int x = rand() % 23;
    int y = rand() % 23;
    return Point(x, y);
}

void playPointSound()
{
    pico_explorer.set_tone(440, 0.5);
    sleep_ms(100);
    pico_explorer.set_tone(840, 0.5);
    sleep_ms(100);
    pico_explorer.set_tone(0, 0);
}

int main()
{
    pico_explorer.init();
    pico_explorer.set_backlight(100);
    pico_explorer.set_audio_pin(pico_explorer.GP0);

    int blockSize = 10;

    Point gridPos(0, 0);
    Point snake = screenPositionFromGrid_corner(gridPos, blockSize);
    Point apple = createRandomGridPoint();

    int score = 0;
    bool appleEaten = false;

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
        snake = screenPositionFromGrid_corner(gridPos, blockSize);
        Rect segment(snake.x, snake.y, blockSize, blockSize);
        pico_explorer.set_pen(255, 255, 255);
        pico_explorer.rectangle(segment);

        // Decide apple location
        if (appleEaten)
        {
            apple = createRandomGridPoint();
            appleEaten = false;
        }
        createApple(apple, blockSize);

        if (pico_explorer.is_pressed(pico_explorer.A))
        {
            if (gridPos.x == 0)
                gridPos.x = (int)((float)PicoExplorer::WIDTH / (float)blockSize) - 1;
            else
                gridPos.x -= 1;
        }

        if (pico_explorer.is_pressed(pico_explorer.B))
        {
            if ((float)gridPos.y == (float)PicoExplorer::HEIGHT / (float)blockSize)
                gridPos.y = 0;
            else
                gridPos.y += 1;
        }

        if (pico_explorer.is_pressed(pico_explorer.X))
        {
            if ((float)gridPos.x == (float)PicoExplorer::WIDTH / (float)blockSize)
                gridPos.x = 0;
            else
                gridPos.x += 1;
        }

        if (pico_explorer.is_pressed(pico_explorer.Y))
        {
            if (gridPos.y == 0)
                gridPos.y = (int)((float)PicoExplorer::HEIGHT / (float)blockSize) - 1;
            else
                gridPos.y -= 1;
        }

        if ((apple.x == gridPos.x) && (apple.y == gridPos.y))
        {
            playPointSound();
            appleEaten = true;
            score += 1;
        }

        pico_explorer.update();
    }
}