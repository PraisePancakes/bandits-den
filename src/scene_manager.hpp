#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/Circ/Circ/include/CFGLoader.hpp"
#include "systems/drawables/system_drawables.hpp"
#include "systems/drawables/system_ui.hpp"
#include "systems/updateables/system_ai.hpp"
#include "systems/updateables/system_camera.hpp"
#include "systems/updateables/system_health.hpp"
#include "systems/updateables/system_input.hpp"
#include "systems/updateables/system_physics.hpp"
#include "systems/updateables/system_weapons.hpp"
#include "config/game_config.hpp"

namespace bden::scenes
{
    namespace directory
    {
        static std::string raybenia = GetApplicationDirectory() + std::string("../data/scene_data/scene_raybenia.circ");
    }

    enum class SCENE_TILE_TEXTURES
    {
        TEXTURE_GRASS = 0,
        TEXTURE_STONE = 1,
        TEXTURE_BRICK = 2,
        TEXTURE_BUSH = 3
    };

    enum class SCENES
    {
        SCENE_NULL = -1,
        SCENE_RAYBENIA,
        SCENE_BENT_PINE,
        SCENE_MONTERREY,
        SCENE_LINJOHN,
        SCENE_LYNCHBURG
    };

    // pass scenes enum here
    template <typename T>
    class SceneManager;
    using namespace bden::config;
    using namespace bden::components;
    // binds class to enum type
    template <typename T>
    class Scene
    {

        SceneManager<T> &context;
        static_assert(std::is_enum_v<T>, "SceneManager requires an enum type");

        std::vector<std::pair<Color, Rectangle>> tiles;
        constexpr static int tile_width = 128;
        constexpr static int tile_height = 128;

        const std::string scene_file_path;
        bden::config::GameConfig::WorldType world;
        std::string scene_title;
        config::GameConfig::WorldType::entity_type player;
        GameConfig::WorldType::entity_type test;
        // systems
        systems::CameraManager<GameConfig::WorldType::world_policy> camera_system;
        systems::PhysicsManager<GameConfig::WorldType::world_policy> physics_system;
        systems::InputManager<GameConfig::WorldType::world_policy> input_system;
        systems::HealthManager<GameConfig::WorldType::world_policy> health_system;
        systems::AiManager<GameConfig::WorldType::world_policy> ai_system;
        systems::DrawableManager<GameConfig::WorldType::world_policy> render_system;
        systems::WeaponManager<GameConfig::WorldType::world_policy> weapon_system;

        GameConfig::WorldType::entity_type spawn_player(float w, float h, float x, float y)
        {
            using namespace config;
            auto p = world.spawn(GameConfig::TagEnum::TAG_PLAYER);
            SquareComponent square{w, h, x, y, PlayerConfig::PLAYER_COLOR, 0.0};
            world.bind<SquareComponent>(p, square);
            Transform player_transform{};
            player_transform.translation.x = x;
            player_transform.translation.y = y;

            const auto c = world.bind<CircleComponent>(p, square.rect.width, PlayerConfig::PLAYER_GLOW);
            Vector2 health_bar_pos(player_transform.translation.x, player_transform.translation.y - c.radius);
            Vector2 health_bar_size(w, 15.f);
            Rectangle health_bar_rect(health_bar_pos.x, health_bar_pos.y, health_bar_size.x, health_bar_size.y);
            world.bind<HealthComponent>(p, 100, health_bar_rect);
            world.bind<RigidBodyComponent>(p, player_transform, Vector2(0, 0), c.radius, PlayerConfig::PLAYER_SPEED);

            Vector2 weapon_ray{x, y};
            world.bind<WeaponComponent>(p, w * WeaponConfig::spear::radius, WeaponConfig::spear::damage, WeaponConfig::spear::speed, WeaponConfig::spear::color, weapon_ray);
            return p;
        };

        GameConfig::WorldType::entity_type spawn_test(float w, float h, float x, float y)
        {
            auto test = world.spawn(GameConfig::TagEnum::TAG_ENEMIES);
            using namespace config;

            SquareComponent square{w, h, x, y, EnemyConfig::ENEMY_COLOR, 0.0};
            world.bind<SquareComponent>(test, square);
            Transform test_transform{};

            test_transform.translation.x = x;
            test_transform.translation.y = y;
            Vector2 weapon_ray{x, y};
            world.bind<AggroComponent>(test, w * 3, false);
            world.bind<WeaponComponent>(test, w * WeaponConfig::spear::radius, WeaponConfig::spear::damage, WeaponConfig::spear::speed, WeaponConfig::spear::color, weapon_ray);
            auto c = world.bind<CircleComponent>(test, square.rect.width, EnemyConfig::ENEMY_GLOW);
            world.bind<RigidBodyComponent>(test, test_transform, Vector2(0, 0), c.radius, EnemyConfig::ENEMY_SPEED);

            Vector2 health_bar_pos(test_transform.translation.x, test_transform.translation.y - c.radius);
            Vector2 health_bar_size(w, 15.f);
            Rectangle health_bar_rect(health_bar_pos.x, health_bar_pos.y, health_bar_size.x, health_bar_size.y);
            world.bind<HealthComponent>(test, 100, health_bar_rect);

            return test;
        };

        std::vector<GameConfig::WorldType::entity_type> to_delete;
        void system_updateables_delete_entities()
        {
            for (auto &e : to_delete)
            {
                world.kill(e);
            }
            to_delete.clear();
        }

        Color get_tile_color(SCENE_TILE_TEXTURES value) const
        {
            switch (value)
            {
            case SCENE_TILE_TEXTURES::TEXTURE_GRASS:
                return GREEN;
            case SCENE_TILE_TEXTURES::TEXTURE_BUSH:
                return DARKGREEN;
            case SCENE_TILE_TEXTURES::TEXTURE_BRICK:
                return DARKBROWN;
            case SCENE_TILE_TEXTURES::TEXTURE_STONE:
                return DARKGRAY;
            default:
                return RAYWHITE;
            }
        };

        void load_circ_scene_data()
        {
            Circ::CFGLoader cfgl(scene_file_path);
            auto grid_outer = cfgl.CFGAttr<std::vector<std::any>>({"scene_data", "grid"});
            scene_title = cfgl.CFGAttr<std::string>({"scene_data", "title"});

            for (size_t i = 0; i < grid_outer.size(); i++)
            {
                auto grid_inner = std::any_cast<std::vector<std::any>>(grid_outer[i]);
                for (size_t j = 0; j < grid_inner.size(); j++)
                {

                    auto el = std::any_cast<int>(grid_inner[j]);
                    float xpos = i * tile_width;
                    float ypos = j * tile_height;
                    Rectangle tile_rect(xpos, ypos, tile_width, tile_height);

                    std::pair<Color, Rectangle> tile_data = std::make_pair(get_tile_color((SCENE_TILE_TEXTURES)el), tile_rect);
                    tiles.push_back(tile_data);
                }
            }
        };

        void render_scene_intro()
        {
            DrawText(scene_title.c_str(), camera_system.get_camera().target.x / 2, camera_system.get_camera().target.y / 2, 128, WHITE);
        };

        void render_scene_tiles()
        {
            for (std::pair<Color, Rectangle> tile_data : tiles)
            {
                DrawRectangle(tile_data.second.x, tile_data.second.y, tile_data.second.width, tile_data.second.height, tile_data.first);
            }
        };

    public:
        bool is_active = false;
        using type = T;
        Scene(SceneManager<T> &ctx, const std::string sfp)
            : context(ctx),
              scene_file_path(sfp),
              camera_system(world),
              physics_system(world),
              input_system(world),
              health_system(world, to_delete),
              ai_system(world), render_system(world), weapon_system(world) {};

        void on_init()
        {
            this->player = (spawn_player(100, 100, 500, 500));
            this->test = (spawn_test(100, 100, 200, 200));

            this->load_circ_scene_data();
            is_active = true;
        };
        void on_update(float dt)
        {
            if (!world.contains(player))
                return;

            physics_system.update(dt);
            camera_system.update(dt, player);
            input_system.update(player, camera_system);
            health_system.update(dt, player);
            ai_system.update(dt, player);
            weapon_system.update(dt, player);
            system_updateables_delete_entities();
        };
        void on_render()
        {
            float scale = std::min((float)GetScreenWidth() / AppConfig::VIRTUAL_WIDTH, (float)GetScreenHeight() / AppConfig::VIRTUAL_HEIGHT);
            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera_system.get_camera());
            render_scene_tiles();
            render_system.render();
            render_scene_intro();
            EndMode2D();
            EndDrawing();
        };

        void on_exit()
        {
            is_active = false;
        };

        SceneManager<T> *get_context() const
        {
            return context;
        };
        ~Scene() {};
    };

    template <typename SCENES_ENUM>
    class SceneManager
    {
    public:
        using scenes_type = SCENES_ENUM;

    private:
        static_assert(std::is_enum_v<scenes_type>, "SceneManager requires an enum type");
        std::unordered_map<scenes_type, Scene<scenes_type> *> scenes;
        Scene<scenes_type> *current_scene;

    public:
        SceneManager() {};

        void insert_scene(scenes_type id, Scene<scenes_type> *s)
        {
            scenes.insert_or_assign(id, s);
        };

        void set_scene(scenes_type id)
        {
            if (current_scene)
            {
                current_scene->is_active = false;
                current_scene->on_exit();
            }

            current_scene = scenes[id];
            current_scene->is_active = true;
            current_scene->on_init();
        };

        void update_current_scene(float dt)
        {
            current_scene->on_update(dt);
        }

        void render_current_scene()
        {
            current_scene->on_render();
        }

        scenes_type get_current_scene_type() const
        {
            for (auto &s : scenes)
            {
                if (s.second == current_scene)
                {
                    return s.first;
                }
            }
            return scenes::SCENES::SCENE_NULL;
        }

        Scene<scenes_type> *get_scene(scenes_type id)
        {
            return scenes[id];
        }

        Scene<scenes_type> *get_current_scene() const
        {
            return current_scene;
        }
    };
    using GameSceneManagerType = bden::scenes::SceneManager<bden::scenes::SCENES>;
    using GameSceneType = bden::scenes::Scene<bden::scenes::SCENES>;
};