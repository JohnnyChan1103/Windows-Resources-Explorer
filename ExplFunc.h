#ifndef EXPL_FUNC
#define EXPL_FUNC

#include <conio.h>
#include <io.h>
#include <vector>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>
#include "ExplFuncDef.h"

using std::string;
using std::vector;
using std::sort;

typedef _finddata_t _FILE;
typedef _finddatai64_t _FILE64;
typedef string STR;
#define Sort sort(iteminfo.begin(), iteminfo.end(), itemOrder)	//for outputing items from A to Z, from folders to files
#define emptyFolder (folderCnt==0&&fileCnt==0)
inline void CLR(){system("cls");}

struct _INFO{
	STR name;
	unsigned attrib;
};

void DEL_LINE(const int &len){for (int i= 0; i<len; i++)	printf("\b");
							  for (int i= 0; i<len; i++)	printf(" ");
							  for (int i= 0; i<len; i++)	printf("\b");}

bool itemOrder(const _INFO &a, const _INFO &b){
	if ((a.attrib & b.attrib & _A_SUBDIR) || (a.attrib & b.attrib))		//Both are files or folders
		return strcmp(a.name.c_str(), b.name.c_str())<=0;
	else
		return a.attrib & _A_SUBDIR;
}

STR Path= "C:\\";	//C:\\Users\\Administrator\\Desktop
STR Tmp= "\0", TmpSuffix= "\0";

int TmpSta= 0;

int level;
inline void LEVEL_TEST(){printf("[TEST] LEVEL: "), printf("%d", level), printf("\n");}
//bool sub_path= false;
					 
char tempPath[200];
char *format= "*";
const STR status[]={
	"Browsing",		//0
	"Downloading",	//1
	"Uploading",	//2
	"Waiting",		//3
};
enum Status{Brs, Dwld, Upld, Wait, Help, Cpy, Cut}sta= Brs;
const STR option[]={	//visit		http://www.th7.cn/Program/cp/2011/12/07/49528.shtml		for file operations
	"Open/Excute",	//0	|======================|
	"Copy",			//1	|       Unusable       |
	"Cut",			//2	|          in          |
	"Delete",		//3 |        Empty         |
	"Rename",		//4	|        Folder        |
	"Paste",		//5	|======================|
	"Download",		//6	|Usable in Empty Folder|
	"Upload",		//7	|======================|
};
short _opt= 0;
const short Optnum= 6;

vector<_INFO> iteminfo;
vector<_INFO>::iterator _idx, _cursor;

int folderCnt= 0, fileCnt= 0, itemCnt= 0;
char key;

extern COORD CursorPos;	//To display the cursor for user
extern HANDLE hOut;
extern CONSOLE_CURSOR_INFO CursorInfo;
const short CursorSt= 3;
short CursorEnd= CursorSt+itemCnt;


//////////Others//////////
void findOpr(short idx);
bool findlevel(const char * Path);
//////////////////////////

//////////Display Information//////////
void DispSta(short idx)	//Display status
{
	printf("[Info]  Status: %s\n\n", status[idx].c_str());
}
void DispPath(const STR & file_name= "\0", const bool &isFolder= 1)
{
	if (Path[Path.length()-1]=='*')
		Path[Path.length()-1]= '\0';
	printf("[Path]  ");
	std::cout<<Path+file_name;
	if (!isFolder)
		printf("\b ");
	printf("\n");
}
void DispErr()
{
	CLR();
	printf("Invalid operation.\n");
}
void DispMenu()
{
	if (sta!=Brs)
		return;
	
	CLR();
	DispSta(sta= Wait);
	
	STR tmp= "\0";
	if (!emptyFolder)
		tmp= _cursor->name + "\\";			//To display current object
	level++;
	DispPath(tmp, 0);
	_opt= 0;
	
	for (short i= 0; i<Optnum; i++){
		printf("\t[Option]  %s", option[i].c_str());
		if (emptyFolder && i<5)
			printf("\t(Unusable)");
		else if(i==5)
			if (Tmp!="\0")
				printf("\t(to "), std::cout<<Path, printf(")");
			else
				printf("\t(Unusable)");
		printf("\n");
	}
	
	/*LEVEL_TEST();*/
	CursorPos.X= 0;
	CursorPos.Y= CursorSt;
	SetConsoleCursorPosition(hOut, CursorPos);
	printf("->");
}
void DispHelp()
{
	CLR();
	DispSta(sta= Help);
	level++;
	DispPath();
	printf("[Note]  Press M to display menu.\n\
[Note]  Press G to input and enter a path.\n\
[Note]  Press arrow key up or down to move the cursor.\n\
[Note]  Press arrow key left to go back.\n\
[Note]  Press arrow key right to open a folder.\n");
}
void Disp(const string &str)	//Display sentence
{
	std::cout<<str;
}
void Disp(const int &num)	//Display integer(int)
{
	printf("%d", num);
}
///////////////////////////////////////


//////////Basic Operations//////////
void Search()	//Search items in this folder
{
	itemCnt= folderCnt= fileCnt= 0;
	
	if (!iteminfo.empty())
		iteminfo.clear();
	long handle;
	
	_FILE tmp;
	//Core of the whole function
	strcpy(tempPath, Path.c_str());
	strcat(tempPath, format);
	Disp(tempPath);
	if ((handle= _findfirst(tempPath, &tmp)) != -1){
		do{
			if (strcmp(tmp.name, ".")==0 || strcmp(tmp.name, "..")==0)
				continue;
			iteminfo.push_back((_INFO){tmp.name, tmp.attrib});
			if (tmp.attrib & _A_SUBDIR)
				folderCnt++;
			else
				fileCnt++;
		}while (_findnext(handle, &tmp) == 0);
	_findclose(handle);
	_cursor= iteminfo.begin();
	}
	
	Sort;
	itemCnt= fileCnt+folderCnt;
	CursorEnd= CursorSt+itemCnt;
}

void Disp()		//Display items of this folder
{
	CLR();
	DispSta(sta);
	DispPath();
	
	for (_idx= iteminfo.begin(); _idx<iteminfo.end(); _idx++){
		Disp("\t[");
		if (_idx->attrib & _A_HIDDEN)
			Disp("Hidden ");
		if (_idx->attrib & _A_SUBDIR)
			Disp("Folder]  "), Disp(_idx->name.c_str()), Disp("\n");
		else
			Disp("File]  "), Disp(_idx->name.c_str()), Disp("\n");
	}
	Disp("[End]\n\n");
	
	if (emptyFolder){
		Disp("[Info]  No item\n\n");
		return;}
	else if (folderCnt == 0)
		Disp("[Info]  No folder, "), Disp(fileCnt), Disp(" files total.\n\n");
	else if (fileCnt == 0)
		Disp("[Info]  "), Disp(folderCnt), Disp(" folders total, no file.\n\n");
	else
		Disp("[Info]  "), Disp(folderCnt), Disp(" folders, "), Disp(fileCnt), Disp(" files total.\n\n");
		
	/*LEVEL_TEST();*/
	CursorPos.X= 0;
	CursorPos.Y= CursorSt;
	SetConsoleCursorPosition(hOut, CursorPos);
	printf("->");
}

void ShortPath(STR & Path)
{
	short idx= Path.length()-1;
	while (Path[--idx] != '\\');
	Path.erase(idx+1);
}

void Enter()	//Open a folder or menu
{
	if (sta == Brs)
		if ((_cursor->attrib & _A_SUBDIR) && !(emptyFolder)){
			Path= Path + _cursor->name + "\\";
			level++;
			Search();
			Disp();
		}
	else
		return;
}

void Back()		//Go back to parent folder
{
	if (level == 0)
		return;
	
	if (sta == Brs)
		ShortPath(Path);
	sta= Brs;
	level--;
	Search();
	Disp();
}

void Goto()
{
	CursorInfo.bVisible= 1;
	SetConsoleCursorInfo(hOut, &CursorInfo);
	
	string tmp;
	CLR();
	DispSta(sta= Brs);
	Disp("[Note]  Enter a path (Enter 0 to quit): ");
	//string TmpPath;
	std::cin>>tmp;
	getchar();
	
	CursorInfo.bVisible= 0;
	SetConsoleCursorInfo(hOut, &CursorInfo);
	
	if (tmp[0]=='0'){
		Disp();
		return;
	}
	if (tmp[tmp.length()-1]!='\\')
		tmp+="\\";
	if (findlevel(tmp.c_str())){
		tmp[0]= toupper(tmp[0]);
		Path= tmp;
	}
	else{
		CLR();
		DispErr();
		Sleep(1000);
	}
	
	Search();
	Disp();
}

void MoveCursor(bool Direction, bool isMenuCursor= 0)	//1 for Up, 0 for Down
{
	printf("\b\b  ");
	int _CursorEnd;
	if (isMenuCursor)
		_CursorEnd= Optnum+CursorSt;
	else
		_CursorEnd= CursorEnd;
	if (Direction==1)
		if (CursorPos.Y==CursorSt)
			CursorPos.Y= _CursorEnd-1;
		else
			CursorPos.Y--;
	
	else{
		CursorPos.Y++;
		if (CursorPos.Y==_CursorEnd)
			CursorPos.Y= CursorSt;
	}
	
	SetConsoleCursorPosition(hOut, CursorPos);
	printf("->");
}

void Browse()
{
	while((key=getch()) != 0x1B)		//ESC to quit
	{
		switch(key)
		{
			case -32:
				key= getch();
				switch(key)
				{
					case 72:										//Up
						if (sta == Help)
							continue;
						if (sta == Wait){	//Display options
							//DEL_LINE(option[_opt].length());
							if (_opt==0)		
								_opt= Optnum-1;
							else
								_opt--;
							//printf("%s", option[_opt].c_str());
							MoveCursor(1, 1);
						}else{				//Display items
							//DEL_LINE(_cursor->name.length());
							if ((!(emptyFolder)) && _cursor>iteminfo.begin())
								_cursor--;
							else if (_cursor == iteminfo.begin())
								_cursor= iteminfo.end()-1;
							MoveCursor(1);
							//printf("%s", _cursor->name.c_str());
						}
						break;
					case 80:										//Down
					if (sta == Help)
							continue;
					if (sta == Wait){		//Display options
						//DEL_LINE(option[_opt].length());
						if (_opt==Optnum-1)
							_opt= 0;
						else
							_opt++;
						//printf("%s", option[_opt].c_str());
						MoveCursor(0, 1);
						}else{				//Diplay items
							//DEL_LINE(_cursor->name.length());
							if ((!(emptyFolder)) && _cursor<iteminfo.end()-1)
								_cursor++;
							else if (_cursor == iteminfo.end()-1)
								_cursor= iteminfo.begin();
							//printf("%s", _cursor->name.c_str());
							MoveCursor(0);
						}
						break;
					case 75:										//Left
						Back();
						break;
					case 77:										//Right
						if (sta == Help)
							continue;
						if (sta == Wait)	//Press the enter key to excute option only.
							continue;
							//findOpr(_opt), sta= Brs, level--, Disp();
						else
							Enter();		//Enter folder 
						break;
					default:
						break;
				}
				break;
			case 13:	//Enter Key (related to Right)
				if (sta == Help)
					continue;
				if (sta == Brs)
					Enter();
				else if (sta == Wait)	//Excute option
					findOpr(_opt), sta= Brs, level--, Disp();
				break;
			case 8:		//BackSpace Key (related to Left)
				Back();
				break;
			case 'H':
			case 'h':
				DispHelp(); break;
			case 'G':
			case 'g':
				Goto(); break;
			case 'M':
			case 'm':
				if (sta==Brs) DispMenu(); break;
			/*TEST ONLY*///case 'L': case'l': printf("%d\n", level);
				
			default:
				break;
		}
	}
	CLR();
}
////////////////////////////////////

//////////File Operations//////////
void _Copy()
{
	TmpSta= Cpy;
	TmpSuffix= _cursor->name;
	Tmp= Path+TmpSuffix;
}

void _Cut()
{
	TmpSta= Cut;
	TmpSuffix= _cursor->name;
	Tmp= Path+TmpSuffix;
}

bool _Del()
{
	Tmp= Path+_cursor->name;
	CLR();
	Disp("Processing...\n");
	if (DeleteFile(Tmp.c_str())){
		CLR();
		Disp("Operation done.");
	}
	else{
		CLR();
		Disp("Operation failed.");
	}
	Sleep(1000);
}

bool _Paste()
{
	string status= "Copy";
	if (TmpSta==Cut)
		status= "Cut";
		
	if (Tmp=="\0")
		return 0;
	CLR();
	printf("[Info]  "), Disp(status), printf(" \""), Disp(Tmp);
	printf("\" to \""), Disp(Path), printf("\"");
	printf("\nAre you sure?[Y/N]\n");
	
	char ch;
	ch= getch();
	while (1){
		if (ch=='Y' || ch=='y'/* || ch==13Enter Key*/){
			STR tmp= Path+TmpSuffix;
			CLR();
			
			if (fopen(tmp.c_str(), "r")>0){		//Object file exists
				Disp("[Info]  There is a file has the same name.\nContinue?[Y/N]\n");
				ch= getch();
				while (1)
					if (ch=='Y' || ch=='y'){
						if (!DeleteFile(tmp.c_str())){	//?????
							CLR(), Disp("Operation failed."), Sleep(1000);
							return 0;
						}
						break;
					}
					else if (ch=='N' || ch=='n')
						return 0;
					else
						ch= getch();
			}
			
			CLR();
			Disp("Processing...\n");
			if (TmpSta==Cpy && CopyFile(Tmp.c_str(), tmp.c_str(), true)){
				CLR();
				Disp("Operation done.");
				Sleep(1000);
			}
			else if (TmpSta==Cut && MoveFile(Tmp.c_str(), tmp.c_str())){
				CLR();
				Disp("Operation done.");
				Sleep(1000);
			}
			else{
				CLR();
				Disp("Operation failed.");
				Sleep(1000);
				return 0;
			}
			break;
		}
		else if(ch=='N' || ch=='n')
			return 0;
		else
			ch= getch();
	}
	
	//Reset object path
	if (TmpSta==Cut)
		Tmp= "\0";
	
	CLR();
	return 1;
}
///////////////////////////////////

//////////Others//////////
bool findlevel(const char * Path)
{
	int ret= 1;
	if (access(Path, 0) != 0)
		return 0;
	if (fopen(Path, "r")>0)	//Object path is a file
		return 0;
	level= -1;
	for (short i= 0; i<strlen(Path); i++)
		if (Path[i]=='\\')	level++;
	if(level<0)	level= 0;
	return 1;
}

void findOpr(short idx)
{
	switch (idx){
		//case 0: if (!emptyFolder) _Run(); return;
		case 1: if (!emptyFolder) _Copy(); _cursor= iteminfo.begin(); return;
		case 2: if (!emptyFolder) _Cut(); _cursor= iteminfo.begin(); return;
		case 3: if (!emptyFolder) _Del(); Search(); return;
		//case 4: if (!emptyFolder) _Rnme(); return;
		case 5: _Paste(); Search(); return;
		//case 6: _Dwld(); return;
		//case 7: _Upld(); return;
	default: return;
	}
}
//////////////////////////
#endif
