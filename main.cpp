#include <string>
#include <vector>
#include <stdlib.h>

#include "pico_explorer.hpp"
#include "snake.h"

using namespace pimoroni;

uint16_t buffer[PicoExplorer::WIDTH * PicoExplorer::HEIGHT];
PicoExplorer pico_explorer(buffer);

Point screen_position_from_grid__corner(Point gridIndex, int &block_size)
{
    int length = PicoExplorer::WIDTH / block_size;
    float x = (float)gridIndex.x / (float)length * PicoExplorer::WIDTH;
    float y = (float)gridIndex.y / (float)length * PicoExplorer::HEIGHT;

    return Point((int)x, (int)y);
}

Point screen_position_from_grid__centre(Point gridIndex, int &block_size)
{
    Point corner = screen_position_from_grid__corner(gridIndex, block_size);
    float x = corner.x + block_size / 2;
    float y = corner.y + block_size / 2;

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
void create_apple(Point &p, int &block_size)
{
    Point pos = screen_position_from_grid__centre(p, block_size);
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

std::vector<std::vector<int>> init_grid(int &block_size)
{
    int length = PicoExplorer::WIDTH / block_size;
    return std::vector<std::vector<int>>(
        length,
        std::vector<int>(length));
}

Point create_random_grid_point()
{
    int x = rand() % 23;
    int y = rand() % 23;
    return Point(x, y);
}

void play_point_sound()
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

    int block_size = 10;
    int screen_end = (int)((float)PicoExplorer::WIDTH / (float)block_size);
    int screen_bottom = (int)((float)PicoExplorer::HEIGHT / (float)block_size);

    int middle_of_screen = (int)(((float)PicoExplorer::WIDTH / (float)block_size) / 2.);
    Point head_pos(middle_of_screen, middle_of_screen);
    Direction head_dir = Direction::DOWN;
    Snake head(head_pos, head_dir);

    std::vector<Snake> full_snake = {head};
    Point apple = create_random_grid_point();

    int score = 0;
    bool apple_eaten = false;

    while (true)
    {
        sleep_ms(500);
        pico_explorer.clear();

        // Set background colour
        pico_explorer.set_pen(0, 0, 0);
        Rect screen(0, 0, PicoExplorer::WIDTH, PicoExplorer::HEIGHT);
        pico_explorer.rectangle(screen);

        // Display the current score
        pico_explorer.set_pen(255, 255, 255);
        pico_explorer.text(std::to_string(score), Point(PicoExplorer::WIDTH - 30, 5), false);

        // Set segments location
        std::vector<Snake>::iterator it;
        for (it = full_snake.begin(); it != full_snake.end(); it++)
        {
            Point snake = screen_position_from_grid__corner(it->get_grid_position(), block_size);
            Rect segment(snake.x, snake.y, block_size, block_size);
            pico_explorer.set_pen(255, 255, 255);
            pico_explorer.rectangle(segment);
        }

        // Decide apple location
        if (apple_eaten)
        {
            apple = create_random_grid_point();
            apple_eaten = false;
        }
        create_apple(apple, block_size);

        Point prev = full_snake.at(0).get_grid_position();
        Direction dir = full_snake.at(0).get_direction();
        bool has_turned = false;

        if (pico_explorer.is_pressed(pico_explorer.A))
        {
            if ((dir != Direction::RIGHT) && (dir != Direction::LEFT))
            {
                has_turned = true;
                full_snake.at(0).set_direction(Direction::LEFT);
                if (prev.x == screen_end)
                    full_snake.at(0).set_grid_position(Point(0, prev.y));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x - 1, prev.y));
            }
        }

        if (pico_explorer.is_pressed(pico_explorer.B))
        {
            if ((dir != Direction::UP) && (dir != Direction::DOWN))
            {
                has_turned = true;
                full_snake.at(0).set_direction(Direction::DOWN);
                if (prev.y == screen_bottom)
                    full_snake.at(0).set_grid_position(Point(prev.x, 0));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x, prev.y + 1));
            }
        }

        if (pico_explorer.is_pressed(pico_explorer.X))
        {
            if ((dir != Direction::RIGHT) && (dir != Direction::LEFT))
            {
                has_turned = true;
                full_snake.at(0).set_direction(Direction::RIGHT);
                if (prev.x == 0)
                    full_snake.at(0).set_grid_position(Point(screen_end - 1, prev.y));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x + 1, prev.y));
            }
        }

        if (pico_explorer.is_pressed(pico_explorer.Y))
        {
            if ((dir != Direction::UP) && (dir != Direction::DOWN))
            {
                has_turned = true;
                full_snake.at(0).set_direction(Direction::UP);
                if (prev.y == 0)
                    full_snake.at(0).set_grid_position(Point(prev.x, screen_bottom - 1));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x, prev.y - 1));
            }
        }

        if (!has_turned)
        {
            switch (dir)
            {
            case Direction::UP:
                if (prev.y == 0)
                    full_snake.at(0).set_grid_position(Point(prev.x, screen_bottom - 1));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x, prev.y - 1));
                break;
            case Direction::DOWN:
                if (prev.y == screen_bottom)
                    full_snake.at(0).set_grid_position(Point(prev.x, 0));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x, prev.y + 1));
                break;
            case Direction::LEFT:
                if (prev.x == 0)
                    full_snake.at(0).set_grid_position(Point(screen_end - 1, prev.y));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x - 1, prev.y));
                break;
            case Direction::RIGHT:
                if (prev.x == screen_end)
                    full_snake.at(0).set_grid_position(Point(0, prev.y));
                else
                    full_snake.at(0).set_grid_position(Point(prev.x + 1, prev.y));
                break;
            default:
                break;
            }
        }

        if ((apple.x == head_pos.x) && (apple.y == head_pos.y))
        {
            play_point_sound();
            apple_eaten = true;
            score += 1;
        }

        pico_explorer.update();
    }
}