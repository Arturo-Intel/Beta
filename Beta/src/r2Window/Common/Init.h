#pragma once

#include <Windows.h>

#define MainArgs HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
#define MainArgsVars hInstance, hPrevInstance, lpCmdLine, nCmdShow


namespace r2
{
    struct XWinState
    {

        HINSTANCE hInstance;
        HINSTANCE hPrevInstance;
        LPSTR lpCmdLine;
        int nCmdShow;

        XWinState(MainArgs)
            : hInstance(hInstance), hPrevInstance(hPrevInstance),
            lpCmdLine(lpCmdLine), nCmdShow(nCmdShow)
        {
        }
        XWinState() {}
    };

	bool init(MainArgs);

	const XWinState& getXWinState();
}

