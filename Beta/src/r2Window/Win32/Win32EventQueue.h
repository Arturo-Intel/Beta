#pragma once
#include <Windows.h>
#include "../Common/Event.h"
#include <queue>

namespace r2 {

	class Window;

	class EventQueue
	{
	public:
		EventQueue();
		void update();
		const Event& front();
		void pop();
		bool empty();
		size_t size();
		LRESULT pushEvent(MSG msg, Window* window);

	protected:
		bool initialized = false;
		std::queue<Event> mQueue;
	};
}

