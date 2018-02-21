#pragma once

#include <vector>
#include <map>
#include <functional>

namespace sarah_matrix
{
        
    class event_notifier
    {
    public:
        enum Type
        {
            INITIALISE,
            START,
            HOTWORD_DETECTED,
            RECORD_START,
            RECORD_END,
            SPEAK_RECEIVED,
            SPEAK_START,
            SPEAK_END,
            STOP,
            DEINITIALISE
        };

    public:
        event_notifier();
        ~event_notifier();

        void function_register(Type, std::function<void(void*)>);

        void notify(Type, void* param = 0);

    private:
        std::map<Type, std::vector< std::function<void(void*)> > >	_map;
    };

}
