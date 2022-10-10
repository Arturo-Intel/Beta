#pragma once
#include <Windows.h>
#include <unordered_map>
#include "../Common/Init.h"
#include "../Win32/Win32EventQueue.h"
#include "../Common/WinDescriptor.h"

#include <functional>

namespace r2
{
	class Window
	{
	public:
		Window();
		~Window();

		bool create(EventQueue& eventQueue, const WindowDesc& desc);//const WindowDesc& desc, EventQueue& eventQueue);
		void close();

		friend class EventQueue;

	protected:

		void executeEventCallback(const r2::Event e);
		std::function<void(const r2::Event e)> mCallback;
		static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LRESULT WindowProc(UINT msg, WPARAM wparam, LPARAM lparam);

		WindowDesc mDesc;
		HWND hwnd = nullptr;
        HINSTANCE hinstance = nullptr;
        WNDCLASSEX wndClass = { 0 };
		RECT windowRect = { 0 };
        DEVMODE dmScreenSettings = { 0 };
		EventQueue* mEventQueue = nullptr;
        unsigned mBackgroundColor = 0xFFFFFFFF;
	};
	static thread_local Window* _windowBeingCreated = nullptr;
	static thread_local std::unordered_map<HWND, Window*> _hwndMap = {};
}

