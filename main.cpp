#include <iostream>
#include "raylib.h"
#include "include/application.hpp"
#include <vector>

int main()
{
    srand(time(NULL));
    using namespace bden::config;
    // TO DO IMPLEMENT TAG SYSTEM FOR SNAKEECS WORLD POLICY
    bden::applicationlayer::application a(app_config::VIRTUAL_WIDTH, app_config::VIRTUAL_HEIGHT, app_config::WINDOW_TITLE, app_config::WINDOW_FLAGS);
    a.run();

    return 0;
}
