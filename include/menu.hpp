#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"

#include "state_manager.hpp"
#include "algorithm"
#include <string>
#include <cmath>

namespace bden::state
{
    class state_menu final : public AppStateType
    {

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
