#pragma once
#include "internal.hpp"
#include "raylib.h"
#include "app_observer.hpp"
#include "rlgl.h"
#include "raymath.h"

namespace bden::gamelayer
{
    using namespace internal;
    using namespace components;

    class world_camera final
    {
        Camera2D cam{};
        int screen_width = 0;
        int screen_height = 0;

        snek::world<configuration_policy> &world;

    public:
        world_camera(snek::world<configuration_policy> &w) : world(w) {};

        void update_app_listener(int w, int h)
        {
            screen_width = w;
            screen_height = h;
        }

        void update(float dt, configuration_policy::entity_type player)
        {
            const float LERP_FACTOR = 5;
            auto &rb = world.get_ref<RigidBodyComponent>(player);
            const auto &ptc = rb.transform;
            auto lerpx = Lerp(cam.target.x, ptc.translation.x, dt * LERP_FACTOR);
            auto lerpy = Lerp(cam.target.y, ptc.translation.y, dt * LERP_FACTOR);

            cam.target = {lerpx, lerpy};
            cam.offset = {screen_width / 2.0f, screen_height / 2.0f};
            cam.rotation = 0.0f;
            cam.zoom = 1.0f;
        }

        const Camera2D &get_camera() const
        {
            return this->cam;
        }
    };
}