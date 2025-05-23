#pragma once
#include <iostream>
#include "raylib.h"

namespace bden::scenes
{

    // check texture mask for each unit of the map and render each flagged texture.
    enum class SCENE_TEXTURES
    {
        TEXTURE_GRASS = 0x1,
        TEXTURE_STONE = 0x2,
        TEXTURE_BRICK = 0x4,
        TEXTURE_BUSH = 0x8
    };

    enum class SCENES
    {
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
        SceneManager<T> *context;
        static_assert(std::is_enum_v<T>, "SceneManager requires an enum type");

    public:
        using type = T;
        Scene(SceneManager<T> *ctx) : context(ctx) {};
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

    template <typename T>
    class SceneManager
    {
    public:
        using scenes_type = SCENES_ENUM;

    private:
        static_assert(std::is_enum_v<scenes_type>, "SceneManager requires an enum type");
        std::unordered_map<states_type, Scene<scenes_type> *> scenes;
        Scene<scenes_type> *current_scene;

    public:
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
            return scenes::GAME_SCENES::SCENE_NULL;
        }

        Scene<scenes_type> *get_scene(STATE_ENUM id)
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