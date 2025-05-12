#pragma once
#include <iostream>
#include <list>

namespace bden::applicationlayer
{

    enum class ApplicationState
    {
        STATE_NULL = -1,
        STATE_GAME,
        STATE_MENU,
        STATE_PAUSE
    };

    struct application_subject;
    struct application_observer;

    struct application_observer
    {
        application_subject *context;

    public:
        virtual void on_update(float) = 0;
        virtual void on_render() = 0;
        virtual void update_app_listener(int, int) = 0;
        application_observer(application_subject *ctx) : context(ctx) {};
        virtual ~application_observer() {};
    };

    struct application_subject
    {
        virtual void attach(application_observer *) = 0;
        virtual void detach(application_observer *) = 0;
        virtual void set_state(ApplicationState state) = 0;
        virtual application_observer *get_state(ApplicationState state) = 0;
        virtual void notify() = 0;
        virtual ~application_subject() {};
    };
}