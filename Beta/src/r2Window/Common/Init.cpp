#include "Init.h"

namespace r2
{
    namespace
    {
        XWinState xWinState;
    }

    bool init(MainArgs)
    {
        xWinState = XWinState(MainArgsVars);
        return true;
    }
    const r2::XWinState& getXWinState() { return xWinState; }
}