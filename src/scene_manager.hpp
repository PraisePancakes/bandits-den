#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/Circ/Circ/include/CFGLoader.hpp"

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
        bden::config::GameConfig::WorldType &world;

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

    public:
        bool is_active = false;
        using type = T;
        Scene(SceneManager<T> &ctx, const std::string sfp, config::GameConfig::WorldType w) : context(ctx), scene_file_path(sfp), world(w) {};
        void on_init()
        {
            this->load_circ_scene_data();
            is_active = true;
        };
        void on_update(float dt) {};
        void on_render()
        {
            for (std::pair<Color, Rectangle> tile_data : tiles)
            {
                DrawRectangle(tile_data.second.x, tile_data.second.y, tile_data.second.width, tile_data.second.height, tile_data.first);
            }
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