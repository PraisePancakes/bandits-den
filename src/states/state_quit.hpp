#pragma once
#include "../state_manager.hpp"
#include "raylib.h"

namespace bden::state
{
    class state_quit : public bden::fsm::AppStateType
    {

    public:
        state_quit(bden::fsm::AppStateManagerType *ctx) : State(ctx) {};
        bool on_init() override
        {
            return true;
        };
        void on_update(float dt) override {

        };
        void on_render() override {

        };
        void on_exit() override
        {
            // serialize game state;

            CloseWindow();
        };
        ~state_quit() {};
    };

};