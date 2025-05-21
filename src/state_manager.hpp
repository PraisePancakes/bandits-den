#pragma once
#include <iostream>
#include <list>
#include <unordered_map>

namespace bden::fsm
{
    namespace states
    {
        enum APP_STATES
        {
            STATE_NULL = -1,
            STATE_MENU,
            STATE_GAME,
            STATE_PAUSE,
            STATE_QUIT

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
        using type = T;
        State(StateManager<T> *ctx) : context(ctx) {};
        virtual bool on_init() = 0;
        virtual void on_update(float) = 0;
        virtual void on_render() = 0;
        virtual void on_exit() = 0;
        bool is_active = false;
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
            if (current_state)
            {
                current_state->is_active = false;
                current_state->on_exit();
            }

            current_state = states[id];
            current_state->is_active = true;
            current_state->on_init();
        };

        void update_current_state(float dt)
        {
            current_state->on_update(dt);
        }

        void render_current_state()
        {
            current_state->on_render();
        }

        states_type get_current_state_type() const
        {
            for (auto &s : states)
            {
                if (s.second == current_state)
                {
                    return s.first;
                }
            }
            return states::APP_STATES::STATE_NULL;
        }

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