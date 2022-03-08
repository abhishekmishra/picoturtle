#include "SkiaTurtle.hpp"
#include "SkiaTurtleLua.hpp"

int main(int argc, char *argv[])
{
    lua_State *L;

    // initialize the turtle lua binding with args
    initTurtleLuaBinding(&L, argc, argv);

    // cleanup the turtle lua binding
    cleanupTurtleLuaBinding(L);
    
    return 0;
}

// void turtle_example()
// {
// turtle::SkiaTurtle *skTurtle = new turtle::SkiaTurtle();

// skTurtle->pendown();
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->stop();

// skTurtle->export_img("out/out_raster.png");

// delete skTurtle;
// }