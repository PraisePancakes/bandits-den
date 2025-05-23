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
            this->load_circ_file_path();

            return true;
        };
        void on_update(float dt) override {};
        void on_render() override
        {
            for (size_t i = 0; i < this->get_tile_map().size(); i++)
            {
                for (size_t j = 0; j < this->get_tile_map()[i].size(); j++)
                {
                    std::cout << get_tile_map()[i][j];
                }
            }
        };
        void on_exit() override {};
        ~scene_raybenia() {};
    };
};