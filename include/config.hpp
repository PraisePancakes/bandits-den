#pragma once

namespace bden::gamelayer::internal::config
{
    namespace player
    {
        constexpr static float PLAYER_SPEED = 250.f;
    } // player
    namespace enemies
    {
        constexpr static float ENEMY_SPEED = player::PLAYER_SPEED / 2;
    } // enemies

    namespace ui
    {
        namespace weapons
        {
            constexpr static float START_ANGLE = 0.f;
            constexpr static float END_ANGLE = 360.f;
            constexpr static int NSEGS = 10;
        } // weapons
    } // ui

};