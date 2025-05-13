#include <iostream>
#include "raylib.h"
#include "include/application.hpp"
#include <vector>

const int w = 1280;
const int h = 960;

int main()
{
    bden::applicationlayer::application a(w, h, "BanditsDen", FLAG_WINDOW_RESIZABLE);
    a.run();

    return 0;
}
