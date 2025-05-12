#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "app_observer.hpp"
#include "algorithm"
#include <string>
#include <cmath>

namespace bden::gamelayer
{
    class state_menu final : public applicationlayer::application_observer
    {
        int screen_width = 0;
        int screen_height = 0;
        void update_app_listener(int w, int h) override
        {
            screen_width = w;
            screen_height = h;
        }

    public:
        state_menu(applicationlayer::application_subject *ctx) : application_observer(ctx) {

                                                                 };
        void on_update(float dt) override
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                this->context->set_state(applicationlayer::ApplicationState::STATE_GAME);
            }
        };

        void on_render() override
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            EndDrawing();
        };

        ~state_menu() {};
    };
};
