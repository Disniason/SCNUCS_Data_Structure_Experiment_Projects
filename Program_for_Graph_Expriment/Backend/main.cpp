#include "command.h"
#include <windows.h>
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    pge::cmd.execute();
    return 0;
}