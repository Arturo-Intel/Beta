#include "r2Window/Win32/Win32Window.h"
#include <stdio.h>

void gameMain(int argc, const char** argv)
{
    r2::EventQueue eventQueue;
    r2::Window window;

    r2::WindowDesc windowDesc;
    windowDesc.name = "MainWindow";
    windowDesc.title = "Hello Window!";
    windowDesc.visible = true;
    windowDesc.width = 1280;
    windowDesc.height = 720;
    windowDesc.fullscreen = false;

    window.create(eventQueue, windowDesc);
    
    // Engine loop
    bool isRunning = true;
    while (isRunning)
    {
        bool shouldRender = true;

        // Update the event queue
        eventQueue.update();

        // Iterate through that queue:
        while (!eventQueue.empty())
        {
            const r2::Event& event = eventQueue.front();
            if (event.type == r2::EventType::Close)
            {
                window.close();
                shouldRender = false;
                isRunning = false;
            }

            eventQueue.pop();
        }

        if (shouldRender)
        {
            //renderer.render();
        }

    }
}