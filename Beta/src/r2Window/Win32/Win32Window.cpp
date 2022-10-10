#include "Win32Window.h"

enum Style : DWORD
{
    windowed = WS_OVERLAPPEDWINDOW,
    aero_borderless = WS_POPUP | WS_THICKFRAME,
    basicBorderless = WS_CAPTION | WS_OVERLAPPED | WS_THICKFRAME |
    WS_MINIMIZEBOX | WS_MAXIMIZEBOX
};

HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));

namespace r2 {
    Window::Window(){}

    Window::~Window()
    {
        if (hwnd != nullptr)
        {
            close();
        }
    }

    bool Window::create(EventQueue& eventQueue, const WindowDesc& desc)
    {
        mEventQueue = &eventQueue;
        const XWinState& xwinState = getXWinState();

        hinstance = xwinState.hInstance;
        HINSTANCE hPrevInstance = xwinState.hPrevInstance;
        LPSTR lpCmdLine = xwinState.lpCmdLine;
        int nCmdShow = xwinState.nCmdShow;

        mDesc = desc;

        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = Window::WindowProcStatic;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = WS_EX_NOPARENTNOTIFY;
        wndClass.hInstance = hinstance;
        wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = hBrush;
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = mDesc.name.c_str();
        wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

        if (!RegisterClassEx(&wndClass))
        {
            /**
             * Either an OS Error or a window with the same "name" id will cause
             * this to fail:
             */
            return false;
        }

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        DWORD dwExStyle = 0;
        DWORD dwStyle = 0;

        if (mDesc.fullscreen)
        {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            if (mDesc.frame)
            {
                dwStyle = Style::windowed;
            }
            else
            {
                dwStyle = Style::basicBorderless;
            }
        }

        RECT windowRect;
        windowRect.left = 0;
        windowRect.top = 0;
        windowRect.right = mDesc.fullscreen ? (long)screenWidth : (long)desc.width;
        windowRect.bottom = mDesc.fullscreen ? (long)screenHeight : (long)desc.height;

        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

        _windowBeingCreated = this;
        hwnd = CreateWindowEx(0, mDesc.name.c_str(), mDesc.title.c_str(), dwStyle,
            0, 0, windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top, NULL, NULL,
            hinstance, NULL);

        if (!hwnd)
        {
            // Failed to create window...
            return false;
        }

        if (mDesc.visible)
        {
            ShowWindow(hwnd, SW_SHOW);
            SetForegroundWindow(hwnd);
            SetFocus(hwnd);
        }
        return true;
    }

    LRESULT Window::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        Window* _this;
        if (_windowBeingCreated != nullptr)
        {
            _hwndMap.emplace(hwnd, _windowBeingCreated);
            _windowBeingCreated->hwnd = hwnd;
            _this = _windowBeingCreated;
            _windowBeingCreated = nullptr;
        }
        else
        {
            auto existing = _hwndMap.find(hwnd);
            _this = existing->second;
        }

        return _this->WindowProc(msg, wparam, lparam);
    }

    LRESULT Window::WindowProc(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        MSG message;
        message.hwnd = hwnd;
        message.lParam = lparam;
        message.wParam = wparam;
        message.message = msg;
        message.time = 0;

        LRESULT result = mEventQueue->pushEvent(message, this);
        if (result > 0) return result;
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    void Window::close()
    {
        if (hwnd != nullptr)
        {
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
    }
    
    void Window::executeEventCallback(const r2::Event e)
    {
        if (mCallback) mCallback(e);
    }
}