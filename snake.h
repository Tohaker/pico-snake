#include "pico_explorer.hpp"

using namespace pimoroni;

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

class Snake
{
private:
    Point grid_position;
    Direction direction;

public:
    Snake(Point grid_position, Direction direction);
    ~Snake();

    Point get_grid_position();
    Direction get_direction();
    void set_direction(Direction new_direction);
    void set_grid_position(Point new_grid_position);
};