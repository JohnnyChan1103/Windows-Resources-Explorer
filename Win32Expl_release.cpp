//TODO: folder's copying/deleting/moving...
//http://blog.csdn.net/xiexievv/article/details/7475848
#include "ExplFuncDef.h"

extern HANDLE hOut;
extern CONSOLE_CURSOR_INFO cursorInfo;
int main()
{
	SetConsoleTitle("Resources Explorer v1.1");
	hOut= GetStdHandle(STD_OUTPUT_HANDLE);
	
	const short Width= 80, Height= 30;
	COORD screenSize= {Width+400, Height+1000};
	SetConsoleScreenBufferSize(hOut, screenSize);
	SMALL_RECT rc = {0, 0, Width-1, Height-1};
	SetConsoleWindowInfo(hOut,true ,&rc);
	
	//Make the cursor invisible (learned from http://blog.csdn.net/xiexievv/article/details/7475848)
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible=false;
	SetConsoleCursorInfo(hOut, &cursorInfo);
	
	Search();
	Disp();
	Browse();
	
	CloseHandle(hOut);
}
