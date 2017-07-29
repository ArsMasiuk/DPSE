#include "CPlatformServices.h"


#ifdef Q_OS_WIN32

#include <Windows.h>


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


#endif // windows

