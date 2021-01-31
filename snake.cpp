#include "snake.h"

using namespace pimoroni;

Snake::Snake(Point grid_position, Direction direction) : grid_position(grid_position), direction(direction) {}

Snake::~Snake() {}

Point Snake::get_grid_position()
{
    return this->grid_position;
}

Direction Snake::get_direction()
{
    return this->direction;
}

void Snake::set_direction(Direction new_direction)
{
    this->direction = new_direction;
}

void Snake::set_grid_position(Point new_grid_position)
{
    this->grid_position = new_grid_position;
}