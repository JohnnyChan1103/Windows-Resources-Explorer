#ifndef EXPL_DEF
#define EXPL_DEF

#include <iostream>
#include "ExplFunc.h"
using std::string;
//////////Display Information//////////
void DispSta(short idx);	//Display status
void DispPath(const string &);
void DispErr();
void DispMenu();
void DispHelp();
void Disp(const string &);	//Display strings
void Disp(const int &num);	//Display integer(int)
///////////////////////////////////////

//////////Basic Operations//////////
void DEL_LINE(const int &);
void Search();	//Search items in this folder
void Disp();		//Display items in this folder
void ShortPath(char *);
void Enter();	//Open a folder
void Back();		//Go back to parent folder
void Goto();
void Browse();
////////////////////////////////////

//////////File Operations//////////
void _Copy();
void _Cut();
bool _Del();
bool _Paste();
///////////////////////////////////
#endif
