/*
 *  immerse.cpp
 *
 *  Created on: 2017-12-10
 *      Author: JamzumSum
 */
#pragma once
#include "GUI.h"
#ifndef   ABM_SETSTATE
#define   ABM_SETSTATE             0x0000000a
#endif

form frmain("JamzumSum","Settings");
HWND tray = FindWindow("Shell_TrayWnd", NULL);
static UINT interval = 500;

void autoHide();

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
		SHAppBarMessage(ABM_SETSTATE, &apBar);
	}
}

bool isInt(LPCSTR str) {
	for (unsigned int i = 0; i < strlen(str);i++) {
		if (!isdigit(str[i])) return false;
	}
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	frmain.Event_On_Unload = [](form* me) {
		AutoHideTaskBar(false);
	};
	frmain.create();

	timer hide(&frmain, 500, 
		[](form* me)
		{
			autoHide();
		},
	true
	);
	Textbox intv(&frmain, 24, 32, 56, 24,"500", false);
	intv.Event_Text_Change = [](Textbox* me) {
		if (isInt(me->name)) {
			interval = atoi(me->name);
			((timer*)frmain.tab[0])->interval = interval;
		}
		else {
			static char r[8];
			_itoa_s(interval, r, 10);
			me->name = r;
		}
	};
	Label lblintv(&frmain, 24, 32, 112, 40, "窗体检测间隔：");
	
	intv.move(lblintv.x+ lblintv.w+8,lblintv.y-2);
	//创建
	intv();lblintv();frmain();
	return 0;
}

void autoHide(){
	static BOOL flag = true;
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
}