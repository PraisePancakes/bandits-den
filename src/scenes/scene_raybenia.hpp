#pragma once
#include "../scene_manager.hpp"
#include "../config/game_config.hpp"

namespace bden::scenes
{
    class scene_raybenia : public GameSceneType
    {
        config::GameConfig::WorldType &world;

    public:
        scene_raybenia(GameSceneManagerType &ctx, const std::string &sfp, config::GameConfig::WorldType &w) : Scene(ctx, sfp), world(w) {};
        bool on_init() override
        {
            this->load_circ_scene_data();

            return true;
        };
        // update npcs ect...
        void on_update(float dt) override {};
        // render npcs ect...
        void on_render() override
        {
            for (size_t i = 0; i < this->tiles.size(); i++)
            {
                auto pair = tiles[i];
                DrawRectangle(pair.second.x, pair.second.y, pair.second.width, pair.second.height, pair.first);
            }
        };

        void on_exit() override {};
        ~scene_raybenia() {};
    };
};