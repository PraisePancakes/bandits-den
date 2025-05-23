#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "../state_manager.hpp"
#include "../systems/drawables/system_ui.hpp"
#include "../systems/drawables/system_drawables.hpp"
#include "../systems/updateables/system_physics.hpp"
#include "../config/menu_config.hpp"
#include "../config/app_config.hpp"

#include "algorithm"
#include <string>
#include <cmath>

namespace bden::state
{
    using namespace components;
    class state_menu final : public bden::fsm::AppStateType
    {
        using world_policy = bden::config::MenuConfig::menu_configuration_policy;
        snek::world<world_policy> world;
        bden::systems::DrawableManager<world_policy> render_system; // for menu particles
        bden::systems::UIManager<world_policy> ui_system;           // for menu ui
        bden::systems::PhysicsManager<world_policy> physics_system;
        world_policy::entity_type title;
        world_policy::entity_type play;
        RenderTexture2D target;

        void init_menu_title()
        {

            auto e = world.spawn();
            title = e;
            Transform loc{};
            auto text_dim = MeasureTextEx(GetFontDefault(), "bandits-den", 40, 1);
            loc.translation.x = (GetScreenWidth() / 2) - (text_dim.x / 2);
            loc.translation.y = (GetScreenHeight() / 3) + (text_dim.y / 2);
            world.bind<TextComponent>(e, loc, "bandits-den", RED, 40);

            // backtext
            auto e2 = world.spawn();
            Transform loc2{};
            loc2.translation.x = (GetScreenWidth() / 2) - (text_dim.x / 2) + 2;
            loc2.translation.y = (GetScreenHeight() / 3) + (text_dim.y / 2) - 2;
            world.bind<TextComponent>(e2, loc2, "bandits-den...", BLACK, 40);
        };

        void init_menu_play_text()
        {
            auto title_text = world.get<TextComponent>(title);
            auto play_text = world.spawn();
            play = play_text;
            Transform loc{title_text->transform.translation.x + 50, title_text->transform.translation.y + 75};
            world.bind<TextComponent>(play_text, loc, "Press", BLACK, 20);
            // ENTER
            auto enter_text = world.spawn();
            Transform loc2{loc.translation.x + 75, loc.translation.y};
            world.bind<TextComponent>(enter_text, loc2, "ENTER", LIME, 20);
            // to play
            auto to_play_text = world.spawn();
            Transform loc3{loc2.translation.x + 85, loc2.translation.y};
            world.bind<TextComponent>(to_play_text, loc3, "to play", BLACK, 20);
        };

        void init_menu_quit_text()
        {
            auto play_text = world.get<TextComponent>(play);
            auto quit_text = world.spawn();

            Transform loc{play_text->transform.translation.x, play_text->transform.translation.y + 50};
            world.bind<TextComponent>(quit_text, loc, "Press", BLACK, 20);
            // ESC
            auto esc_text = world.spawn();
            Transform loc2{loc.translation.x + 75, loc.translation.y};
            world.bind<TextComponent>(esc_text, loc2, "ESC", RED, 20);
            // to play
            auto to_quit_text = world.spawn();
            Transform loc3{loc2.translation.x + 55, loc2.translation.y};
            world.bind<TextComponent>(to_quit_text, loc3, "to quit", BLACK, 20);
        };

        constexpr static int min_y_range = (config::AppConfig::VIRTUAL_HEIGHT / 8);
        constexpr static int max_y_range = (config::AppConfig::VIRTUAL_HEIGHT / 2);

        std::unordered_map<world_policy::entity_type, Vector2> stored_positions;

        void init_menu_particles()
        {

            for (size_t i = 0; i < config::MenuConfig::PARTICLE_COUNT * 2; i++)
            {
                auto e = world.spawn();

                Transform particle_transform{};
                particle_transform.translation.x = ((std::rand() % config::AppConfig::VIRTUAL_WIDTH) + 1);
                particle_transform.translation.y = (std::rand() % (min_y_range) + max_y_range);
                stored_positions[e] = {particle_transform.translation.x, particle_transform.translation.y};

                Vector2 particle_vel{};
                float particle_collider_r = 2.f;
                RigidBodyComponent rb{particle_transform, particle_vel, particle_collider_r, config::MenuConfig::PARTICLE_SPEED};
                Color particle_color(config::MenuConfig::PARTICLE_COLOR);

                world.bind<ParticleComponent>(e, rb, particle_color);
            }
        };

        void update_menu_particles()
        {
            auto particles = world.view<ParticleComponent>();

            particles.for_each([this](world_policy::entity_type e, ParticleComponent &pc)
                               {
                if(pc.rigidbody.transform.translation.x > config::AppConfig::VIRTUAL_WIDTH) {
                
                    pc.rigidbody.transform.translation.x = stored_positions[e].x - config::AppConfig::VIRTUAL_WIDTH;
                    pc.rigidbody.transform.translation.y = stored_positions[e].y;
                }
                pc.rigidbody.velocity.x = 1;
                pc.rigidbody.velocity.y = std::sin(DEG2RAD * pc.rigidbody.transform.translation.x); });
        };

        void init_menu_gui()
        {
            init_menu_title();
            init_menu_play_text();
            init_menu_quit_text();
        };

    public:
        state_menu(bden::fsm::AppStateManagerType *ctx, RenderTexture2D rt) : State(ctx), render_system(world), ui_system(world), physics_system(world), target(rt) {
                                                                              };

        bool on_init() override
        {
            init_menu_gui();
            init_menu_particles();
            return true;
        };

        void on_update(float dt) override
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                this->get_context()->set_state(bden::fsm::AppStateManagerType::states_type::STATE_GAME);
            }
            if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
            {
                this->get_context()->set_state(bden::fsm::AppStateManagerType::states_type::STATE_QUIT);
            }
            update_menu_particles();
            physics_system.update(dt);
        };

        void on_render() override
        {
            float scale = std::min((float)GetScreenWidth() / config::AppConfig::VIRTUAL_WIDTH, (float)GetScreenHeight() / config::AppConfig::VIRTUAL_HEIGHT);
            BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            render_system.render();
            ui_system.render();
            EndTextureMode();

            BeginDrawing();
            ClearBackground(BLACK);
            Rectangle src = {0.f, 0.f, (float)target.texture.width, (float)-target.texture.height};
            Vector2 dest_pos = {(GetScreenWidth() - ((float)config::AppConfig::VIRTUAL_WIDTH * scale)) * 0.5f, (GetScreenHeight() - ((float)config::AppConfig::VIRTUAL_HEIGHT * scale)) * 0.5f};
            Vector2 dest_dim = {(float)config::AppConfig::VIRTUAL_WIDTH * scale, (float)config::AppConfig::VIRTUAL_HEIGHT * scale};
            Rectangle dest = {
                dest_pos.x,
                dest_pos.y,
                dest_dim.x,
                dest_dim.y};
            Vector2 origin = {0, 0};

            DrawTexturePro(target.texture, src,
                           dest,
                           origin, 0.0f, WHITE);
            EndDrawing();
        };

        void on_exit() override {};

        ~state_menu() {};
    };
};