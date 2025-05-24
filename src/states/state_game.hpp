#pragma once
#include <iostream>
#include "raylib.h"
#include "../config/player_config.hpp"
#include "../config/game_config.hpp"
#include "../config/weapon_config.hpp"
#include "../config/app_config.hpp"
#include "../config/enemy_config.hpp"

#include "raymath.h"
#include "../state_manager.hpp"
#include "../scene_manager.hpp"
#include <fstream>
#include <filesystem>
#include "algorithm"
#include <string>
#include <cmath>
#include "rlgl.h"

namespace bden::state
{
    using namespace components;
    using namespace config;

    // TODO create entity contracts, player must have component_set x,y,z... , enemy must have component_set y, z... etc.

    class state_game final : public bden::fsm::AppStateType
    {

        // managers
        bden::scenes::GameSceneManagerType scene_manager;

        void init_scenes()
        {
            bden::scenes::GameSceneType *raybenia = new bden::scenes::GameSceneType(scene_manager, scenes::directory::raybenia);
            scene_manager.insert_scene(scenes::SCENES::SCENE_RAYBENIA, raybenia);
            scene_manager.set_scene(scenes::SCENES::SCENE_RAYBENIA);
        };

    public:
        state_game(bden::fsm::StateManager<bden::fsm::states::APP_STATES> *ctx, RenderTexture2D &render_target) : State(ctx),
                                                                                                                  scene_manager() {

                                                                                                                  };

        bool on_init() override
        {
            init_scenes();

            return true;
        };

        void on_update(float dt) override
        {
            if (WindowShouldClose())
            {
                this->get_context()->set_state(bden::fsm::AppStateManagerType::states_type::STATE_QUIT);
            }

            scene_manager.get_current_scene()->on_update(dt);
        };

        void on_render() override
        {

            scene_manager.get_current_scene()->on_render();
        };

        void on_exit() override
        {
            scene_manager.get_current_scene()->on_exit();
        };

        ~state_game() = default;
    };
};