#include "A4Stuff.h"
#include "SetupA4.h"
#include <OSUtils.h>
#include <QuickDraw.h>

#define kINITid 0

typedef pascal void (*DrawMenuBarProc)();
DrawMenuBarProc oldDrawMenuBar;

pascal void myDrawMenuBar() {
	long	oldA4;
	GrafPtr oldPort;
	GrafPort myPort;
	Rect notchRect;
	
	short middleX;
		
	oldA4 = SetCurrentA4();
	RememberA4();
	
	oldDrawMenuBar();
	
	GetPort(&oldPort);
	
	OpenPort(&myPort);
	
	// Where is the middle of the screen?
	middleX = myPort.portRect.right - myPort.portRect.left;
	middleX /= 2;
	middleX += myPort.portRect.left;
	
	PenNormal();
	SetRect(&notchRect, middleX-40, -10, middleX + 40, 18);
	PaintRoundRect(&notchRect, 15, 15);
	
	ClosePort(&myPort);
	
	SetPort(oldPort);
	
	SetA4(oldA4);
}

void main(void)
{
	long	oldA4;	
	Handle h;
	
	oldA4 = SetCurrentA4();
	RememberA4();
	
	h = Get1Resource('INIT', kINITid);
	if (h) {
		DetachResource(h);
	} else {
		DebugStr("\pargh");
	}
	
	oldDrawMenuBar = (DrawMenuBarProc)GetToolTrapAddress(_DrawMenuBar);
	SetToolTrapAddress((UniversalProcPtr)myDrawMenuBar, _DrawMenuBar);
	
	/* restore the a4 world */
	SetA4(oldA4);
}
