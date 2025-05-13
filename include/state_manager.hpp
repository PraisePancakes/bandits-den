#pragma once
#include <iostream>
#include <list>

namespace bden::fsm
{
    namespace states
    {
        enum APP_STATES
        {
            STATE_NULL = -1,
            STATE_MENU,
            STATE_GAME,
            STATE_PAUSE
        };
    };

    template <typename T>
    class StateManager;

    template <typename T>
    class State
    {
        StateManager<T> *context;
        static_assert(std::is_enum_v<T>, "StateManager requires an enum type");

    public:
        State(StateManager<T> *ctx) : context(ctx) {};
        virtual void on_update(float) = 0;
        virtual void on_render() = 0;
        StateManager<T> *get_context() const
        {
            return context;
        };
        ~State() {};
    };

    template <typename STATE_ENUM>
    class StateManager
    {
    public:
        using states_type = STATE_ENUM;

    private:
        static_assert(std::is_enum_v<STATE_ENUM>, "StateManager requires an enum type");
        std::unordered_map<states_type, State<STATE_ENUM> *> states;
        State<STATE_ENUM> *current_state;

    public:
        void insert_state(STATE_ENUM id, State<STATE_ENUM> *s)
        {
            states.insert_or_assign(id, s);
        };

        void set_state(STATE_ENUM id)
        {
            current_state = states[id];
        };

        State<STATE_ENUM> *get_state(STATE_ENUM id)
        {
            return states[id];
        }

        State<STATE_ENUM> *get_current_state() const
        {
            return current_state;
        }
    };
}

using AppStateManagerType = bden::fsm::StateManager<bden::fsm::states::APP_STATES>;
using AppStateType = bden::fsm::State<bden::fsm::states::APP_STATES>;