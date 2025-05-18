#pragma once
#include "state_manager.hpp"

namespace bden::state
{
    class state_quit : public bden::fsm::State<bden::fsm::states::APP_STATES>
    {

    public:
        state_quit(bden::fsm::StateManager<bden::fsm::states::APP_STATES> *ctx) : State(ctx) {};
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