#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "app_observer.hpp"
#include "state_manager.hpp"
#include "algorithm"
#include <string>
#include <cmath>

namespace bden::state
{
    class state_menu final : public applicationlayer::application_observer, public AppStateType
    {
        int screen_width = 0;
        int screen_height = 0;
        void update_app_listener(int w, int h) override
        {
            screen_width = w;
            screen_height = h;
        }
        snek::world<bden::config::menu_config::menu_configuration_policy> world;

    public:
        state_menu(AppStateManagerType *ctx) : State(ctx) {

                                               };
        void on_update(float dt) override
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                this->get_context()->set_state(AppStateManagerType::states_type::STATE_GAME);
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
