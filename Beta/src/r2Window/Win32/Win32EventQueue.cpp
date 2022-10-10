#include "Win32EventQueue.h"
#include "Win32Window.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT)0x02)
#endif

namespace
{
    RAWINPUTDEVICE rawInputDevice[1];
}

namespace r2 {
    EventQueue::EventQueue() { initialized = false; }

    void EventQueue::update()
    {
        MSG msg = {};

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT EventQueue::pushEvent(MSG msg, Window* window)
    {
        UINT message = msg.message;
        LRESULT result = 0;

        RECT currentWindowRect = { -1, -1, -1, -1 };
        // TODO: hwnd to xwin::Window unordered_map, when xwin::Window closes, it
        // sends a message to the event queue to remove that hwnd
        // and any remaining events that match that xwin::Window

        if (!initialized)
        {
            initialized = true;
            rawInputDevice[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
            rawInputDevice[0].usUsage = HID_USAGE_GENERIC_MOUSE;
            rawInputDevice[0].dwFlags = RIDEV_INPUTSINK;
            rawInputDevice[0].hwndTarget = window->hwnd;
            RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0]));
        }

        r2::Event e = r2::Event(r2::EventType::None, window);

        switch (message)
        {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            e = r2::Event(r2::EventType::Close, window);
            break;
        }
        default:
            // Do nothing
            break;
        }
        
        if (e.type != EventType::None)
        {
            mQueue.emplace(e);
            window->executeEventCallback(e);
        }
        return result;
    }

    const Event& EventQueue::front() { return mQueue.front(); }

    void EventQueue::pop() { mQueue.pop(); }

    bool EventQueue::empty() { return mQueue.empty(); }
    size_t EventQueue::size() { return mQueue.size(); }
}