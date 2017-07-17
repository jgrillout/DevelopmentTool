
#ifndef MAIN_HPP_INCLUDED
#include "main.hpp"
#endif
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>



using namespace std;


int screenLayout(string ScreenLayoutId,string ScreenName,string tableId)
{

    WINDOW *winSqlMsgArea;
    WINDOW *winFullScreen;
    WINDOW *winMsgArea;//*winFuncKeyArea,



// screenYXlocked array is used to track certain positions on the screen the user can't type in


    bool screenYXlocked[MAXROWS][MAXCOLS];



    int c=0,curRow=0,curCol=0,labelCol=0,fieldEndCol=0,numfs=0;
    int index=0,nextFreeColLeft=0,NextFreeColRight=0,colx=0,coly=0,tabStop=5;
    int numsf=0,savenumsf=0;//Tracks the number of screen fields a developer created index for sf array
    int numstxt=0,savenumsft=0; //Tracks the number of screen texts a developer created index for stxt array
    bool quit=false,NewField=false,deleted=false,saved=false,doBox=false;
    char input[3];
    char NumToCharPointer[11];

    string temp,label,insert="Off";//,ScreenLayoutId="demo";
    std::vector<dbScreenFields> screenEntryFieldsResults;
    std::vector<dbScreenFields>::iterator screenEntryFieldsit;
    std::vector<dbScreenText> screenTextsResults;
    std::vector<dbScreenText>::iterator screenTextsit;



     /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    wattron(stdscr,COLOR_PAIR(1));

     //termScreenFields sf[MAX_SCREEN_FIELDS]; // this caused seg fault error when trying to access a string member of the struct
     // this fixed seg fault error found syntax via google search
     termScreenFields *sf= new termScreenFields[MAX_SCREEN_FIELDS];
     screenTexts *stxt= new screenTexts[MAX_SCREEN_TEXTS];


//    winSqlMsgArea = newwin(24, 80, 0, 0);
//    assert(winSqlMsgArea != NULL);
//    wattron(winSqlMsgArea,COLOR_PAIR(1));
//    wrefresh(winSqlMsgArea);
    winFullScreen = newwin(24, 80, 0, 0);

    assert(winFullScreen != NULL);
    wattron(winFullScreen,COLOR_PAIR(1));
    wrefresh(winFullScreen);

    winMsgArea = newwin(4, 78, 19, 1);

    assert(winMsgArea != NULL);
    wattron(winMsgArea,COLOR_PAIR(1));
    wrefresh(winMsgArea);


    winSqlMsgArea = newwin(24, 80, 0, 0);

    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);
    refresh();

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
    if (ScreenLayoutId>"")
        loadLayout(sf,stxt, numsf,numstxt,ScreenLayoutId, screenYXlocked,winFullScreen,true);



    curRow=1,curCol=1;

    keypad(stdscr,TRUE);
    mvwprintw(winFullScreen,23,1,"Row %02d Col %02d Layout: %s Insert %s",curRow,curCol,ScreenLayoutId.c_str(),insert.c_str());
    move(1,1);
    wrefresh(winFullScreen);
    while (quit!=true)
    {
        doBox=false;
        c=getch();


        switch(c)
        {
            case KEY_F(1):

                label=scrapeLabel(winFullScreen,curRow,curCol,labelCol);
                label=TrimWhitespaces(label.c_str());
                //curCol=tabStop;

                NewField=true;
                sprintf(NumToCharPointer, "Row%02dCol%02d", curRow,curCol);

                sf[numsf].ScreenLocId=NumToCharPointer;




                NewField=GetScreenEntryField(screenEntryFieldsResults,ScreenLayoutId,NumToCharPointer,winSqlMsgArea);

                if (NewField==false)
                {

                    screenEntryFieldsit=screenEntryFieldsResults.begin();
                    sf[numsf].ScreenLayoutId=screenEntryFieldsit->ScreenLayoutId;
                    sf[numsf].ScreenLocId=screenEntryFieldsit->ScreenLocId;
                    sf[numsf].tableId=screenEntryFieldsit->tableId;
                    sf[numsf].fieldId=screenEntryFieldsit->fieldId;
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
                    sf[numsf].fieldVisible=screenEntryFieldsit->fieldVisible;
                    sf[numsf].fieldTabOrder=screenEntryFieldsit->fieldTabOrder;
                    sf[numsf].fieldEmumValues=screenEntryFieldsit->fieldEmumValues;

                }
                else
                {

                    sf[numsf].ScreenLayoutId=ScreenLayoutId;
                    sf[numsf].ScreenLocId=NumToCharPointer;
                    sf[numsf].tableId=tableId;
                    sf[numsf].fieldId="";
                    sf[numsf].labelText=label;
                    sf[numsf].labelRow=curRow;
                    sf[numsf].labelCol=labelCol;
                    sf[numsf].fieldName="";
                    sf[numsf].fieldType=0;
                    sf[numsf].fieldRow=curRow;
                    sf[numsf].fieldCol=curCol;
                    sf[numsf].fieldLen=0;
                    sf[numsf].fieldPrec=0;
                    sf[numsf].fieldEditable="Yes";
                    sf[numsf].fieldRequired="No ";
                    sf[numsf].fieldVisible="Yes";
                    sf[numsf].fieldTabOrder=numsf;
                    sf[numsf].fieldEmumValues="";
                }


                savenumsf=numsf;
                deleted=false;
                saved=false;


                screenField(winFullScreen,winSqlMsgArea,winMsgArea,tableId,sf,numsf,ScreenLayoutId,screenYXlocked,deleted,saved,NewField);

                loadLayout(sf,stxt,numsf,numstxt,ScreenLayoutId,screenYXlocked,winFullScreen,true);


                box(winFullScreen,0,0);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                refresh();
                for (int i=numsf; i< numsf;++i) //// dont think i will ever be <numsf but so far I haven't noticed a problem with this
                {
                    sf[i].ScreenLocId="";
                    sf[i].fieldId="";
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
                    sf[i].fieldVisible="";
                    sf[i].fieldTabOrder=0;
                    sf[i].fieldEmumValues="";
                }

                break;
            case KEY_F(2):

                NewField=true;
                sprintf(NumToCharPointer, "Row%02dCol%02d", curRow,curCol);

                stxt[numstxt].ScreenLocId=NumToCharPointer;




                NewField=GetScreenText(screenTextsResults,ScreenLayoutId,NumToCharPointer,winSqlMsgArea);


                if (NewField==false)
                {

                    screenTextsit=screenTextsResults.begin();
                    stxt[numstxt].ScreenLayoutId=screenTextsit->ScreenLayoutId;
                    stxt[numstxt].ScreenLocId=screenTextsit->ScreenLocId;
                    stxt[numstxt].labelText=screenTextsit->labelText;
                    stxt[numstxt].labelRow=screenTextsit->labelRow;
                    stxt[numstxt].labelCol=screenTextsit->labelCol;




                }
                else
                {

                    stxt[numstxt].ScreenLayoutId=ScreenLayoutId;
                    stxt[numstxt].ScreenLocId=NumToCharPointer;
                    stxt[numstxt].labelText="";
                    stxt[numstxt].labelRow=curRow;
                    stxt[numstxt].labelCol=curCol;

                }


                savenumsft=numstxt;
                deleted=false;
                saved=false;


                screenText(winFullScreen,winSqlMsgArea,winMsgArea,stxt,numstxt,ScreenLayoutId,screenYXlocked,deleted,saved,NewField);



                loadLayout(sf,stxt,numsf,numstxt,ScreenLayoutId,screenYXlocked,winFullScreen,true);
                box(winFullScreen,0,0);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                refresh();
                for (int i=numstxt; i< numstxt;++i) // dont think i will ever be <numstxt but so far I haven't noticed a problem with this
                {
                    stxt[i].ScreenLocId="";
                    stxt[i].labelText="";
                    stxt[i].labelRow=0;
                    stxt[i].labelCol=0;

                }

                break;


            case KEY_F(3):
                break;

            case KEY_F(5):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Clear Screen",COLOR_PAIR(1),KEY_F(5))==true)
            {
                   string answer="N";


                    answer=getInput(winFullScreen,21,0,78,1,"Remove field definitions too?",COLOR_PAIR(1));

                    if (answer=="y" || answer=="Y")
                        removeLayout(ScreenLayoutId);

                    clearLayout(sf,stxt,numsf,numstxt,ScreenLayoutId,screenYXlocked,winFullScreen,true);

            }
                touchwin(winFullScreen);

                wrefresh(winFullScreen);
                break;



            case KEY_F(7): //exit
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;

            }
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;


            case KEY_F(8): //generate
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Generate Code",COLOR_PAIR(1),KEY_F(8))==true)
            {
                loadLayout(sf,stxt, numsf,numstxt,ScreenLayoutId, screenYXlocked,winFullScreen, false);
                generateCode(ScreenLayoutId, ScreenName,sf,stxt,numsf,numstxt,screenYXlocked,winFullScreen);

            }
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;
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
    string s,label;
    int z=0,stopAt=0,startAt=0;
    s="";
    labelCol=0;


    stopAt=curCol;
    startAt=curCol-1;
    if (startAt<=0)
        startAt=1;
    //debugMsg("startAt","",startAt);
    //debugMsg("stopAt","",stopAt);

    for (int col=startAt;col>0;col--)
        {

           //just get the text character (no attributes) that is an int
            z = mvwinch(winFullScreen,curRow,col) & A_CHARTEXT;
            if (z=='_' )
            {
                startAt=col+1;
                break;
            }
            else
                startAt=col;

           // else
           // if   (z!='_')
           //     s.push_back(z);


        }







        for (int col=startAt;col<stopAt;col++)
        {

           //just get the text character (no attributes) that is an int
            z = mvwinch(winFullScreen,curRow,col) & A_CHARTEXT;
            if (z=='_' )
                break;

            else
            if   (z!='_')
                s.push_back(z);


        }
        label=TrimWhitespaces(s.c_str());
        //debugMsg("label",label.c_str(),0);
        //debugMsg("label.length()","",label.length());
//        int x = s.find_first_not_of(" ");

        labelCol = curCol-(label.length()+1);
        if(labelCol>startAt && startAt<10)
            labelCol=startAt;
       // debugMsg("labelCol","",labelCol);


return label;


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
int loadLayout(termScreenFields sf[],screenTexts stxt[],int &numsf, int &numstxt, std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen,bool display)
{


     sqlite3 *db;
     sqlite3_stmt *stmt;
     int rc = 0;
     int col, cols;
     string sql;
     char* csql;
     numsf=0,numstxt=0;
     char msg[100];



    if (display)
        clearLayout( sf, stxt,numsf,numstxt,ScreenLayoutId,screenYXlocked,winFullScreen,display);


    WINDOW* winSqlMsgArea;
    winSqlMsgArea = newwin(24, 80, 0, 0);
    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);

    int result=0,rows=0;



    std::vector<dbScreenFields> myScreenFieldsResults;
    std::vector<dbScreenFields>::iterator myScreenFieldsit;
    std::vector<dbScreenText> myScreenTextResults;
    std::vector<dbScreenText>::iterator myScreenTextit;

    GetAllScreenEntryFields(myScreenFieldsResults,ScreenLayoutId, winSqlMsgArea);



    numsf=0;

    myScreenFieldsit = myScreenFieldsResults.begin();
    for (myScreenFieldsit = myScreenFieldsResults.begin(); myScreenFieldsit != myScreenFieldsResults.end(); ++myScreenFieldsit)
    {


        if (numsf>MAX_SCREEN_FIELDS)
            break;
        else
       {

        sf[numsf].ScreenLayoutId=myScreenFieldsit->ScreenLayoutId;
        sf[numsf].ScreenLocId=myScreenFieldsit->ScreenLocId;
        sf[numsf].tableId=myScreenFieldsit->tableId;
        sf[numsf].fieldId=myScreenFieldsit->fieldId;
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
        sf[numsf].fieldVisible=myScreenFieldsit->fieldVisible;
        sf[numsf].fieldTabOrder=myScreenFieldsit->fieldTabOrder;
        sf[numsf].fieldEmumValues=myScreenFieldsit->fieldEmumValues;



        }
        numsf++;
    }



        sql="SELECT idscreenText,ScreenLayoutId,ScreenLocId,labelText,labelRow,labelCol ";
        sql.append("FROM screenText ");
        sql.append("WHERE ScreenLayoutId =");
        sql.append("\"");
        sql.append(ScreenLayoutId);
        sql.append("\"");
        sql.append(" ORDER BY ScreenLocId");//fieldTabOrder");
        myScreenTextResults.clear();
        csql=sql.c_str();

       // open the database file

       rc = sqlite3_open("devTool.db", &db);

       if( rc ){
        sprintf(msg,"Can't open database: %s",sqlite3_errmsg(db));
        sqlErr(msg,sql,winSqlMsgArea);
         //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       }else{
         // prepare the SQL statement from the command line

         rc = sqlite3_prepare_v2(db, csql, -1, &stmt, 0);

         if( rc ){
            sprintf(msg,"SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
            sqlErr(msg,sql,winSqlMsgArea);
           //fprintf(stderr, "SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
         }else{
           cols = sqlite3_column_count(stmt);
           //debugMsg("sqlite3_column_count","",cols);
           // execute the statement
           //cout<<"inside dostep do"<<endl;
           do{

            dbScreenText texts;
             rc = sqlite3_step(stmt);
             switch( rc ){
               case SQLITE_DONE:
                 break;
               case SQLITE_ROW:
                 rows++;
                 // print results for this row
                 for( col=0; col<cols; col++){
                    //cout<<"inside dostep for col " <<col<<endl;
                   const char *txt = (const char*)sqlite3_column_text(stmt,col);
                   //printf("%s = %s\n", sqlite3_column_name(stmt, col), txt ? txt : "NULL");
                   //cout<<"inside dostep switch (col) " <<col<<endl;
                   switch (col)
                   {
                    case 0:
                        texts.idscreenText=atoi(txt);
                        break;
                    case 1:
                        texts.ScreenLayoutId=txt;
                        break;
                    case 2:
                        texts.ScreenLocId=txt;
                        break;
                    case 3:
                        texts.labelText=txt;
                        break;
                    case 4:
                        texts.labelRow=atoi(txt);
                        break;
                    case 5:
                        texts.labelCol=atoi(txt);
                        break;
                   }



                 }
                 myScreenTextResults.push_back(texts);
                 break;
               default:
                  sprintf(msg,"Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                  sqlErr(msg,sql,winSqlMsgArea);
                 //fprintf(stderr, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                 break;
             }
           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         }
         // close the database file
         sqlite3_close(db);
       }







    numstxt=0;
    //debugMsg("myScreenTextResults.size","",myScreenTextResults.size());
    myScreenTextit = myScreenTextResults.begin();
    for (myScreenTextit = myScreenTextResults.begin(); myScreenTextit != myScreenTextResults.end(); ++myScreenTextit)
    {


        if (numstxt>MAX_SCREEN_TEXTS)
            break;
        else
       {

        stxt[numstxt].ScreenLayoutId=myScreenTextit->ScreenLayoutId;
        stxt[numstxt].ScreenLocId=myScreenTextit->ScreenLocId;
        stxt[numstxt].labelText=myScreenTextit->labelText;
        stxt[numstxt].labelRow=myScreenTextit->labelRow;
        stxt[numstxt].labelCol=myScreenTextit->labelCol;
        }
        numstxt++;

    }

    if (display)
        screenTextDisplayLayout(sf,stxt,numsf,numstxt,screenYXlocked,winFullScreen);


}

int screenTextDisplayLayout(termScreenFields sf[],screenTexts stxt[],int &numsf, int &numstxt,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen)
{
    for (int x=0;x<MAXROWS;x++)
        for (int y=0;y<MAXCOLS;y++)
                        screenYXlocked [x] [y]=false;
    string label="";

    int fieldEndCol=0,len=0;

    wmove(winFullScreen,0,0);
    wclrtobot(winFullScreen);
    for (int i=0;i<numsf;i++)
    {

        fieldEndCol= sf[i].fieldCol+sf[i].fieldLen;
        label=sf[i].labelText;

        mvwprintw(winFullScreen, sf[i].labelRow, sf[i].labelCol, "%s", sf[i].labelText.c_str());
        mvwhline(winFullScreen,sf[i].fieldRow, sf[i].fieldCol,'_', sf[i].fieldLen);


        for (int x=sf[i].fieldCol;x<fieldEndCol;x++)
            screenYXlocked [sf[i].fieldRow] [x]=true;
        wrefresh(winFullScreen);

    }


    for (int i=0;i<numstxt;i++)
    {
        len=strlen(stxt[i].labelText.c_str());
        fieldEndCol= stxt[i].labelCol+len;
        //debugMsg("stxt[i].labelText.c_str()",stxt[i].labelText.c_str(),i);

        mvwprintw(winFullScreen, stxt[i].labelRow, stxt[i].labelCol, "%s", stxt[i].labelText.c_str());



        for (int x=stxt[i].labelCol+1;x<fieldEndCol;x++)
            screenYXlocked [stxt[i].labelRow] [x]=true;
        wrefresh(winFullScreen);

    }


    wmove(winFullScreen,1,1);
    box(winFullScreen,0,0);
    touchwin(winFullScreen);
    wrefresh(winFullScreen);
    refresh();
    return 0;

}




int clearLayout(termScreenFields sf[],screenTexts stxt[],int &numsf, int &numstxt,std::string ScreenLayoutId,bool screenYXlocked[][MAXCOLS],WINDOW* winFullScreen,bool display)

{


    for( int i=0;i<MAX_SCREEN_FIELDS;i++)
    {
        sf[i].fieldRow=0;
        sf[i].fieldCol=0;
        sf[i].labelText="";
        sf[i].labelCol=0;
        sf[i].labelRow=0;
        sf[i].fieldEditable="";
        sf[i].fieldRequired="";
        sf[i].fieldVisible="";
        sf[i].fieldLen=0;
        sf[i].fieldName="";
        sf[i].fieldPrec=0;
        sf[i].fieldVisible="";

    }
    for( int i=0;i<MAX_SCREEN_TEXTS;i++)
    {

        stxt[i].labelText="";
        stxt[i].labelCol=0;
        stxt[i].labelRow=0;


    }


    // initialize so all positions are available

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


    if (display)
    {
        wmove(winFullScreen,0,0);
        wclrtobot(winFullScreen);

        touchwin(winFullScreen);
        wrefresh(winFullScreen);
        box(winFullScreen,0,0);
        refresh();
    }


    return 0;

}
int  removeLayout(string ScreenLayoutId)
{
    WINDOW* winSqlMsgArea;
    winSqlMsgArea = newwin(24, 80, 0, 0);
    assert(winSqlMsgArea != NULL);
    wattron(winSqlMsgArea,COLOR_PAIR(1));
    wrefresh(winSqlMsgArea);

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc=0,result=0;
    string sql;
    char* csql;
    char msg[100];



    sql="DELETE ";
    sql.append("FROM screenFields ");
    sql.append("WHERE ScreenLayoutId =");
    sql.append("'");
    sql.append(ScreenLayoutId);
    sql.append("'");
    //debugMsg("sql",sql.c_str(),0);
    csql=sql.c_str();
    //const char* data = "";
    execSQLAction(csql,winSqlMsgArea);

    delwin(winSqlMsgArea);
    return 0;

}


int  generateCode (std::string ScreenLayoutId,std::string ScreenName,termScreenFields sf[],screenTexts stxt[],int numsf,int numstxt,bool screenYXlocked[][MAXCOLS],WINDOW *winFullScreen)
{




        string printFile="./generate/";

    string inFile = "./generate/template.cpp";
    printFile.append(ScreenName);
    printFile.append(".cpp");
    std::remove(printFile.c_str());
    ofstream out(printFile.c_str());
    string prefix = ScreenName+"Entry";
    string constants[numsf*2];
    string s;
    int x=0;
    string         value="";
    string         options="";
    std::stringstream   linestream;


    streambuf *coutbuf = std::cout.rdbuf(); //save old cout buf
    cout.rdbuf(out.rdbuf()); //redirect std::cout to printfile



    int j=0;

    for (int i=0;i<numsf;i++)
    {


        if (sf[i].fieldId>"")
        {

            constants[j]=ScreenLayoutId+"_"+sf[i].fieldId+"_LABEL";
            constants[j+1]=ScreenLayoutId+"_"+sf[i].fieldId+"_FIELD";
        }
        j+=2;
    }

    cout <<"#include \"""main.hpp\""<<endl;
    cout <<"#include <ncurses.h>"<<endl;
    cout <<"#include <form.h>"<<endl;
    cout <<"#include <menu.h>"<<endl;
    cout <<"#include <malloc.h>"<<endl;
    cout <<"#include <string.h>"<<endl;
    cout <<"#include <strings.h>"<<endl;
    cout <<"#include <stdlib.h>"<<endl;
    cout <<"#include <stdio.h>"<<endl;
    cout <<"#include <assert.h>"<<endl;
    cout <<endl;




    for (int i=0;i<((numsf*2));i++)
    {
        if (constants[i]>"")
            cout << "const int " <<constants[i]<<"(" <<i<<");"<<endl;

    }

    cout <<"static WINDOW *winFormArea;"<<endl;
    cout <<"static WINDOW *winFullScreen;"<<endl;
    cout <<"static WINDOW *winMsgArea;"<<endl;
    cout <<"static FORM *"<<prefix<<"Form;"<<endl;





    // forward declarations
    cout << "int "<<prefix<<"(WINDOW *winMsgArea,WINDOW *winFullScreen);"<<endl<<endl;
    cout << "void "<<prefix<<"SetupForm"<<"(FIELD *"<<prefix<<"Fields[],WINDOW *winFullScreen);"<<endl<<endl;
    cout << "void "<<prefix<<"FormCleanup"<<"(FIELD *"<<prefix<<"Fields[]);"<<endl<<endl;


    cout << "int  main(void)"<<endl;
    cout << "\{"<<endl;

    cout << "\t initscr();"<<endl;
    cout << "\t start_color();"<<endl;
    cout << "\t init_pair(1,COLOR_GREEN,COLOR_BLACK);"<<endl;
    cout << "\t cbreak();"<<endl;
    cout << "\t noecho();"<<endl;
    cout << "\t keypad(stdscr, TRUE);"<<endl;
    cout << "\t wattron(stdscr,COLOR_PAIR(1));"<<endl;
    cout << "\t winFullScreen = newwin(24, 80, 0, 0);"<<endl;
    cout << "\t assert(winFullScreen != NULL);"<<endl;

    cout << "\t wattron(winFullScreen,COLOR_PAIR(1));"<<endl;
    cout << "\t wrefresh(winFullScreen);"<<endl;

    cout << "\t winMsgArea = newwin(4, 78, 19, 1);"<<endl;
    cout << "\t assert(winMsgArea != NULL);"<<endl;
    cout << "\t wattron(winMsgArea,COLOR_PAIR(1));"<<endl;

    cout << "\t wrefresh(winMsgArea);"<<endl;
    cout << "\t refresh();"<<endl;

    cout << "\t "<<prefix<<"(winMsgArea,winFullScreen);"<<endl;
    cout << "}"<<endl;


    cout << "int "<<prefix<<"(WINDOW *winMsgArea,WINDOW *winFullScreen)"<<endl;
    cout <<"{"<<endl;

    cout <<"short formCurrentField;"<<endl;
    cout <<"int key=0, ch=0;"<<endl;
    cout << "static FIELD*tempFptr;"<<endl;
    cout << "bool firstTime=true;"<<endl;


    cout << "FIELD *"<<prefix<<"Fields["<<(numsf*2)+numstxt<<"];"<<endl;
     ifstream input_data;
    input_data.open(inFile.c_str());

    while( input_data.good() )
    {
        getline( input_data, s );
        replaceAll(s,"dataEntry",prefix);
        replaceAll(s,"%title%",ScreenName);
        cout << s<<endl;


    }

    cout << "void "<<prefix<<"SetupForm"<<"(FIELD *"<<prefix<<"Fields[],WINDOW *winFullScreen)"<<endl<<endl;
    cout << "{"<<endl<<endl;
    cout << "init_pair(1,COLOR_GREEN,COLOR_BLACK);"<<endl;
    cout << "winFormArea = derwin(winFullScreen, 20, 78, 1, 1);"<<endl;
    cout << "assert(winFormArea != NULL);"<<endl;
    cout << "wattron(winFormArea,COLOR_PAIR(1));"<<endl;
    cout << " wrefresh(winFormArea);"<<endl;
    cout << "//              new_field(h,w,  tp,left, offscreen, nbuffers);"<<endl;





        for (int i=0;i<numsf;i++)
        {
                cout <<prefix<<"Fields["<<constants[x]<<"] = new_field(1, "<<strlen(sf[i].labelText.c_str())<<","<<sf[i].labelRow<<","<<sf[i].labelCol<<",0,0);// Label "<<sf[i].labelText.c_str()<<endl;
                cout <<prefix<<"Fields["<<constants[x+1]<<"] = new_field(1, "<<sf[i].fieldLen<<","<<sf[i].fieldRow<<","<<sf[i].fieldCol<<",0,0);// field "<<sf[i].fieldId.c_str()<<endl;
                x+=2;
        }


         for (int i=0;i<numstxt;i++)

        {

                cout <<prefix<<"Fields["<<(numsf*2)+i<<"] = new_field(1, "<<strlen(stxt[i].labelText.c_str())<<","<<stxt[i].labelRow<<","<<stxt[i].labelCol<<",0,0);// Text "<<stxt[i].labelText.c_str()<<endl;

        }

        cout <<prefix<<"Fields["<<(numsf*2)+numstxt<<"] = NULL;"<<endl;

        cout << " for (int i=0;i<" <<(numsf*2)+numstxt<<";i++)"<<endl;
        cout << " {"<< endl;


        cout << "\t";
        cout << "assert(" << prefix<<"Fields[i] !=NULL);"<<endl;

        cout << " }"<< endl;

        x=0;



        for (int i=0;i<numsf;i++)
        {
            cout << "set_field_buffer("<<prefix<<"Fields["<<constants[x]<<"] ,0,"<<'\"'<<sf[i].labelText.c_str()<<'\"'<<");"<<"//Label "<<sf[i].labelText.c_str()<<endl;
            cout << "set_field_buffer("<<prefix<<"Fields["<<constants[x+1]<<"] ,0,"<<'\"'<<'\"'<<");// field "<<sf[i].fieldId.c_str()<<endl;
            x+=2;
        }

        for (int i=0;i<numstxt;i++)
        {


                cout << "set_field_buffer("<<prefix<<"Fields["<<(numsf*2)+i<<"] ,0,"<<'\"'<<stxt[i].labelText.c_str()<<'\"'<<");"<<"//Text "<<endl;
        }





        string type="",fieldOptsBack="",fieldOpts="",labelOpts="",def="",just="|JUSTIFY_LEFT";
        string nines(10,'9');
        char NumToCharPointer[10];

        x=0;
        for (int i=0;i<numsf;i++)
        {

            type="",just="|JUSTIFY_LEFT";
            //0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC,5=ENUM,6=YN,7=Date



            //opts=" A_UNDERLINE|O_PUBLIC | O_ACTIVE|O_STATIC|O_BLANK";
            fieldOpts=" A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK";
            fieldOptsBack="COLOR_PAIR(1)";
            labelOpts=" O_PUBLIC | O_AUTOSKIP";



            if (sf[i].fieldEditable=="Y" && sf[i].fieldVisible=="Y")
            {
                fieldOpts.append("|O_EDIT|O_ACTIVE");
            }


            // actually this doesn't matter, I thought O_NULLOK would force a field to have a value but it doesn't
            if (sf[i].fieldRequired=="N")
            {
                fieldOpts.append("|O_NULLOK");

            }

            if (sf[i].fieldVisible=="Y")
            {
                fieldOpts.append("|O_VISIBLE");
                fieldOptsBack.append("|A_UNDERLINE");
                labelOpts.append("|O_VISIBLE");
            }




            switch (sf[i].fieldType)
            {
            //0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC,5=ENUM,6=YN,7=Date
            case 0:
                break;
            case 1:
                type="TYPE_ALPHA";

                cout << "set_field_type("<<prefix<<"Fields["<<constants[x+1]<<"] ,"<<type<<",0);"<<endl;
                break;
            case 2:
                type="TYPE_ALNUM";

                cout << "set_field_type("<<prefix<<"Fields["<<constants[x+1]<<"] ,"<<type<<",0);"<<endl;
                break;
            case 3:
                just="|JUSTIFY_RIGHT";
                type="TYPE_INTEGER";
                def ="set_field_type(";
                def.append(prefix);
                def.append("Fields[");
                def.append(constants[x+1]);
                def.append("] ,");
                def.append(type);
                def.append(",0,");
                def.append(nines.substr(1,sf[i].fieldLen));
                def.append(");");
                cout << def<<endl;
                break;
            case 4:
                just="|JUSTIFY_RIGHT";
                type="TYPE_NUMERIC";
                def ="set_field_type(";
                def.append(prefix);
                def.append("Fields[");
                def.append(constants[x+1]);
                def.append("] ,");
                def.append(type);
                def.append(",");
                sprintf(NumToCharPointer, "%d", sf[i].fieldPrec);
                def.append(NumToCharPointer);
                def.append(",0,");
                def.append(nines.substr(1,sf[i].fieldLen));
                def.append(");");

                cout << def<<endl;
                break;
            case 5: //ENUM

                linestream<<sf[i].fieldEmumValues;

                while(getline(linestream,value,','))
                {
                    options.append("\"");
                    options.append(value);
                    options.append("\""",");
                }
                linestream.clear();

                cout<<"char * "<<sf[i].ScreenLocId<<"choices[] = { "<<options<<" (char *)0 };"<<endl;
                cout<< "set_field_type("<<prefix<<"Fields["<<constants[x+1]<<"],TYPE_ENUM, "<<sf[i].ScreenLocId<<"choices, false,false);"<<endl;
                break;
            case 6: //YN

                linestream<<sf[i].fieldEmumValues;

                while(getline(linestream,value,','))
                {
                    options.append("\"");
                    options.append(value);
                    options.append("\""",");
                }
                linestream.clear();

                cout<<"char * "<<sf[i].ScreenLocId<<"choices[] = { "<<options<<" (char *)0 };"<<endl;
                cout<< "set_field_type("<<prefix<<"Fields["<<constants[x+1]<<"],TYPE_ENUM, "<<sf[i].ScreenLocId<<"choices, false,false);"<<endl;
                break;
            case 7:
                cout<< "set_field_type("<<prefix<<"Fields["<<constants[x+1]<<"],PE_CustomDateType);"<<endl;
                break;
            default:
                break;



            }

            options="";

            fieldOpts.append(just);
            fieldOpts.append(")");
            fieldOptsBack.append(")");

            // label settings
            cout << "set_field_fore("<<prefix<<"Fields["<<constants[x]<<"]"<<",COLOR_PAIR(1));"<<endl;
            cout << "set_field_opts("<<prefix<<"Fields["<<constants[x]<<"],"<<labelOpts<<"); "<<endl;

            // field settings

            cout <<"set_field_back("<<prefix<<"Fields["<<constants[x+1]<<"], "<<fieldOptsBack<<"; "<<endl;
            cout << "set_field_opts("<<prefix<<"Fields["<<constants[x+1]<<"],"<<fieldOpts<<"; "<<endl;


            x+=2;
        }

        for (int i=0;i<numstxt;i++)
        {
            cout << "set_field_fore("<<prefix<<"Fields["<<(numsf*2)+i<<"]"<<",COLOR_PAIR(1));"<<endl;
            cout << "set_field_opts("<<prefix<<"Fields["<<(numsf*2)+i<<"],"<<labelOpts<<");"<<endl;
        }

        cout << prefix <<"Form = new_form("<<prefix<<"Fields);"<<endl;
        cout << "assert("<<prefix<<"Form != NULL);"<<endl;
        cout << "set_form_win("<<prefix<<"Form,winFullScreen);"<<endl;
        cout << "set_form_sub("<<prefix<<"Form,winFormArea);"<<endl;

        cout <<"move("<<sf[0].fieldRow+1<<","<<sf[0].fieldCol+1<<");"<<endl;

        cout << "}"<<endl;




cout << "void "<<prefix<<"FormCleanup"<<"(FIELD *"<<prefix<<"Fields[])"<<endl<<endl;
cout << "{"<<endl<<endl;

cout << "unpost_form( "<<prefix<<"Form);"<<endl<<endl;
cout << " free_form("<<prefix<<"Form);"<<endl<<endl;
cout << " for (int i=0; i<"<<(numsf*2)+numstxt<<";i++)"<<endl<<endl;
cout << "\t{"<<endl<<endl;
cout << "\t\t free_field("<<prefix<<"Fields[i]); "<<endl<<endl;
cout << "\t}"<<endl<<endl;
cout << "\tdelwin(winFormArea);"<<endl<<endl;
cout << "\tendwin(); "<<endl<<endl;
cout << "}"<<endl<<endl;


}
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

