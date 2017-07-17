
#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))



const int MAXROWS(24);
const int MAXCOLS(79);

// forward declarations --------------------------------------------------------------------------------------------

//shared functions


char* TrimWhitespaces(char *str);
bool isValidDate(char* s);
void debugMsg(char *string1,char *string2,int val);
void errMsg(WINDOW *win,int row,int col,char *string1);
std::string getInput( WINDOW *win, int startrow, int startcol, int width,int inputsize,char *prompt, chtype color);
std::string askYNQuestion( WINDOW *win, int startrow, int startcol, int width, char *prompt, chtype color);
void getNumericInput( WINDOW *msgwin, WINDOW *fullwin, int startrow, int startcol, int len,int width, char *prompt, char *input, chtype color);
bool  confirmAction( WINDOW *msgwin, WINDOW *fullwin, int startrow, int startcol, int width, std::string prompt, chtype color, int keyToPress);
void PrintInMiddle(WINDOW *win, int startrow, int startcol, int width, char *string, chtype color);
void FindMiddle(WINDOW *win, int startrow, int startcol, int &outrow, int &outcol,int width, char *msg);

// end of forward declarations --------------------------------------------------------------------------------------------
#endif // MAIN_HPP_INCLUDED
