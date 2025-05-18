#pragma once
#include <iostream>
#include "raylib.h"

namespace bden::config::AppConfig
{

    constexpr static int VIRTUAL_WIDTH{1280};
    constexpr static int VIRTUAL_HEIGHT{720};
    constexpr static int WINDOW_FLAGS{FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI};
    constexpr static const char *WINDOW_TITLE = "bandits-den";

}