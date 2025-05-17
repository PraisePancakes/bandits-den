#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "system_camera.hpp"
#include "../../config.hpp"

namespace bden::systems
{

    using namespace components;

    template <typename WorldPolicy>
    class InputManager final
    {

        snek::world<WorldPolicy> &world;

        void system_updateables_input_player_keys(WorldPolicy::entity_type player)
        {
            // update players velocity/movement
            auto &rb = world.template get_ref<RigidBodyComponent>(player);

            auto &vel = rb.velocity;
            vel.x = 0;
            vel.y = 0;
            // dir
            if (IsKeyDown(KEY_W))
            {
                vel.y -= 1;
            }
            if (IsKeyDown(KEY_A))
            {
                vel.x -= 1;
            }
            if (IsKeyDown(KEY_S))
            {
                vel.y += 1;
            }
            if (IsKeyDown(KEY_D))
            {
                vel.x += 1;
            }
        };

        void system_updateables_input_player_mouse(WorldPolicy::entity_type player, CameraManager<WorldPolicy> &camera_system)
        {
            auto &rb = world.template get_ref<RigidBodyComponent>(player);
            auto &sc = world.template get_ref<SquareComponent>(player);

            auto &tc = rb.transform;
            const Vector2 abs_mouse_pos = GetMousePosition();
            const Vector2 rel_screen_mouse = GetScreenToWorld2D(abs_mouse_pos, camera_system.get_camera());
            const float x = rel_screen_mouse.x - tc.translation.x;
            const float y = rel_screen_mouse.y - tc.translation.y;

            float rad = atan2(x, y);
            float deg = (rad * 180.0) / PI;

            tc.rotation.x = -deg;
            rlPushMatrix();
            rlRotatef(tc.rotation.x, x, y, 0);
            rlPopMatrix();
        };

    public:
        InputManager(snek::world<WorldPolicy> &w) : world(w) {};

        void update(WorldPolicy::entity_type player, CameraManager<WorldPolicy> &camera_system)
        {
            if constexpr (WorldPolicy::template is_valid_component<RigidBodyComponent>())
            {
                system_updateables_input_player_keys(player);
            }
            if constexpr (WorldPolicy::template is_valid_component_set<RigidBodyComponent, SquareComponent>())
            {
                system_updateables_input_player_mouse(player, camera_system);
            }
        }
    };
}