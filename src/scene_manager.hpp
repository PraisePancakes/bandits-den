#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/Circ/Circ/include/CFGLoader.hpp"

namespace bden::scenes
{

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
        std::vector<std::vector<int>> tile_map;
        const std::string scene_file_path;

    protected:
        constexpr static int tile_width = 16;
        constexpr static int tile_height = 16;

        void load_circ_file_path()
        {
            Circ::CFGLoader cfgl(scene_file_path);
            auto grid_outer = cfgl.CFGAttr<std::vector<std::any>>({"scene_data", "grid"});

            for (size_t i = 0; i < grid_outer.size(); i++)
            {
                auto grid_inner = std::any_cast<std::vector<std::any>>(grid_outer[i]);
                std::vector<int> row;
                for (size_t j = 0; j < grid_inner.size(); j++)
                {
                    auto el = std::any_cast<int>(grid_inner[j]);
                    row.push_back(el);
                }
                tile_map.push_back(row);
            }
        };
        const std::vector<std::vector<int>> get_tile_map() const
        {
            return tile_map;
        };

    public:
        using type = T;
        Scene(SceneManager<T> &ctx, const std::string sfp) : context(ctx), scene_file_path(sfp) {};
        virtual bool on_init() = 0;
        virtual void on_update(float) = 0;
        virtual void on_render() = 0;
        virtual void on_exit() = 0;
        bool is_active = false;
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