#pragma once
#include "../../internal.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "system_camera.hpp"

namespace bden::gamelayer::systems
{
    using namespace internal;
    using namespace components;

    template <typename WorldPolicy>
    class InputManager final
    {

        snek::world<WorldPolicy> &world;

        void system_updateables_input_player_keys(WorldPolicy::entity_type player, float PLAYER_SPEED)
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
            // normalize velocity vector
            auto mag = std::sqrt(vel.x * vel.x + vel.y * vel.y);

            if (mag != 0.0)
            {
                vel.x = (vel.x / mag);
                vel.y = (vel.y / mag);
            }
            // apply speed to the velocity
            vel.x *= PLAYER_SPEED;
            vel.y *= PLAYER_SPEED;
        };

        void system_updateables_input_player_mouse(WorldPolicy::entity_type player, CameraManager<WorldPolicy> &camera_system)
        {
            auto &rb = world.template get_ref<RigidBodyComponent>(player);
            auto &sc = world.template get_ref<SquareComponent>(player);

            auto &tc = rb.transform;
            auto &ang = tc.rotation.x;
            const Vector2 abs_mouse_pos = GetMousePosition();
            const Vector2 rel_screen_mouse = GetScreenToWorld2D(abs_mouse_pos, camera_system.get_camera());
            const float x = rel_screen_mouse.x - tc.translation.x;
            const float y = rel_screen_mouse.y - tc.translation.y;

            float rad = atan2(x, y);
            float deg = (rad * 180.0) / PI;

            ang = -deg;
            rlPushMatrix();
            rlRotatef(ang, x, y, 0);
            rlPopMatrix();
        };

    public:
        InputManager(snek::world<WorldPolicy> &w) : world(w) {};

              void update(WorldPolicy::entity_type player, CameraManager<WorldPolicy> &camera_system, float PLAYER_SPEED)
        {
            system_updateables_input_player_keys(player, PLAYER_SPEED);
            system_updateables_input_player_mouse(player, camera_system);
        }
    };
}