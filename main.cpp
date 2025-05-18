#include <iostream>
#include "raylib.h"
#include "include/application.hpp"
#include "include/config/app_config.hpp"
#include <vector>

int main()
{
    srand(time(NULL));
    using namespace bden::config;
    // TO DO IMPLEMENT TAG SYSTEM FOR SNAKEECS WORLD POLICY
    bden::applicationlayer::application a(AppConfig::VIRTUAL_WIDTH, AppConfig::VIRTUAL_HEIGHT, AppConfig::WINDOW_TITLE, AppConfig::WINDOW_FLAGS);
    a.run();

    return 0;
}
