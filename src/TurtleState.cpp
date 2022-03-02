#include "TurtleState.hpp"

turtle::TurtleState::TurtleState() {
    color = new TurtleColor();
    location = new TurtleLocation();
    angle = 0;
    pen = false;
    pen_width = 1.0f;
    id = "0";
    name ="turtle";
    last = 0;
    font_str = "";
    canvas_size = new TurtleDimension();
}