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
            STARTED,
            HOTWORD_DETECTED,
            RECORD_START,
            RECORD_END,
            SPEAK_START,
            SPEAK_END,
            ENDED
        };

    public:
        event_notifier();

        void function_register(Type, std::function<void()>);

        void notify(Type);

    private:
        std::map<Type, std::vector< std::function<void()> > >	_map;
    };

}
