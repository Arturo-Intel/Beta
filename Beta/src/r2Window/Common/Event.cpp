#include "Event.h"
namespace r2 {

	Event::Event(EventType type, Window* window) 
		: type(type), window(window) {}
	Event::~Event()
	{
	}
}