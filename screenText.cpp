
#include "main.hpp"

static FORM *screenTextForm;
static WINDOW *winFormArea;
using namespace std;

int screenText(WINDOW *winFullScreen,WINDOW *winSqlMsgArea,WINDOW *winMsgArea,screenTexts stxt[], int &numstxt,string ScreenLayoutId,bool screenYXlocked[][79], bool &deleted,bool &saved,bool NewField)
{

    WINDOW* winFieldScreen;


    winFieldScreen = newwin(24, 80, 0, 0); // size to stdscr
    assert(winFieldScreen != NULL);
    wattron(winFieldScreen,COLOR_PAIR(1));
    box(winFieldScreen,0,0);

    keypad(winFieldScreen,TRUE);
    wrefresh(winFieldScreen);

    short formCurrentField;
    bool firstTime=true,quit=false;
    int key=0, ch=0;
    string answer="";



    static FIELD *screenEntryTexts[10];
    static FIELD*tempFptr;
    char NumToCharPointer[10];


screenTextStart:

    quit=false;
    screenTextSetupForm(screenEntryTexts,winFieldScreen);

    post_form(screenTextForm);


    set_field_buffer(screenEntryTexts[STXT_SCREEN_LOC_ID],0,stxt[numstxt].ScreenLocId.c_str());

    set_field_buffer(screenEntryTexts[STXT_TEXT_TEXT],0,stxt[numstxt].labelText.c_str());
    sprintf(NumToCharPointer, "%d", stxt[numstxt].labelRow);
    set_field_buffer(screenEntryTexts[STXT_TEXT_ROW],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", stxt[numstxt].labelCol);
    set_field_buffer(screenEntryTexts[STXT_TEXT_COL],0,NumToCharPointer);



    screenTextDisplayFunctionKeys(winFieldScreen);

    box(winFormArea, 0, 0);
    box(winFieldScreen, 0, 0);

//    wrefresh(winMsgArea);
    wrefresh(winFormArea);
    wrefresh(winFieldScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        screenTextFormCleanup(screenEntryTexts);
        goto screenTextStart;
    }


    move(4,8);
    tempFptr = current_field(screenTextForm);
    formCurrentField = field_index(tempFptr);


    while (quit!=true)
    {


        tempFptr = current_field(screenTextForm);
        formCurrentField = field_index(tempFptr);


        ch=getch();
        switch (ch)
        {
        case KEY_F(2):

            break;

        case  KEY_F(3)://delete
            if (confirmAction(winMsgArea,winFieldScreen,2,2,78,"Delete Text",COLOR_PAIR(1),KEY_F(3))==true)

            {
                quit=true;
                screenTextDeleteText(stxt,numstxt,ScreenLayoutId,screenYXlocked,winFullScreen,winSqlMsgArea);
                deleted=true;

            }
            pos_form_cursor(screenTextForm);
            touchwin(winFieldScreen);
            wrefresh(winFormArea);
            wrefresh(winFieldScreen);
            break;

        case KEY_F(4):// save
            //      sync buffer
            form_driver(screenTextForm, REQ_NEXT_FIELD);
            form_driver(screenTextForm, REQ_PREV_FIELD);

            if (confirmAction(winMsgArea,winFieldScreen,2,2,78,"Save",COLOR_PAIR(1),KEY_F(4))==true)
            {
                quit=true;
                saved=true;

                stxt[numstxt].labelText = TrimWhitespaces(field_buffer(screenEntryTexts[STXT_TEXT_TEXT], 0));
                stxt[numstxt].labelRow = atoi(TrimWhitespaces(field_buffer(screenEntryTexts[STXT_TEXT_ROW], 0)));
                stxt[numstxt].labelCol = atoi(TrimWhitespaces(field_buffer(screenEntryTexts[STXT_TEXT_COL], 0)));

            }

            if (NewField==true)

                screenTextInsertField(stxt,numstxt,winSqlMsgArea);

            else
                {
                    screenTextUpdateField(stxt,numstxt,screenYXlocked,winSqlMsgArea);

                }
            break;



        case KEY_F(7): //exit
            if (confirmAction(winMsgArea,winFieldScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
            }

            pos_form_cursor(screenTextForm);
            touchwin(winFieldScreen);
            wrefresh(winFormArea);
            wrefresh(winFieldScreen);
            break;
        case KEY_ENTER:
        case 9: // TAB
        case 10:
        case 13:

            //      sync buffer
            form_driver(screenTextForm, REQ_NEXT_FIELD);
            form_driver(screenTextForm, REQ_PREV_FIELD);
            screenTextVerify(stxt,numstxt,screenEntryTexts,winFullScreen,winFieldScreen,winMsgArea,winSqlMsgArea,ScreenLayoutId);

        form_driver(screenTextForm, REQ_NEXT_FIELD);
        form_driver(screenTextForm, REQ_BEG_LINE);
        break;


        case KEY_UP:
                form_driver(screenTextForm, REQ_PREV_FIELD);
                form_driver(screenTextForm, REQ_BEG_LINE);
                break;
        case KEY_LEFT:
            form_driver(screenTextForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(screenTextForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(screenTextForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(screenTextForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(screenTextForm, REQ_END_FIELD);
            break;
        case KEY_HOME:
            form_driver(screenTextForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(screenTextForm, ch);
            break;
        }
        wrefresh(winFormArea);


    }


    screenTextFormCleanup(screenEntryTexts);
    refresh();
    delwin(winFieldScreen);

    return 0;

}



bool  screenTextVerify(screenTexts stxt[],int numstxt,FIELD *screenEntryTexts[],WINDOW *winFullScreen,WINDOW *winFieldScreen, WINDOW *winMsgArea, WINDOW * winSqlMsgArea,string ScreenLayoutId)

{



    static FIELD*tempFptr;

    short formCurrentField;

    tempFptr = current_field(screenTextForm);
    formCurrentField = field_index(tempFptr);



 switch (formCurrentField)
    {

    case STXT_TEXT_TEXT:
        wrefresh(winFieldScreen);
        wrefresh(winFullScreen);
        pos_form_cursor(screenTextForm);
        refresh();
        break;

    default:
        break;
    }

    return true;
}


void screenTextSetupForm(FIELD *screenEntryTexts[],WINDOW *winFieldScreen)
{


    winFormArea = derwin(winFieldScreen, 10, 65, 1, 1);
    assert(winFormArea != NULL);
    wattron(winFormArea,COLOR_PAIR(1));
    wrefresh(winFormArea);




//              new_field(h,w,  row,col, offscreen, nbuffers);

    screenEntryTexts[0]             =new_field(1, 10,  0,   2,0,0);
    screenEntryTexts[STXT_KEY]      =new_field(1,  6,  0,  17,0,0);

//Screen Id           ____________________

    screenEntryTexts[2]                  = new_field(1, 15, 1, 2,0,0);// Label Screen Location
    screenEntryTexts[STXT_SCREEN_LOC_ID] = new_field(1, 10, 1, 18,0,0);// field Screen Location


//Label           ____________________

    screenEntryTexts[4]              =new_field(1,  5,  3,  2,0,0);
    screenEntryTexts[STXT_TEXT_TEXT] =new_field(1, 50,  3,  7,0,0);

//Label Row       __

    screenEntryTexts[6]               =new_field(1, 12,  5,   2,0,0);
    screenEntryTexts[STXT_TEXT_ROW]   =new_field(1,  2,  5,  18,0,0);

//Label Column    __

    screenEntryTexts[8]              =new_field(1, 12,  6,   2,0,0);
    screenEntryTexts[STXT_TEXT_COL]  =new_field(1,  2,  6,  18,0,0);



    screenEntryTexts[10] = NULL;


    for (int i=0;i<=9;i++)
        assert(screenEntryTexts[i] != NULL);


    set_field_buffer(screenEntryTexts[0], 0, "Key"); // hidden
    set_field_buffer(screenEntryTexts[STXT_KEY], 0, ""); //hidden
    set_field_buffer(screenEntryTexts[2], 0, "Screen Location");
    set_field_buffer(screenEntryTexts[STXT_SCREEN_LOC_ID], 0, "");

    set_field_buffer(screenEntryTexts[4], 0, "Text");

    set_field_buffer(screenEntryTexts[STXT_TEXT_TEXT], 0, "");
    set_field_buffer(screenEntryTexts[6], 0, "Text Row");
    set_field_buffer(screenEntryTexts[STXT_TEXT_ROW], 0, "");
    set_field_buffer(screenEntryTexts[8], 0, "Text Column");
    set_field_buffer(screenEntryTexts[STXT_TEXT_COL], 0, "");



    set_field_type(screenEntryTexts[STXT_KEY], TYPE_NUMERIC,0,1, 999999);// key

    // documenation states by default a field will except any data input by the user so
    // dont do a set_field_type if you need to have a text field with spaces.


    set_field_type(screenEntryTexts[STXT_TEXT_ROW], TYPE_NUMERIC,0,1, 24);
    set_field_type(screenEntryTexts[STXT_TEXT_COL], TYPE_NUMERIC,0,1, 79);

        for (int i=0; i<=9; ++i)

        if (i % 2 !=0) // this is a field
            set_field_back(screenEntryTexts[i], A_UNDERLINE|COLOR_PAIR(1));
        else // this is a label
            set_field_fore(screenEntryTexts[i], COLOR_PAIR(1));

    set_field_opts(screenEntryTexts[0], O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryTexts[STXT_KEY],  O_PUBLIC |O_AUTOSKIP);
    set_field_opts(screenEntryTexts[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryTexts[STXT_SCREEN_LOC_ID], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryTexts[4], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryTexts[STXT_TEXT_TEXT], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
    set_field_opts(screenEntryTexts[6], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryTexts[STXT_TEXT_ROW], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);
    set_field_opts(screenEntryTexts[8], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryTexts[STXT_TEXT_COL], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);


    screenTextForm = new_form(screenEntryTexts);
    assert(screenTextForm != NULL);
    set_form_win(screenTextForm,winFieldScreen);
    set_form_sub(screenTextForm,winFormArea);
}


void  screenTextFormCleanup(FIELD *screenEntryTexts[])
{
    unpost_form(screenTextForm);
    free_form(screenTextForm);
    for (int i=0; i<=9; ++i)
    {
        free_field(screenEntryTexts[i]);
    }
    delwin(winFormArea);
    wclear(stdscr);

}

int screenTextDisplayFunctionKeys(WINDOW *winFieldScreen)
{
    mvwprintw(winFieldScreen,22, 2, "                                                                          ");
    mvwprintw(winFieldScreen,22, 12, "F3 Delete");
    mvwprintw(winFieldScreen,22, 22, "F4 Save");
    mvwprintw(winFieldScreen,22, 42, "F7 Exit");
    //wattroff(winFieldScreen,A_REVERSE);
    //wrefresh(winFormArea);
    wrefresh(winFieldScreen);
    refresh();
}

int screenTextInsertField(screenTexts stxt[],int numstxt,WINDOW *winSqlMsgArea)
{
    string sql;
    char NumToCharPointer[10];
    int identity;
    dbScreenText row;

    row.ScreenLayoutId = stxt[numstxt].ScreenLayoutId;
    sprintf(NumToCharPointer, "Row%02dCol%02d", stxt[numstxt].labelRow,stxt[numstxt].labelCol);



    row.ScreenLocId = NumToCharPointer;

    row.labelText =stxt[numstxt].labelText;
    row.labelRow = stxt[numstxt].labelRow;
    row.labelCol = stxt[numstxt].labelCol;

    sql = "INSERT INTO screenText ";
    sql.append ("(ScreenLayoutId,ScreenLocId,labelText,labelRow, labelCol)");

    sql.append(" VALUES ( '");
    sql.append (row.ScreenLayoutId);
    sql.append("','");
    sql.append (row.ScreenLocId);
    sql.append("','");
    sql.append (row.labelText);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.labelRow);
    sql.append(NumToCharPointer);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.labelCol);
    sql.append(NumToCharPointer);
    sql.append("')");
    //debugMsg("sql ",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);


    return identity;
}

int screenTextDeleteText(screenTexts stxt[],int numstxt,string ScreenLayoutId,bool screenYXlocked[][79],WINDOW* winFullScreen,WINDOW* winSqlMsgArea)
{

    int fieldEndCol=0,curRow=0,len=0;
    curRow=stxt[numstxt].labelRow;
    len=strlen(stxt[numstxt].labelText.c_str());
    std::string sLabelSpace(len++, ' ');

    mvwaddstr(winFullScreen,stxt[numstxt].labelRow,stxt[numstxt].labelCol,sLabelSpace.c_str());
    wrefresh(winFullScreen);
    refresh();

    string sql;
    char NumToCharPointer[10];

    sql = "DELETE FROM screenText ";
    sql.append("WHERE ScreenLayoutId= ");
    sql.append("\"""");
    sql.append (ScreenLayoutId);
    sql.append("\"");


    sql.append(" AND ScreenLocId= ");
    sql.append("\"");

    sql.append(stxt[numstxt].ScreenLocId);

    sql.append("\"");
    //debugMsg("sql",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);


    stxt[numstxt].labelText="";
    stxt[numstxt].labelCol=0;
    stxt[numstxt].labelRow=0;

}


int screenTextUpdateField(screenTexts stxt[],int numstxt,bool screenYXlocked[][79],WINDOW* winSqlMsgArea)
{
    int fieldEndCol=0,curRow=0;
    curRow=stxt[numstxt].labelRow;

    string sql;


    sql = "DELETE FROM screenText ";
    sql.append("WHERE ScreenLayoutId= ");
    sql.append("\"""");
    sql.append (stxt[numstxt].ScreenLayoutId);
    sql.append("\"");
    sql.append(" AND ScreenLocId= ");
    sql.append("\"");
    sql.append(stxt[numstxt].ScreenLocId);
    sql.append("\"");
     //debugMsg("sql ",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);
    screenTextInsertField(stxt,numstxt,winSqlMsgArea);

}



void setscreenEntryTextsFromScreenText(screenTexts stxt[],int numstxt,FIELD *screenEntryTexts[],std::vector<dbScreenText>::iterator screenTextit,bool EditingRow)
{
    char NumToCharPointer[10];


    if (EditingRow==true)
    {
        set_field_buffer(screenEntryTexts[STXT_SCREEN_LOC_ID], 0,  screenTextit->ScreenLocId.c_str());

        set_field_buffer(screenEntryTexts[STXT_TEXT_TEXT], 0,  screenTextit->labelText.c_str());

        sprintf(NumToCharPointer, "%i", screenTextit->labelRow);
        set_field_buffer(screenEntryTexts[STXT_TEXT_ROW], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenTextit->labelCol);
        set_field_buffer(screenEntryTexts[STXT_TEXT_COL], 0,  NumToCharPointer);
        stxt[numstxt].ScreenLocId=screenTextit->ScreenLocId;
        stxt[numstxt].labelText=screenTextit->labelText;
        stxt[numstxt].labelRow=screenTextit->labelRow;
        stxt[numstxt].labelCol=screenTextit->labelCol;




    }
    else
    {

    for (int i=0; i<=9; ++i)

        if (i % 2 !=0)
            set_field_buffer(screenEntryTexts[i], 0,  "");


    }

}

int GetAllscreenEntryTexts(std::vector<dbScreenText> &screenEntryTextsResults,std::string ScreenLayoutId, WINDOW *winSqlMsgArea)
{

        int rows=0;
        sqlite3 *db;
        sqlite3_stmt *stmt;
        int rc = 0;
        int col, cols;
        string sql;
        char * csql;
        char msg[100];

        sql= "SELECT * FROM screenText WHERE ScreenLayoutId = ";
        sql.append ("'");
        sql.append (ScreenLayoutId);
        sql.append("'");
        sql.append(" ORDER BY ScreenLocId");


        //debugMsg("sql",sql.c_str(),0);

        screenEntryTextsResults.clear();

        csql=sql.c_str();

       // open the database file
       rc = sqlite3_open("devTool.db", &db);
       if( rc ){
         sprintf(msg, "Can't open database: %s\n", sqlite3_errmsg(db));
         sqlErr(msg,sql,winSqlMsgArea);
       }else{

         rc = sqlite3_prepare_v2(db, csql, -1, &stmt, 0);
         if( rc ){
           sprintf(msg, "SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
           sqlErr(msg,sql,winSqlMsgArea);
         }else{
           cols = sqlite3_column_count(stmt);
           // execute the statement

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

                   const char *txt = (const char*)sqlite3_column_text(stmt,col);


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
                 screenEntryTextsResults.push_back(texts);
                 break;
               default:
                 sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                 sqlErr(msg,sql,winSqlMsgArea);
                 rows=-1;
                 break;
             }
           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         }
         // close the database file
         sqlite3_close(db);
       }
      //return rc!=SQLITE_DONE;

    return rows;
}
