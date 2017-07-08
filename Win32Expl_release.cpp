//TODO: folder's copying/deleting/moving...
//http://blog.csdn.net/xiexievv/article/details/7475848
#include "ExplFuncDef.h"

COORD CursorPos;
HANDLE hOut= GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO CursorInfo;
int main()
{
	//Make the cursor invisible (learned from http://blog.csdn.net/xiexievv/article/details/7475848)
	GetConsoleCursorInfo(hOut, &CursorInfo);
	CursorInfo.bVisible=false;
	SetConsoleCursorInfo(hOut, &CursorInfo);
	
	Search();
	Disp();
	Browse();
}
