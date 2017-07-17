
#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <sqlite3.h>
#include <assert.h>
#include "dataDictionary.h"
#include "fields.h"
#include "screenFields.h"
#include "screens.h"
#include "tables.h"
#include "screenFields2.h"
#include "screenText.h"
#include "screenText2.h"

#include <string>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include <sqlite3.h>

// just found this bit of code to test if a field option is set or not
// if (field_opts(fields[i]) & O_ACTIVE)

struct termScreenFields{
    int idscreenFields;
    std::string ScreenLayoutId;
    std::string ScreenLocId;
    std::string tableId;
    std::string fieldId;
    std::string fieldName;
    std::string labelText;
    int labelRow;
    int labelCol;
    int fieldType;
    int fieldRow;
    int fieldCol;
    int fieldLen;
    int fieldPrec;
    std::string fieldEditable;
    std::string fieldRequired;
    std::string fieldVisible;
    int fieldTabOrder;
    std::string fieldEmumValues;

    } ;

const int MAXROWS(24);
const int MAXCOLS(79);
const std::string PRINTPATH="/home/john/";



const int MAX_SCREEN_FIELDS(50);
const int SF_KEY(1);
const int SF_SCREEN_LOC_ID(3);
const int SF_FIELD_ID(5);
const int SF_FIELD_NAME(7);
const int SF_LABEL_TEXT(9);
const int SF_LABEL_ROW(11);
const int SF_LABEL_COL(13);
const int SF_FIELD_TYPE(15);
const int SF_FIELD_ROW(17);
const int SF_FIELD_COL(19);
const int SF_FIELD_LEN(21);
const int SF_FIELD_PRE(23);
const int SF_FIELD_EDIT(25);
const int SF_FIELD_REQ(27);
const int SF_FIELD_VIS(29);
const int SF_FIELD_TAB(31);
const int SF_FIELD_ENUMS(33);


const int MAX_SCREEN_TEXTS(50);

const int STXT_KEY(1);
const int STXT_SCREEN_LOC_ID(3);
const int STXT_TEXT_TEXT(5);
const int STXT_TEXT_ROW(7);
const int STXT_TEXT_COL(9);

const int FE_FIELD_ID(2);
const int FE_FIELD_NAME(4);
const int FE_FIELD_LEN(6);
const int FE_FIELD_TYPE(8);
const int FE_FIELD_PRE(10);
const int FE_FIELD_PRIMKEY(12);
const int FE_FIELD_REQ(14);
const int FE_FIELD_ENUMS(16);

const int SD_SCREEN_ID(2);
const int SD_SCREEN_NAME(4);
const int SD_SCREEN_TABLE_ID(6);
const int SD_SCREEN_TABLE_NAME(8);

const int DD_FIELD_ID(4);
const int DD_TABLE_ID(2);

const int TE_TABLE_ID(2);
const int TE_TABLE_NAME(4);
const int TE_DATABASE(6);




// forward declarations --------------------------------------------------------------------------------------------
// int test(void);
void CleanupMenu(WINDOW *myMainMenuWin,MENU *myMainMenu, ITEM **myMenuItems, int NumChoices);

// tablesEntry declarations

int tablesEntry(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea);
void  tablesEntryFormCleanup(FIELD *tablesEntryFields[]);
void tablesEntrySetupForm(FIELD *tablesEntryFields[],WINDOW *winFullScreen);
int tablesEntryInsertTable(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea);
int tablesEntryUpdateTable(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea);
int tablesEntryDeleteTable(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea);
bool  tablesEntryVerify(FIELD *tablesEntryFields[],WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow);
int tablesEntryFieldsDisplayLookup(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow);
void SetTables(FIELD *tablesEntryFields[],std::vector<tables>::iterator tablesit,bool EditingRow);
int GetAllTables(std::vector<tables> &tablesResults, WINDOW *winSqlMsgArea);

// fieldsEntry declarations

int fieldsEntry(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea);
void  FieldsEntryFormCleanup(FIELD *fieldsEntryFields[]);
void fieldsEntrySetupForm(FIELD *fieldsEntryFields[],WINDOW *winFullScreen);
int fieldsEntryInsertField(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea);
int fieldsEntryUpdateField(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea);
int fieldsEntryDeleteField(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea);
bool  fieldsEntryVerify(FIELD *fieldsEntryFields[],WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow);
int fieldsEntryFieldsDisplayLookup(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow);
void fieldsEntrySetFields(FIELD *dataDictionaryEntryFields[],std::vector<fields>::iterator Fieldsit,bool EditingRow);

// dataDictionaryEntry declarations
int dataDictionaryEntry(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea);
void  dataDictionaryFieldsEntryFormCleanup(FIELD *dataDictionaryEntryFields[]);
void dataDictionaryEntrySetupForm(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,WINDOW *winFullScreen);
int InsertDictionaryFields(FIELD *dataDictionaryEntryFields[],WINDOW *winSqlMsgArea);
int DeleteDictionaryFields(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea);
int DeleteAllDictionaryFields(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea);
void DeleteThisDictionaryField(FIELD *dataDictionaryEntryFields[],
std::vector<std::string> &dataDictionaryEntryFieldsId,int formCurrentField,WINDOW *winSqlMsgArea);
void saveFieldInVector(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,int formCurrentField);
void FieldInsert(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,int formCurrentField,WINDOW *winSqlMsgArea);

bool  dataDictionaryEntryVerify(FIELD *dataDictionaryEntryFields[],
std::vector<std::string> &dataDictionaryEntryFieldsId,WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow);
int dataDictionaryEntryDisplayTablesLookup(FIELD *dataDictionaryEntryFields[],
std::vector<std::string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow);
int dataDictionaryEntryDisplayFieldsLookup(FIELD *dataDictionaryEntryFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow);
void SetdataDictionaryFieldsFromVector(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea);
void SetdataDictionaryFields(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId,std::string tableId,WINDOW *winSqlMsgArea);
void CleardataDictionaryFields(FIELD *dataDictionaryEntryFields[],std::vector<std::string> &dataDictionaryEntryFieldsId);


//screenField declarations

int screenField(WINDOW *winFullScreen,WINDOW *winSqlMsgArea,WINDOW *winMsgArea,std::string tableId,termScreenFields sf[], int &numsf,std::string ScreenLayoutId,bool screenYXlocked[][79], bool &deleted, bool &saved,bool NewField);
bool  screenFieldVerify(termScreenFields sf[],int numsf,FIELD *screenEntryFields[],WINDOW *winFullScreen,WINDOW *winFieldScreen, WINDOW *winMsgArea,WINDOW * winSqlMsgArea, std::string ScreenLayoutId,std::string tableId);
void screenFieldSetupForm(FIELD *screenEntryFields[],WINDOW *winFullScreen);
void  screenFieldFormCleanup(FIELD *screenEntryFields[]);
int displayFunctionKeys(WINDOW *winFieldScreen);
int screenFieldInsertField(termScreenFields sf[],int numsf,WINDOW *winSqlMsgArea);
int screenFieldDeleteField(termScreenFields sf[],int numsf,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen,WINDOW* winSqlMsgArea);
int screenFieldUpdateField(termScreenFields sf[],int numsf,bool screenYXlocked[][MAXCOLS],WINDOW* winSqlMsgArea);
int screenFieldDisplayLookup2(FIELD *screenEntryFields[],std::string tableId,WINDOW *winSqlMsgArea);
int screenFieldDisplayLookup(std::string ScreenLayoutId,termScreenFields sf[],int numsf,FIELD *screenEntryFields[],WINDOW *winSqlMsgArea);
void setScreenEntryFieldsFromDataDictionary(FIELD *screenEntryFields[],std::vector<dataDictionary>::iterator dataDictionaryit,bool EditingRow,WINDOW *winSqlMsgArea);
void setScreenEntryFieldsFromScreenFields(termScreenFields sf[],int numsf,FIELD *screenEntryFields[],std::vector<dbScreenFields>::iterator screenFieldsit,bool EditingRow);

//screenLayout declarations

int screenLayout(std::string ScreenLayoutId,std::string ScreenName,std::string tableId);
std::string scrapeLabel(WINDOW *winFullScreen, int curRow, int curCol,int &labelCol);
bool IsYXLocked (int row,int col,  bool screenYXlocked[][MAXCOLS],int &nextFreeColLeft,int &NextFreeColRight);
bool WhereIsYXLocked (int row,int col,bool screenYXlocked[][MAXCOLS],int &LockColLeft,int &LockColRight,WINDOW* winFullScreen);
int loadLayout(termScreenFields sf[],screenTexts stxt[],int &numsf, int &numstxt,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen,bool display);
int displayLayout(termScreenFields sf[],int numsf,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen);
int createLayout(termScreenFields sf[],int &numsf,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen);
int clearLayout(termScreenFields sf[],screenTexts stxt[],int &numsf, int &numstxt,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen,bool display);
int  removeLayout(std::string ScreenLayoutId);
int  generateCode (std::string ScreenLayoutId,std::string ScreenName,termScreenFields sf[],screenTexts stxt[],int numsf,int numstxt,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen);
bool replace(std::string& str, const std::string& from, const std::string& to);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
// screen text declarations
int screenText(WINDOW *winFullScreen,WINDOW *winSqlMsgArea,WINDOW *winMsgArea,screenTexts stxt[], int &numstxt,std::string ScreenLayoutId,bool screenYXlocked[][79], bool &deleted, bool &saved,bool NewField);
void screenTextSetupForm(FIELD *screenEntryTexts[],WINDOW *winFieldScreen);
void  screenTextFormCleanup(FIELD *screenEntryTexts[]);
int screenTextDeleteText(screenTexts stxt[],int numstxt,std::string ScreenLayoutId,bool screenYXlocked[][79],WINDOW* winFullScreen,WINDOW* winSqlMsgArea);
int screenTextUpdateField(screenTexts stxt[],int numstxt,bool screenYXlocked[][79],WINDOW* winSqlMsgArea);
int screenTextInsertField(screenTexts stxt[],int numstxt,WINDOW *winSqlMsgArea);
bool screenTextVerify(screenTexts stxt[],int numstxt,FIELD *screenEntryTexts[],WINDOW *winFullScreen,WINDOW *winFieldScreen, WINDOW *winMsgArea, WINDOW * winSqlMsgArea,std::string ScreenLayoutId);
int screenTextDisplayFunctionKeys(WINDOW *winFieldScreen);
int screenTextDisplayLayout(termScreenFields sf[],screenTexts stxt[],int &numsf, int &numstxt,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen);

// screensDefForm declarations
//********************************************************

int screensDef(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea);
void  screensDefFormCleanup(FIELD *screensDefFields[]);
void screensDefSetupForm(FIELD *screensDefFields[],WINDOW *winFullScreen);
int DeleteScreen(FIELD *screensDefFields[],WINDOW *winSqlMsgArea);
int InsertScreen(FIELD *screensDefFields[],WINDOW *winSqlMsgArea);
int UpdateScreen(FIELD *screensDefFields[],WINDOW *winSqlMsgArea);
bool  screensDefVerify(FIELD *screensDefFields[],WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow);
int screensDefDisplayLookup(FIELD *screensDefFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow);
int screensDefDisplayTableLookup(FIELD *screensDefFields[],WINDOW *winSqlMsgArea);
void SetscreensDef(FIELD *screensDefFields[],std::vector<dbscreensDef>::iterator screensDefit,bool EditingRow);
std::vector<dbscreensDef>  GetScreen(std::string lookupId,WINDOW *winSqlMsgArea);
int GetAllScreens(std::vector<dbscreensDef> &screensDefResults,WINDOW *winSqlMsgArea);

//********************************************************

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
bool execSQLAction(std::string sql, WINDOW *winSqlMsgArea);
std::vector<dataDictionary>  GetDictionaryFields(std::string lookupId,WINDOW *winSqlMsgArea);
std::vector<fields> GetField(std::string lookupId,WINDOW *winSqlMsgArea);
std::vector<tables>  GetTable(std::string lookupId,WINDOW *winSqlMsgArea);
int GetAllFields(std::vector<fields> &FieldsResults, WINDOW *winSqlMsgArea);
bool GetScreenEntryField(std::vector<dbScreenFields> &screenEntryFieldsResults,std::string ScreenLayoutId, std::string ScreenLocId,WINDOW *winSqlMsgArea);
int GetAllScreenEntryFields(std::vector<dbScreenFields> &screenEntryFieldsResults,std::string ScreenLayoutId, WINDOW *winSqlMsgArea);
bool GetScreenText(std::vector<dbScreenText> &screenTextsResults,std::string ScreenLayoutId, std::string ScreenLocId,WINDOW *winSqlMsgArea);
void zapWins(WINDOW *winFullScreen,WINDOW *winMsgArea,WINDOW *winSqlMsgArea);
int sqlErr(char errMsg, std::string sql,WINDOW *winSqlMsgArea);

// end of forward declarations --------------------------------------------------------------------------------------------
#endif // MAIN_HPP_INCLUDED
