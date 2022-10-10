#include <Windows.h>
#include "r2Window/Common/Init.h"
#include "r2Window/Main/Main.h"
#include <stdio.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    AllocConsole();
    FILE* pCout;
    FILE* pCerr;
    freopen_s(&pCout, "CONOUT$", "w+", stdout);
    freopen_s(&pCerr, "CONOUT$", "w+", stderr);
#endif

    r2::init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	gameMain(NULL, NULL);

#ifdef _DEBUG
    FreeConsole();
    fclose(pCout);
    fclose(pCerr);
    return 0;
#endif

}