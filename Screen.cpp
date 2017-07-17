#define MYSQLPP_SSQLS_NO_STATICS
#ifndef MAIN_HPP_INCLUDED
#include "main.hpp"
#endif
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;



int screenLayout(string ScreenLayoutId,string ScreenName,string tableId,WINDOW *winMsgArea, WINDOW *winFullScreen)
{

    WINDOW *winSqlMsgArea; //*winFuncKeyArea, *winFullScreen, *winMsgArea,



// screenYXlocked array is used to track certain positions on the screen the user can't type in


    bool screenYXlocked[MAXROWS][MAXCOLS];



    int c=0,curRow=0,curCol=0,labelCol=0,fieldEndCol=0,numfs=0;
    int index=0,nextFreeColLeft=0,NextFreeColRight=0,colx=0,coly=0,tabStop=5;
    int numsf=0,savenumsf=0;// tracks the number of screen fields a developer created index for sf array
    bool quit=false,NewField=false,deleted=false,saved=false,doBox=false;
    char input[3];
    char NumToCharPointer[11];





    string temp,label,insert="Off";//,ScreenLayoutId="demo";
    std::vector<myScreenFields> screenEntryFieldsResults;
    std::vector<myScreenFields>::iterator screenEntryFieldsit;



     /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    wattron(stdscr,COLOR_PAIR(1));

     //screenFields sf[MAX_SCREEN_FIELDS]; // this caused seg fault error when trying to access a string member of the struct
     // this fixed seg fault error found syntax via google search
     screenFields *sf= new screenFields[MAX_SCREEN_FIELDS];


    winSqlMsgArea = newwin(24, 80, 0, 0);
    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);

    for (int x=0;x<MAXROWS;x++)
        for (int y=0;y<MAXCOLS;y++)
                        screenYXlocked [x] [y]=false;

    //clearLayout(sf,numfs,ScreenLayoutId,screenYXlocked,winFullScreen);



//    winFuncKeyArea = newwin(4, 78, 19,1);
//    assert(winFuncKeyArea != NULL);
//    wattron(winFuncKeyArea,COLOR_PAIR(1));
//    wrefresh(winFuncKeyArea);

// not working for some reason even if I use winfullscreen
//    wattron(winFuncKeyArea,A_REVERSE);
//    mvwprintw(winFuncKeyArea,2, 2, "F2 Lookup");
//    mvwprintw(winFuncKeyArea,2, 12, "F3 Delete");
//    mvwprintw(winFuncKeyArea,2, 22, "F4 Save");
//    mvwprintw(winFuncKeyArea,2, 32, "F5 New");
//    mvwprintw(winFuncKeyArea,2, 42, "F7 Exit");
//    wattroff(winFuncKeyArea,A_REVERSE);
//    wmove(winFuncKeyArea,1,1);
//    box(winFuncKeyArea,0,0);
//    wrefresh(winFuncKeyArea);



    box(stdscr,0,0);

    wrefresh(winFullScreen);
    refresh();



    curRow=1;
    curCol=1;

    keypad(stdscr,TRUE);
    mvwprintw(winFullScreen,23,1,"Row %02d Col %02d Layout: %s Insert %s",curRow,curCol,ScreenLayoutId.c_str(),insert.c_str());
    move(1,1);
    wrefresh(winFullScreen);
    while (quit!=true)
    {
        doBox=false;
        c=getch();
        //debugMsg("c=","",c);

        switch(c)
        {
            case 9: //tab

//                getNumericInput(winMsgArea,winFullScreen,2,2,2,78,"Enter Tab Stop",input,COLOR_PAIR(1));
//                tabStop=atoi(input);
//                touchwin(winFullScreen);
//                wrefresh(winFullScreen);
                    curCol+=tabStop;
                break;
            case KEY_BTAB:
                     curCol-=tabStop;
                break;


            case KEY_SDC: // shift delete character
            // must be bug in ncurses wdeleteln doesn't update screen correctly
                    //wdeleteln(winFullScreen);
                    //doBox=true;
                    break;
            case KEY_DC: // delete character

                //curCol--;
                //wdelch(winFullScreen);

                // must be bug in ncurses mvwdelch doesn't update screen correctly
                //mvwdelch(winFullScreen,curRow,curCol);
                //wrefresh(winFullScreen);
                break;

            case KEY_IC:
             // must be bug in ncurses insch doesn't update screen correctly
             // so no need to have this code
//                if (insert=="On ")
//                    insert="Off";
//                else
//                    insert="On ";
//                doBox=true;
                break;


            case KEY_BACKSPACE:

            if  (curCol>1 && IsYXLocked(curRow,curCol-1,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
            {
                curCol--;

                addch('\b');
                addch(' ');


            }
            else
            {
             //curCol=sf[numsf].fieldCol;

            }
            break;

            case KEY_DOWN:
            if (curRow<MAXROWS && IsYXLocked(curRow+1,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {

                    curRow++;


                }

                break;
            case 10:
            case 13:

                if (curRow<MAXROWS && IsYXLocked(curRow+1,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {
                    // must be bug in ncurses insertln doesn't update screen correctly
//                    if (insert=="On ")
//                    {
//                        insertln();
//                        doBox=true;
//                    }
                    curRow++;


                }

                break;
            case KEY_UP:

                if (curRow>=1 && IsYXLocked(curRow-1,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {
                    curRow--;

                }
                break;

            case KEY_LEFT:
                if  (curCol>1 && IsYXLocked(curRow,curCol-1,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {


                    waddch(winFullScreen,'\b');
                    --curCol;

                }
                else if(IsYXLocked(curRow,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==true)
                {

                    curCol=nextFreeColLeft;
                    //debugMsg("IsYXLocked=true","nextFreeColLeft",nextFreeColLeft);


                }
                else
                {
                    //debugMsg("IsYXLocked=false","curCol",curCol);

                    --curCol;
                    if (curCol==1)
                        curCol=2;
                }

                break;

            case KEY_RIGHT:

                //WhereIsYXLocked(curRow,curCol,screenYXlocked,coly,colx,winFullScreen);



                wrefresh(winFullScreen);
                if (IsYXLocked(curRow,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==true)
                {
                        curCol=NextFreeColRight;

                        break;
                }



                else if  (curCol<MAXCOLS && IsYXLocked(curRow,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {
                    curCol++;

                    if (curCol==MAXCOLS)
                        curCol=MAXCOLS-1;

                    if (curCol==1)
                        curCol=2;
                }


                break;

            case KEY_F(1):

                label=scrapeLabel(winFullScreen,curRow,curCol,labelCol);
                label=TrimWhitespaces(label.c_str());
                //curCol=tabStop;

                NewField=true;
                sprintf(NumToCharPointer, "Row%02dCol%02d", curRow,curCol);

                sf[numsf].ScreenId=NumToCharPointer;

                //debugMsg("before GetScreenEntryField","A",1);


                NewField=GetScreenEntryField(screenEntryFieldsResults,ScreenLayoutId,NumToCharPointer,winSqlMsgArea);
                //debugMsg("after GetScreenEntryField","B",1);

                if (NewField==false)
                {


                    screenEntryFieldsit=screenEntryFieldsResults.begin();
                    sf[numsf].FieldId=screenEntryFieldsit->FieldId;
                    sf[numsf].labelText=screenEntryFieldsit->labelText;
                    sf[numsf].labelRow=screenEntryFieldsit->labelRow;
                    sf[numsf].labelCol=screenEntryFieldsit->labelCol;
                    sf[numsf].fieldName=screenEntryFieldsit->fieldName;
                    sf[numsf].fieldType=screenEntryFieldsit->fieldType;
                    sf[numsf].fieldRow=screenEntryFieldsit->fieldRow;
                    sf[numsf].fieldCol=screenEntryFieldsit->fieldCol;
                    sf[numsf].fieldLen=screenEntryFieldsit->fieldLen;
                    sf[numsf].fieldPrec=screenEntryFieldsit->fieldPrec;
                    sf[numsf].fieldEditable=screenEntryFieldsit->fieldEditable;
                    sf[numsf].fieldRequired=screenEntryFieldsit->fieldRequired;
                    sf[numsf].fieldTabOrder=screenEntryFieldsit->fieldTabOrder;



                }
                else
                {
                    //numsf=index;
                    //debugMsg("NewField","true",1);

                    sf[numsf].ScreenId=NumToCharPointer;
                    sf[numsf].labelText=label;
                    sf[numsf].labelRow=curRow;
                    sf[numsf].labelCol=labelCol;
                    sf[numsf].fieldName="";
                    sf[numsf].fieldType=0;
                    sf[numsf].fieldRow=curRow;
                    sf[numsf].fieldCol=curCol;
                    sf[numsf].fieldLen=0;
                    sf[numsf].fieldPrec=0;
                    sf[numsf].fieldEditable="";
                    sf[numsf].fieldRequired="";
                    sf[numsf].fieldTabOrder=0;
                }


                savenumsf=numsf;
                deleted=false;
                saved=false;
                //debugMsg("before screenField","",1);

                screenField(winFullScreen,winSqlMsgArea,winMsgArea,tableId,sf,numsf,ScreenLayoutId,screenYXlocked,deleted,saved,NewField);

                winFullScreen = newwin(24, 80, 0, 0);




                if (deleted==false)
                {
                    mvwhline(winFullScreen,sf[numsf].fieldRow, sf[numsf].fieldCol,'_', sf[numsf].fieldLen);

                    wmove(winFullScreen,sf[numsf].fieldRow,sf[numsf].fieldCol-1);

                    fieldEndCol= sf[numsf].fieldCol+sf[numsf].fieldLen;

                    for (int x=sf[numsf].fieldCol;x<fieldEndCol;x++)
                        screenYXlocked [curRow] [x]=true;
                }

                if (saved==true)
                {
                    mvwaddstr(winFullScreen,sf[numsf].labelRow,sf[numsf].labelCol,sf[numsf].labelText.c_str());
                    numsf=savenumsf;
                    numsf++;

                }
                box(winFullScreen,0,0);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                refresh();
                for (int i=numsf; i< numsf;++i)
                {
                    sf[i].ScreenId="";
                    sf[i].FieldId="";
                    sf[i].fieldName="";
                    sf[i].labelText="";
                    sf[i].labelRow=0;
                    sf[i].labelCol=0;
                    sf[i].fieldType=0;
                    sf[i].fieldRow=0;
                    sf[i].fieldCol=0;
                    sf[i].fieldLen=0;
                    sf[i].fieldPrec=0;
                    sf[i].fieldEditable="";
                    sf[i].fieldRequired="";
                    sf[i].fieldTabOrder=0;
                }

                break;
            case KEY_F(2):

                ScreenLayoutId=getInput(winFullScreen,21,0,78,10,"Open layout",COLOR_PAIR(1));
                loadLayout(sf, numsf,ScreenLayoutId, screenYXlocked,winFullScreen);
                curRow=1,curCol=1;

                break;
            case KEY_F(3):

                ScreenLayoutId=getInput(winFullScreen,21,0,78,10,"Create layout",COLOR_PAIR(1));
                if (ScreenLayoutId>"")
                    createLayout(sf, numsf,ScreenLayoutId, screenYXlocked,winFullScreen);

                break;
            case KEY_F(5):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Clear Screen",COLOR_PAIR(1),KEY_F(5))==true)
            {
                   string answer="N";


                    answer=getInput(winFullScreen,21,0,78,1,"Remove field definitions too?",COLOR_PAIR(1));

                    if (answer=="y" || answer=="Y")
                        removeLayout(ScreenLayoutId);

                    clearLayout(sf,numsf,ScreenLayoutId,screenYXlocked,winFullScreen);

            }
                touchwin(winFullScreen);

                wrefresh(winFullScreen);
                break;



            case KEY_F(7): //exit
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                break;
            }


            case KEY_F(8): //generate
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Generate Code",COLOR_PAIR(1),KEY_F(8))==true)
            {
                generateCode(ScreenLayoutId,sf,numsf,screenYXlocked,winFullScreen);
            }
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;


            default:

                if  (curCol<MAXCOLS-1 && IsYXLocked(curRow,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {
                    // must be a bug in winsch
//                    if (insert=="On ")
//                        winsch(winFullScreen,c);
//                    else
                    mvwaddch(winFullScreen,curRow,curCol,c);

                    curCol++;
                }
                if (curCol>=MAXCOLS && IsYXLocked(curRow+1,curCol,screenYXlocked,nextFreeColLeft,NextFreeColRight)==false)
                {
                    curRow++;
                    curCol=2;
                }
                break;


        }


        mvwprintw(winFullScreen,23,1,"Row %02d Col %02d Layout: %s Insert %s",curRow,curCol,ScreenLayoutId.c_str(),insert.c_str());
        if ( (curRow!=0 && curRow!=MAXROWS) && (curCol!=0 && curCol!=MAXCOLS))
            move(curRow,curCol);
        if (curRow<1)
            curRow=1;
        if (curRow>MAXROWS)
            curRow=MAXROWS-1;
        if (curCol<1)
            curCol=1;
        if (curCol>=MAXCOLS)
            curCol=MAXCOLS-2;


        wrefresh(winFullScreen);
        //refresh();

    }

    // seg fault !
    //delete [] sf;
    delwin(winMsgArea);

    delwin(winFullScreen);

    delwin(winSqlMsgArea);

    endwin();


    delete[] sf;
    return 0;

}

string scrapeLabel(WINDOW *winFullScreen, int curRow, int curCol,int &labelCol)
{
    //debugMsg("scrapeLabel","labelCol",labelCol);
    string s;
    int z=0,stopAt=0,startAt=0;
    s="";
    labelCol=0;


    stopAt=curCol;
    startAt=stopAt-10;
    if (startAt<=0)
        startAt=1;
    debugMsg("startAt","",startAt);
    debugMsg("stopAt","",stopAt);





        for (int col=startAt;col<stopAt;col++)
        {

           //just get the text character (no attributes) that is an int
            z = mvwinch(winFullScreen,curRow,col) & A_CHARTEXT;
            if (z!='_')
                s.push_back(z);


        }
        int x = s.find_first_not_of(" ");

        labelCol = startAt+x;
        labelCol;

return s;


}

bool IsYXLocked (int row,int col,  bool screenYXlocked[][MAXCOLS],int &nextFreeColLeft,int &NextFreeColRight)
{
//debugMsg("inside IsYXLocked","NextFreeColRight",NextFreeColRight);

    if (screenYXlocked[row][col]==true)
    {
        for (int x=col;x<MAXCOLS;x++)
        {
            if (screenYXlocked[row][x]==false)
            {
                NextFreeColRight=x;

                break;
            }

        }


        for (int x=col;x>0;x--)
        {
            if (screenYXlocked[row][x]==false)
            {
                nextFreeColLeft=x;
                break;
            }

    }

    }

    //debugMsg("IsYXLocked","nextFreeColLeft",nextFreeColLeft);
    //debugMsg("IsYXLocked","NextFreeColRight",NextFreeColRight);


 return screenYXlocked[row][col];
}
bool WhereIsYXLocked (int row,int col,bool screenYXlocked[][MAXCOLS],int &LockColLeft,int &LockColRight,WINDOW* winFullScreen)
{

    LockColRight=0;
    LockColRight=0;



//    for (int x=col;x<MAXCOLS;x++)
//
//    {
//        if (screenYXlocked[row][x]==true)
//        {
//         mvwprintw(winFullScreen,23,20," screenYXlocked[%02d][%02d] = %i",row,x,screenYXlocked[row][x]);
//         wrefresh(winFullScreen);
//
//         getch();
//        }
//
//
//    }


    for (int x=col;x<MAXCOLS;x++)
    {
        if (screenYXlocked[row][x]==true)
        {
            LockColRight=x;
            break;
        }

    }




    for (int x=col;x>1;x--)
    {
        if (screenYXlocked[row][x]==false)
        {
            LockColLeft=x;
            break;
        }

    }
//    if (LockColRight!=0 || LockColLeft!=0)
//    {
//    mvwprintw(winFullScreen,23,40," Locked Left %02d Locked Right %02d",LockColRight,LockColLeft);
//    wrefresh(winFullScreen);
//    }

}
int loadLayout(screenFields sf[],int &numsf,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen)
{
    int numfs=0;


    clearLayout(sf,numsf,ScreenLayoutId,screenYXlocked,winFullScreen);

    WINDOW* winSqlMsgArea;
    winSqlMsgArea = newwin(24, 80, 0, 0);
    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);

    int result=0;
    string sql;
    mysqlpp::Connection con;

    std::vector<myScreenFields> myScreenFieldsResults;
    std::vector<myScreenFields>::iterator myScreenFieldsit;


    try
    {

        con.connect("devTool","localHost", "root", "moonpie");
        sql="SELECT idscreenFields,ScreenLayoutId,labelText,labelRow,labelCol,fieldName,fieldType,";
        sql.append("fieldRow,fieldCol,fieldLen,fieldPrec,fieldEditable,fieldRequired ");
        sql.append("FROM screenFields ");
        sql.append("WHERE ScreenLayoutId =");
        sql.append("\"");
        sql.append(ScreenLayoutId);
        sql.append("\"");
        sql.append(" ORDER BY idscreenFields");//fieldTabOrder");

        //debugMsg("sql",sql.c_str(),0);

        mysqlpp::Query query = con.query(sql);


        myScreenFieldsResults.clear();
        query.storein(myScreenFieldsResults);
        result=myScreenFieldsResults.size();
        //debugMsg("myScreenFieldsResults.size() ","",result);
    }


        catch (const mysqlpp::Exception& er)
        {
            wclear(winSqlMsgArea);
            box(winSqlMsgArea,0,0);
            PrintInMiddle(winSqlMsgArea,2,2,70,"SQL ERROR - Exception Error",COLOR_PAIR(1));
            mvwprintw(winSqlMsgArea, 2, 2, "%s",er.what());
            mvwprintw(winSqlMsgArea, 5, 2, "%s","______________________________________________________________________");
            mvwprintw(winSqlMsgArea, 6, 2, "%s",sql.c_str());
            mvwprintw(winSqlMsgArea, 21, 2, "%s","Press Enter");
            wrefresh(winSqlMsgArea);
            refresh();
            getch();
            delwin(winSqlMsgArea);
            result=false;
        }


    con.disconnect();

    numsf=0;

    myScreenFieldsit = myScreenFieldsResults.begin();
    for (myScreenFieldsit = myScreenFieldsResults.begin(); myScreenFieldsit != myScreenFieldsResults.end(); ++myScreenFieldsit)
    {


        if (numsf>MAX_SCREEN_FIELDS)
            break;
        else
       {

        //debugMsg("myScreenFieldsit->fieldName",myScreenFieldsit->fieldName.c_str(),numsf);
        sf[numsf].labelText=myScreenFieldsit->labelText;
        sf[numsf].labelRow=myScreenFieldsit->labelRow;
        sf[numsf].labelCol=myScreenFieldsit->labelCol;
        sf[numsf].fieldName=myScreenFieldsit->fieldName;
        sf[numsf].fieldType=myScreenFieldsit->fieldType;
        sf[numsf].fieldRow=myScreenFieldsit->fieldRow;
        sf[numsf].fieldCol=myScreenFieldsit->fieldCol;
        sf[numsf].fieldLen=myScreenFieldsit->fieldLen;
        sf[numsf].fieldPrec=myScreenFieldsit->fieldPrec;
        sf[numsf].fieldEditable=myScreenFieldsit->fieldEditable;
        sf[numsf].fieldRequired=myScreenFieldsit->fieldRequired;

        }
        numsf++;





    }
    displayLayout(sf,numsf,screenYXlocked,winFullScreen);


}
int displayLayout(screenFields sf[],int numsf,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen)
{
    for (int x=0;x<MAXROWS;x++)
        for (int y=0;y<MAXCOLS;y++)
                        screenYXlocked [x] [y]=false;
    string label="";

    int fieldEndCol=0;

    wmove(winFullScreen,0,0);
    wclrtobot(winFullScreen);
    for (int i=0;i<numsf;i++)
    {

        fieldEndCol= sf[i].fieldCol+sf[i].fieldLen;
        label=sf[i].labelText;
        //debugMsg("loop ",label.c_str(),numsf);

        mvwprintw(winFullScreen, sf[i].labelRow, sf[i].labelCol, "%s", sf[i].labelText.c_str());
        mvwhline(winFullScreen,sf[i].fieldRow, sf[i].fieldCol,'_', sf[i].fieldLen);


        for (int x=sf[i].fieldCol;x<fieldEndCol;x++)
            screenYXlocked [sf[i].fieldRow] [x]=true;
        wrefresh(winFullScreen);

    }

    wmove(winFullScreen,1,1);
    box(winFullScreen,0,0);
    touchwin(winFullScreen);
    wrefresh(winFullScreen);
    refresh();





    return 0;

}

int createLayout(screenFields sf[],int &numsf,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen)
{


// need to check if layout already exists
    WINDOW* winSqlMsgArea;
    winSqlMsgArea = newwin(24, 80, 0, 0);
    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);

    int result=0;
    string sql;
    mysqlpp::Connection con;


    std::vector<ScreenFields2> ScreenFields2Results;
    std::vector<ScreenFields2>::iterator ScreenFields2it;


    try
    {

        con.connect("devTool","localHost", "root", "moonpie");
        sql="SELECT count(ScreenLayoutId) as rows,ScreenLayoutId ";
        sql.append("FROM screenFields ");
        sql.append("WHERE ScreenLayoutId =");
        sql.append("\"");
        sql.append(ScreenLayoutId);
        sql.append("\"");


        //debugMsg("sql",sql.c_str(),0);

        mysqlpp::Query query = con.query(sql);


        ScreenFields2Results.clear();
        query.storein(ScreenFields2Results);
        result=ScreenFields2Results.size();
        //debugMsg("ScreenFields2Results.size() ","",result);
    }


        catch (const mysqlpp::Exception& er)
        {
            wclear(winSqlMsgArea);
            box(winSqlMsgArea,0,0);
            PrintInMiddle(winSqlMsgArea,2,2,70,"SQL ERROR - Exception Error",COLOR_PAIR(1));
            mvwprintw(winSqlMsgArea, 2, 2, "%s",er.what());
            mvwprintw(winSqlMsgArea, 5, 2, "%s","______________________________________________________________________");
            mvwprintw(winSqlMsgArea, 6, 2, "%s",sql.c_str());
            mvwprintw(winSqlMsgArea, 21, 2, "%s","Press Enter");
            wrefresh(winSqlMsgArea);
            refresh();
            getch();
            result=1;
        }


    con.disconnect();
    string answer="";

    if (result!=0)
        answer=askYNQuestion(winFullScreen,21,0,78,"Layout already exist overwrite?",COLOR_PAIR(1));

    if (answer=="Yes")
        removeLayout(ScreenLayoutId);



    clearLayout(sf, numsf,ScreenLayoutId, screenYXlocked,winFullScreen);
//    PrintInMiddle(winFullScreen,MAXROWS,0,78,title.c_str(),COLOR_PAIR(1));


    delwin(winSqlMsgArea);
    return 0;

}


int clearLayout(screenFields sf[],int &numsf,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen)

{


    for( int i=0;i<MAX_SCREEN_FIELDS;i++)
    {
        sf[i].fieldRow=0;
        sf[i].fieldCol=0;
        sf[i].labelText="";
        sf[i].labelCol=0;
        sf[i].labelRow=0;
        sf[i].fieldEditable="";
        sf[i].fieldLen=0;
        sf[i].fieldName="";
        sf[i].fieldPrec=0;
        sf[i].fieldRequired="";

    }

    //debugMsg("after for i","numsf",numsf);
    // initialize so all positions are available
    //debugMsg("before for row","numsf",numsf);
    for (int row=0;row<MAXROWS;row++)
    {
        for (int col=0;col<MAXCOLS;col++)
        {
            screenYXlocked[row][col]=false;


        }
    }


    // top & bottom rows have line from box so have to be locked
    for (int col=0;col<MAXCOLS;col++)
        {
            screenYXlocked[0][col]=true;
            screenYXlocked[MAXROWS-1][col]=true;

        }


    // lock box lines on the left & right side of the screen
    for (int row=0;row<MAXROWS;row++)
        {
            screenYXlocked[row][0]=true;
            screenYXlocked[row][1]=true;//debug
            screenYXlocked[row][MAXCOLS]=true;
        }



    wmove(winFullScreen,0,0);
    wclrtobot(winFullScreen);

    touchwin(winFullScreen);
    wrefresh(winFullScreen);
    box(winFullScreen,0,0);
    refresh();


    return 0;

}
int  removeLayout(string ScreenLayoutId)
{
    WINDOW* winSqlMsgArea;
    winSqlMsgArea = newwin(24, 80, 0, 0);
    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);

    int result=0;
    string sql;
    mysqlpp::Connection con;


    std::vector<ScreenFields2> ScreenFields2Results;
    std::vector<ScreenFields2>::iterator ScreenFields2it;


    try
    {

        con.connect("devTool","localHost", "root", "moonpie");
        mysqlpp::Query query = con.query();

        sql="DELETE ";
        sql.append("FROM screenFields ");
        sql.append("WHERE ScreenLayoutId =");
        sql.append("\"");
        sql.append(ScreenLayoutId);
        sql.append("\"");



        //debugMsg("sql",sql.c_str(),0);

        query << sql;
        query.execute();
    }


        catch (const mysqlpp::Exception& er)
        {
            wclear(winSqlMsgArea);
            box(winSqlMsgArea,0,0);
            PrintInMiddle(winSqlMsgArea,2,2,70,"SQL ERROR - Exception Error",COLOR_PAIR(1));
            mvwprintw(winSqlMsgArea, 2, 2, "%s",er.what());
            mvwprintw(winSqlMsgArea, 5, 2, "%s","______________________________________________________________________");
            mvwprintw(winSqlMsgArea, 6, 2, "%s",sql.c_str());
            mvwprintw(winSqlMsgArea, 21, 2, "%s","Press Enter");
            wrefresh(winSqlMsgArea);
            refresh();
            getch();
            result=1;
        }


    con.disconnect();



    delwin(winSqlMsgArea);
    return 0;

}


int  generateCode (std::string ScreenLayoutId,screenFields sf[],int numsf,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen)
{

    //debugMsg("generateCode","numsf",numsf);




    string printFile="/home/john/";
    printFile.append(ScreenLayoutId);
    printFile.append(".cpp");
     //ofstream out(PRINTPATH.c_str()+ScreenLayoutId+".cpp");
    std::remove(printFile.c_str());
    ofstream out(printFile.c_str());



    streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt


        cout << "void dataEntrySetupForm(FIELD *dataEntryFields[],WINDOW *winFullScreen)"<<endl<<endl;
        cout << "{"<<endl<<endl;
        cout << "init_pair(1,COLOR_GREEN,COLOR_BLACK);"<<endl;
        cout << "winFormArea = derwin(winFullScreen, 20, 78, 1, 1);"<<endl;
        cout << "assert(winFormArea != NULL);"<<endl;
        cout << "wattron(winFormArea,COLOR_PAIR(1));"<<endl;
        cout << " wrefresh(winFormArea);"<<endl;
        cout << "//              new_field(h,w,  tp,left, offscreen, nbuffers);"<<endl;




        int x=0;
        for (int i=0;i<numsf;i++)
        {

                cout << "dataEntryFields["<<x<<"] = new_field(1, "<<strlen(sf[i].labelText.c_str())<<","<<sf[i].labelRow<<","<<sf[i].labelCol<<",0,0);// Label "<<sf[i].labelText.c_str()<<endl;
                cout << "dataEntryFields["<<(x+1)<<"] = new_field(1, "<<sf[i].fieldLen<<","<<sf[i].fieldRow<<","<<sf[i].fieldCol<<",0,0);// field "<<sf[i].labelText.c_str()<<endl;
                x+=2;
        }

        cout << "dataEntryFields["<<(numsf*2)<<"] = NULL;"<<endl;

        cout << " for (int i=0;i<" <<(numsf*2)<<";i++)"<<endl;
        cout << " {"<< endl;

        for (int i=0;i<=(numsf*2);i++)
        {
            cout << "\t";
            cout << "assert(dataEntryFields[i] !=NULL);"<<endl;
         }
          cout << " }"<< endl;

        x=0;
        for (int i=0;i<numsf;i++)
        {

                cout << "set_field_buffer(dataEntryFields["<<x<<"] ,0,"<<'\"'<<sf[i].labelText.c_str()<<'\"'<<");"<<"//Label "<<sf[i].labelText.c_str()<<endl;
                cout << "set_field_buffer(dataEntryFields["<<(x+1)<<"] ,0,"<<'\"'<<'\"'<<");// field "<<sf[i].fieldName.c_str()<<endl;
                x+=2;
        }




        string type="";
        string nines(10,'9');
        for (int i=0;i<=numsf;i++)
        {
            //0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC
            //debugMsg("for ","",sf[i].fieldType);
            switch (sf[i].fieldType)
            {

            case 1:
                type="TYPE_ALPHA";

                cout << "set_field_type(dataEntryFields["<<i<<"] ,"<<type<<",0);"<<endl;
                break;
            case 2:
                type="TYPE_ALNUM";

                cout << "set_field_type(dataEntryFields["<<i<<"] ,"<<type<<",0);"<<endl;
                break;
            case 3:
            case 4:
                type="TYPE_NUMERIC";

                cout << "set_field_type(dataEntryFields["<<i<<"] ,"<<type<<",0,"<<nines.substr(1,sf[i].fieldLen)<<");"<<endl;
                break;
            default:

                break;



            }
        }

            cout << "for (int i=0;i<="<<(numsf*2)<<";i++)"<<endl;
            cout << "{"<<endl;
            //not sure if I want use use this as the way to identity a ready only field
            // or if I want to use the editable member of the sf struct
            cout <<"\t if (i % 2 !=0)"<<endl;  // typically odd field are labels and even are editable fields
            cout <<"\t\t set_field_back(dataEntryFields[i], A_UNDERLINE|COLOR_PAIR(1));"<<endl;
            cout <<"\t else"<<endl;
            cout <<"\t\t set_field_fore(dataEntryFields[i],COLOR_PAIR(1));"<<endl;
            cout << "}"<<endl;

        x=0;
        for (int i=0;i<=((numsf*2)-1);i++)
        {
            //debugMsg("for","i",i);
            if( x % 2 == 0 ) // label
            {

                cout << "set_field_opts(dataEntryFields["<<i<<"] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);"<<endl;
            }
            else
            {
                if (sf[i].fieldType==3 ||sf[i].fieldType==4)
                    cout << "set_field_opts(dataEntryFields["<<x<<"] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);"<<endl;
                else
                    cout << "set_field_opts(dataEntryFields["<<x<<"] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);"<<endl;

            }
            x++;
        }

        cout << "dataEntryForm = new_form(dataEntryFields);"<<endl;
        cout << "assert(dataEntryForm != NULL);"<<endl;
        cout << "set_form_win(dataEntryForm,winFullScreen);"<<endl;
        cout << "set_form_sub(dataEntryForm,winFormArea);"<<endl;
        cout <<"move("<<sf[0].fieldRow+1<<","<<sf[0].fieldCol+1<<");"<<endl;
        cout << "}"<<endl;



     //cout.precision(2);
     //cout.setf(ios::fixed);



    //cout.rdbuf(coutbuf); //reset to standard output again

}
