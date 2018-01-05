/*
 *  immerse.cpp
 *
 *  Created on: 2017年12月10日
 *      Author: JamzumSum
 */
#include <windows.h>
#include <iostream>
using namespace std;

#ifndef   ABM_SETSTATE
#define   ABM_SETSTATE             0x0000000a
#endif

HWND tray = FindWindow("Shell_TrayWnd", NULL);
void AutoHideTaskBar(BOOL bHide)
{
	LPARAM lParam = bHide ? ABS_AUTOHIDE : ABS_ALWAYSONTOP;
	APPBARDATA apBar;
	memset(&apBar, 0, 36U);
	apBar.cbSize = 36U;
	apBar.hWnd = tray;
	if (apBar.hWnd)
	{
		apBar.lParam = lParam;
		SHAppBarMessage(ABM_SETSTATE, &apBar); //set auto hide
	}
}

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	switch(CEvent){
	case CTRL_C_EVENT:
		return false;
	case CTRL_BREAK_EVENT:
		return false;
	case CTRL_CLOSE_EVENT:
		AutoHideTaskBar(false);
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, false);
		break;
	case CTRL_LOGOFF_EVENT:
		return false;
	case CTRL_SHUTDOWN_EVENT:
		return false;
}
	return true;
}

int main(){
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == 0) {
		cout << "Hook failure." << endl;
		return 1;
	}
	cout << "Hola.Just enjoy the immersion taskbar. Close the console whenever you wanna close the application." << endl;
	BOOL flag = true;
	for (;;) {
		if (IsZoomed(GetForegroundWindow())) {
			if (flag) {
				AutoHideTaskBar(true);
				flag = false;
			}
		}
		else {
			if (!flag) {
				AutoHideTaskBar(false);
				flag = true;
			}
		}
		Sleep(500);
	}
	return 0;
}
