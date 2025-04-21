#pragma once
#include <iostream>
#include <list>

namespace bden::applicationlayer
{

    struct application_observer
    {
        virtual void update_app_listener(int x, int y) = 0;

        virtual ~application_observer() {};
    };

    struct application_subject
    {
        virtual void attach(application_observer *) = 0;
        virtual void detach(application_observer *) = 0;
        virtual void notify() = 0;
        virtual ~application_subject() {};
    };
}