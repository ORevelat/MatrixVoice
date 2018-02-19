
#include "main.h"

#include "event-notifier.h"

namespace sarah_matrix
{

	event_notifier::event_notifier()
	{
		LOG(INFO) << "event_notifier initialising ...";

		_map.clear();
		_map[INITIALISE] = std::vector< std::function<void()> >();
		_map[START] = std::vector< std::function<void()> >();
		_map[HOTWORD_DETECTED] = std::vector< std::function<void()> >();
		_map[RECORD_START] = std::vector< std::function<void()> >();
		_map[RECORD_END] = std::vector< std::function<void()> >();
		_map[SPEAK_START] = std::vector< std::function<void()> >();
		_map[SPEAK_END] = std::vector< std::function<void()> >();
		_map[STOP] = std::vector< std::function<void()> >();
		_map[DEINITIALISE] = std::vector< std::function<void()> >();
	}

	event_notifier::~event_notifier()
	{
		_map.clear();
	}

	void event_notifier::function_register(Type t, std::function<void()> func)
	{
		auto& currentlist = _map[t];
		currentlist.push_back(func);
	}
  
	void event_notifier::notify(Type t)
	{
		for (auto i : _map[t])
		{
			i();
		}
	}
  
}