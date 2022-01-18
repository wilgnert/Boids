#include "../headers/Game.hpp"

int main()
{
    const char * window_title = "TP Boids";
    const int window_width = 1280;
    const int window_height = 720;
    int gl_major_version = 4;
    int gl_minor_version = 6;

    Game game(
        window_title,
        window_width,
        window_height,
        gl_major_version,
        gl_minor_version,
        true
    );


    /* Loop until the user closes the window */
    while (!game.getWindowShouldClose())
    {
        game.update();
        game.render();
    }

    return 0;
}