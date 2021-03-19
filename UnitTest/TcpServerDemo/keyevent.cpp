#include "keyevent.h"

void KeyEvent::run()
{
    while (true)
    {
        auto c = getchar();
        if (c == 'I')
        {
            emit initServer();
        }
        if (c == 'D')
        {
            emit disposeServer();
        }
        if (c == 'Q')
        {
            emit closeProgram();
            return;
        }
    }
}
