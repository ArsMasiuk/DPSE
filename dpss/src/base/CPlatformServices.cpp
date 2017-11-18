#include "CPlatformServices.h"


#ifdef Q_OS_WIN32

#include <Windows.h>
#include <Psapi.h>


bool CPlatformServices::SetActiveWindow(uint id)
{
	HWND hWnd = (HWND)id;
	if (!hWnd)
		return false;

	BringWindowToTop(hWnd);

	HWND   hCurrWnd = GetForegroundWindow();
	DWORD  myThreadID = GetCurrentThreadId();
	DWORD  currThreadID = GetWindowThreadProcessId(hCurrWnd, NULL);
	AttachThreadInput(myThreadID, currThreadID, TRUE);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	AttachThreadInput(myThreadID, currThreadID, FALSE);

	if (IsIconic(hWnd))
		ShowWindow(hWnd, SW_RESTORE);
	else
		ShowWindow(hWnd, SW_SHOW);

	return true;
}


CPlatformServices::PIDs CPlatformServices::GetRunningPIDs()
{
	PIDs result;

	DWORD pids[10240];
	DWORD bytesReturned;
	if (EnumProcesses(pids, 10240 * sizeof(DWORD), &bytesReturned))
	{
		int count = bytesReturned / sizeof(DWORD);
		for (int i = 0; i < count; ++i)
		{
			result << pids[i];
		}
	}

	return result;
}


#endif // windows


#ifdef Q_OS_LINUX

#include <QX11Info>


bool CPlatformServices::SetActiveWindow(uint id)
{
    return true;
}


CPlatformServices::PIDs CPlatformServices::GetRunningPIDs()
{
    PIDs result;

    return result;
}

#endif // linux
