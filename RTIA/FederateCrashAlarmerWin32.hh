
#ifdef _WIN32

#include <windows.h>
#include "SocketUN.hh"

DWORD WINAPI FederateCrashAlarmerWin32Thread(LPVOID socketAsPtr)
{
    char data[16];
    DWORD readcount = 0;
    certi::SocketUN* controllerSocket = (certi::SocketUN*)socketAsPtr;
    
    // Since parent will never write anything to our stdin this will wait until
    // pipe acting as stdin gets closed in the other end. This happens when parent
    // exits or crashes.
    BOOL res = ReadFile(GetStdHandle(STD_INPUT_HANDLE), data, 16, &readcount, NULL);
    if (res == FALSE)
        delete controllerSocket;
    
    return 0;
}

SOCKET StartFederateCrashAlarmerWin32()
{
    certi::SocketUN* controllerSocket = new certi::SocketUN(certi::stIgnoreSignal);
    SOCKET fd = controllerSocket->socketpair();
    if (fd == -1)
        return -1;
    
    HANDLE threadHandle = CreateThread(NULL,
                                       0,
                                       FederateCrashAlarmerWin32Thread, (LPVOID) controllerSocket,
                                       0,
                                       NULL);
                                       
    return fd;
}

#endif
