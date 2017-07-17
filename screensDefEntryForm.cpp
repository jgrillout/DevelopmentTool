
#include "main.hpp"

static FORM *screensDefForm;
static WINDOW *winFormArea;

using namespace std;



int screensDef(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea)

{

    short formCurrentField;
    bool firstTime=true,quit=false;
    int key=0, ch=0;


    static FIELD *screensDefFields[9];
    static FIELD*tempFptr;

    string ScreenLayoutId="",ScreenName="",tableId="";

screensDefStart:


    quit=false;
    bool EditingRow=false;
    bool NewRow=true;
    screensDefSetupForm(screensDefFields,winFullScreen);
    post_form(screensDefForm);
    set_current_field(screensDefForm,screensDefFields[SD_SCREEN_ID]);

    wattron(winFullScreen,A_REVERSE);
    mvwprintw(winFullScreen,22, 2, "F1 Layout");
    mvwprintw(winFullScreen,22, 12, "F2 Lookup");
    mvwprintw(winFullScreen,22, 22, "F3 Delete");
    mvwprintw(winFullScreen,22, 32, "F4 Save");
    mvwprintw(winFullScreen,22, 42, "F5 New");
    mvwprintw(winFullScreen,22, 52, "F7 Exit");
    wattroff(winFullScreen,A_REVERSE);
    box(winFormArea, 0, 0);
    //move(2,11);
    wrefresh(winFormArea);
    wrefresh(winFullScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        screensDefFormCleanup(screensDefFields);
        goto screensDefStart;
    }


    while (quit!=true)
    {

        tempFptr = current_field(screensDefForm);
        formCurrentField = field_index(tempFptr);
        ch=getch();
        switch (ch)
        {

        case KEY_F(1):
            //      sync buffer
            form_driver(screensDefForm, REQ_NEXT_FIELD);
            form_driver(screensDefForm, REQ_PREV_FIELD);
            if (NewRow==true)
                {

                    InsertScreen(screensDefFields,winSqlMsgArea);
                }
                else
                {

                    UpdateScreen(screensDefFields,winSqlMsgArea);
                }
            ScreenLayoutId = TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_ID], 0));
            ScreenName = TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_NAME], 0));
            tableId = TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_TABLE_ID], 0));

            if (ScreenLayoutId>"" && ScreenName>"" && tableId>"")
            {

                screenLayout(ScreenLayoutId,ScreenName,tableId);
                wclear(winFormArea);
                wrefresh(winFormArea);
                wclear(winFullScreen);
                wrefresh(winFullScreen);
                refresh();
                goto screensDefStart;
            }
            break;
        case KEY_F(2):
            if (formCurrentField==SD_SCREEN_ID)
            {
                def_prog_mode();
                endwin();
                screensDefDisplayLookup(screensDefFields,winSqlMsgArea,NewRow,EditingRow);
                refresh();
                pos_form_cursor(screensDefForm);

            }
            if (formCurrentField==SD_SCREEN_TABLE_ID)
            {
                def_prog_mode();
                endwin();
                screensDefDisplayTableLookup(screensDefFields,winSqlMsgArea);
                refresh();
                pos_form_cursor(screensDefForm);

            }
            break;

        case  KEY_F(3):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Delete Screen & Layout",COLOR_PAIR(1),KEY_F(3))==true)
            {
                DeleteScreen(screensDefFields,winSqlMsgArea);
                pos_form_cursor(screensDefForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                goto screensDefStart;
                break;
            }
            else
            {
                pos_form_cursor(screensDefForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                goto screensDefStart;
                break;
            }

        case KEY_F(4):
            //      sync buffer
            form_driver(screensDefForm, REQ_NEXT_FIELD);
            form_driver(screensDefForm, REQ_PREV_FIELD);
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Save",COLOR_PAIR(1),KEY_F(4))==true)
            {
                if (NewRow==true)
                {

                    InsertScreen(screensDefFields,winSqlMsgArea);
                }
                else
                {

                    UpdateScreen(screensDefFields,winSqlMsgArea);
                }
                 pos_form_cursor(screensDefForm);
                 touchwin(winFullScreen);
                 wrefresh(winFullScreen);
                 goto screensDefStart;
                 break;
            }
            else
            {

                pos_form_cursor(screensDefForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                break;
            }

        case KEY_F(5):
            goto screensDefStart;
            break;

        case KEY_F(7):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
            }

            pos_form_cursor(screensDefForm);
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;

            quit=true;
            break;
        case KEY_ENTER:
        case 9:
        case 10:
        case 13:



            if(formCurrentField==2)
            {

                //      sync buffer
                    form_driver(screensDefForm, REQ_NEXT_FIELD);
                    form_driver(screensDefForm, REQ_PREV_FIELD);

                if (screensDefVerify(screensDefFields,winFullScreen,winMsgArea,winSqlMsgArea,EditingRow,NewRow)==true)

                {

                    form_driver(screensDefForm, REQ_NEXT_FIELD);
                    form_driver(screensDefForm, REQ_BEG_LINE);

                }
                else
                {

                    form_driver(screensDefForm, REQ_NEXT_FIELD);
                    form_driver(screensDefForm, REQ_BEG_LINE);
                }
            }
            else
            {


                form_driver(screensDefForm, REQ_NEXT_FIELD);
                form_driver(screensDefForm, REQ_BEG_LINE);

            }

            break;


        case KEY_UP:

            form_driver(screensDefForm, REQ_PREV_FIELD);
            form_driver(screensDefForm, REQ_BEG_LINE);
            break;


        case KEY_LEFT:
            form_driver(screensDefForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(screensDefForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(screensDefForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(screensDefForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(screensDefForm, REQ_END_FIELD);
            break;
        case KEY_HOME:
            form_driver(screensDefForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(screensDefForm, ch);
            break;
        }
        wrefresh(winFormArea);

    }


    screensDefFormCleanup(screensDefFields);



    return 0;

}


void  screensDefFormCleanup(FIELD *screensDefFields[])
{
    unpost_form(screensDefForm);
    free_form(screensDefForm);
    for (int i=0; i<3; i++)
    {
        free_field(screensDefFields[i]);
    }
    delwin(winFormArea);

    endwin();
}

void screensDefSetupForm(FIELD *screensDefFields[],WINDOW *winFullScreen)

{

init_pair(1,COLOR_GREEN,COLOR_BLACK);
winFormArea = derwin(winFullScreen, 20, 78, 1, 1);
assert(winFormArea != NULL);
wattron(winFormArea,COLOR_PAIR(1));
wrefresh(winFormArea);




//              new_field(h,w,  tp,left, offscreen, nbuffers);
screensDefFields[0] = new_field(1,  5, 0, 0,0,0);// hidden  key id
screensDefFields[1] = new_field(1, 10, 2, 1,0,0);// Label screen layout id
screensDefFields[SD_SCREEN_ID] = new_field(1, 20, 2,15,0,0);// field screen layout id
screensDefFields[3] = new_field(1, 12, 4, 1,0,0);// Label screen Name
screensDefFields[SD_SCREEN_NAME] = new_field(1, 30, 4,15,0,0);// field screen Name
screensDefFields[5] = new_field(1,  8, 6, 1,0,0);// Label table id
screensDefFields[SD_SCREEN_TABLE_ID] = new_field(1,  15, 6,15,0,0);// field table id
screensDefFields[7] = new_field(1,  10, 8, 1,0,0);// Label table name
screensDefFields[SD_SCREEN_TABLE_NAME] = new_field(1,  30, 8,15,0,0);// field table name

screensDefFields[9] = NULL;
 for (int i=0;i<9;i++)
 {
	assert(screensDefFields[i] !=NULL);

 }
 set_field_buffer(screensDefFields[0] ,0,"");//hidden key id

set_field_buffer(screensDefFields[1] ,0,"Screen Id");//Label SD_SCREEN_ID
set_field_buffer(screensDefFields[SD_SCREEN_ID] ,0,"");// field SD_SCREEN_ID
set_field_buffer(screensDefFields[3] ,0,"Screen Name");//Label SD_SCREEN_NAME
set_field_buffer(screensDefFields[SD_SCREEN_NAME] ,0,"");// field SD_SCREEN_NAME
set_field_buffer(screensDefFields[5] ,0,"Table Id");//Label SD_SCREEN_TABLE_ID
set_field_buffer(screensDefFields[SD_SCREEN_TABLE_ID] ,0,"");// field LENGTH
set_field_buffer(screensDefFields[7] ,0,"Table Name");//Label SD_SCREEN_TABLE_NAME
set_field_buffer(screensDefFields[SD_SCREEN_TABLE_NAME] ,0,"");// field SD_SCREEN_TABLE_NAME


for (int i=0;i<=9;i++)
{
	 if (i % 2 ==0)
		 set_field_back(screensDefFields[i], A_UNDERLINE|COLOR_PAIR(1));
	 else
		 set_field_fore(screensDefFields[i],COLOR_PAIR(1));
}
set_field_opts(screensDefFields[0] ,  O_PUBLIC | O_AUTOSKIP);

set_field_opts(screensDefFields[1] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(screensDefFields[SD_SCREEN_ID] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(screensDefFields[3] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(screensDefFields[SD_SCREEN_NAME] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(screensDefFields[5] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(screensDefFields[SD_SCREEN_TABLE_ID] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(screensDefFields[7] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(screensDefFields[SD_SCREEN_TABLE_NAME] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);


screensDefForm = new_form(screensDefFields);
assert(screensDefForm != NULL);
set_form_win(screensDefForm,winFullScreen);
set_form_sub(screensDefForm,winFormArea);
move(3,16);
}
int InsertScreen(FIELD *screensDefFields[],WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];

    dbscreensDef row;

    row.idscreens = atoi(TrimWhitespaces(field_buffer(screensDefFields[0], 0)));
    row.ScreenLayoutId = TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_ID], 0));
    row.ScreenName = TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_NAME], 0));
    row.tableId = TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_TABLE_ID], 0));


    sql = "INSERT INTO screens (ScreenLayoutId,ScreenName,tableId)";
    sql.append(" VALUES ( \"");
    sql.append (row.ScreenLayoutId);
    sql.append("\""",\"");
    sql.append (row.ScreenName);
    sql.append("\""",\"");
    sql.append (row.tableId);
    sql.append("\""")");
    //debugMsg("sql",sql.c_str(),0);

    bool result=true;
    result=execSQLAction(sql,winSqlMsgArea);
    return 0;
}

int UpdateScreen(FIELD *screensDefFields[],WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];


    int key;



    key=atoi(TrimWhitespaces(field_buffer(screensDefFields[0], 0)));

    sql = "UPDATE screens SET ";
    sql.append ("ScreenLayoutId= \"");
    sql.append (TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_ID], 0)));
    sql.append("\""" , ScreenName= \"");
    sql.append(TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_NAME], 0)));
    sql.append("\""" , tableId= \"");
    sql.append(TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_TABLE_ID], 0)));


    sql.append("\""" WHERE idscreens = ");
    sprintf(NumToCharPointer, "%d", key);
    sql.append(NumToCharPointer);
    //debugMsg("sql",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);



    return 0;


}

int DeleteScreen(FIELD *screensDefFields[],WINDOW *winSqlMsgArea)
{



    string sql;
    string ScreenLayoutId;
    int key;
    key=atoi(TrimWhitespaces(field_buffer(screensDefFields[0], 0)));
    char NumToCharPointer[10];

    sprintf(NumToCharPointer, "%d", key);

    sql = "DELETE FROM screens WHERE idscreens= ";
    sql.append(NumToCharPointer);

    execSQLAction(sql,winSqlMsgArea);

    key=TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_ID], 0));
    sql = "DELETE FROM screenFields WHERE ScreenLayoutId= ";
    sql.append("\"");
    sql.append (TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_ID], 0)));
    sql.append("\"");
    //debugMsg("sql",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);

    return 0;
}



bool  screensDefVerify(FIELD *screensDefFields[],WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow)
{


    std::vector<dbscreensDef>::iterator screensDefit;
    std::vector<dbscreensDef> screensDefResults;
    std::vector<tables>::iterator tablesit;
    std::vector<tables> tablesResults;

    char NumToCharPointer[10];
    short formCurrentField;
    bool Result=false;
    static FIELD*tempFptr;
    std::string lookup;


    tempFptr = current_field(screensDefForm);
    formCurrentField = field_index(tempFptr);


    switch (formCurrentField)
    {


    case SD_SCREEN_ID:
        lookup=TrimWhitespaces(field_buffer(screensDefFields[SD_SCREEN_ID], 0));
        screensDefResults=GetScreen(lookup,winSqlMsgArea);

        if (screensDefResults.empty()==false)
        {


            EditingRow=true;
            NewRow=false;
            Result=true;
            screensDefit=screensDefResults.begin();
            SetscreensDef(screensDefFields,screensDefit,EditingRow);
            lookup=screensDefit->tableId;
            tablesResults=GetTable(lookup,winSqlMsgArea);
            tablesit=tablesResults.begin();

            set_field_buffer(screensDefFields[SD_SCREEN_TABLE_NAME], 0,  tablesit->tableName.c_str());



        }
        else
        {


            EditingRow=false;
            NewRow=true;
            Result=false;
            set_current_field(screensDefForm,screensDefFields[SD_SCREEN_ID]);
        }

        touchwin(winFullScreen);
        wrefresh(winFormArea);
        wrefresh(winFullScreen);
        pos_form_cursor(screensDefForm);
        refresh();
        break;

    default:
        break;
    }

    return Result;
}

int screensDefDisplayLookup(FIELD *screensDefFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow)
{

    ITEM **myItems;
    int c,r;

    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;
    std::vector<dbscreensDef> screensDefResults;
    std::vector<dbscreensDef>::iterator screensDefit;
    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;

    static FIELD*tempFptr;

    short formCurrentField;
    tempFptr = current_field(screensDefForm);
    formCurrentField = field_index(tempFptr);
    EditingRow= false;
    NewRow= false;


    NumChoices=GetAllScreens(screensDefResults,winSqlMsgArea);


    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (screensDefit = screensDefResults.begin(); screensDefit != screensDefResults.end(); ++screensDefit)
    {

        myItems[r] = new_item(screensDefit->ScreenLayoutId.c_str(),screensDefit->ScreenName.c_str());

        intptrs.push_back(&screensDefit->idscreens);

        set_item_userptr(myItems[r], (int *)*intptrs.back());
        ++r;

    }


    if (NumChoices!=0)
    {
        /* Create menu */
        myLookUpMenu = new_menu((ITEM **)myItems);
        set_menu_fore(myLookUpMenu, COLOR_PAIR(1) | A_REVERSE);
        set_menu_back(myLookUpMenu, COLOR_PAIR(1));
        /* Create the window to be associated with the menu */
        //newwin(rows,cols,y_org,x_org)
        //derwin(WINDOW *orig, int nlines, int ncols,int begin_y, int begin_x);
        int winSize = max(NumChoices,5);
        if (winSize<4) winSize=4;
        myLookUpMenuWin = newwin(winSize, 40, 2, 4);
        set_menu_win(myLookUpMenu, myLookUpMenuWin);
        set_menu_sub(myLookUpMenu, derwin(myLookUpMenuWin, (winSize-2), 38, 1, 1));
        set_menu_format(myLookUpMenu, (winSize-2), 1);
        keypad(myLookUpMenuWin, TRUE);
        //set_menu_format(myLookUpMenu, 10, 1);
        set_menu_mark(myLookUpMenu, " > ");

        wattron(myLookUpMenuWin,COLOR_PAIR(1)); // color of box lines
        box(myLookUpMenuWin, 0, 0);
        refresh();
        post_menu(myLookUpMenu);
        wrefresh(myLookUpMenuWin);

        int result=0;

        while(exitMenu==false)
        {
            c = wgetch(myLookUpMenuWin);
            switch(c)
            {
            case KEY_DOWN:
                menu_driver(myLookUpMenu,REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(myLookUpMenu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(myLookUpMenu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(myLookUpMenu, REQ_SCR_UPAGE);
                break;
            case 9: // TAB
                exitMenu=true;
                EditingRow=false;
                NewRow=false;
                screensDefit = screensDefResults.begin();
                SetscreensDef(screensDefFields,screensDefit,EditingRow); // screensDefit is not  used have to pass something in this case
                break; // test exit without choice
            case KEY_ENTER:
            case 10:
            case 13:

                ITEM *cur;
                cur = current_item(myLookUpMenu);
                exitMenu=true;
                retValue = (int)item_userptr(cur);
                EditingRow=false;
                NewRow=true;

                for (screensDefit = screensDefResults.begin(); screensDefit != screensDefResults.end(); ++screensDefit)
                {
                    if (screensDefit->idscreens==retValue)
                    {

                        EditingRow=true;
                        NewRow=false;
                        SetscreensDef(screensDefFields,screensDefit,EditingRow);
                        set_current_field(screensDefForm,screensDefFields[SD_SCREEN_ID]);

                        break;

                    }

                }
                break;
            } //switch
            box(myLookUpMenuWin, 0, 0);
            wrefresh(winFormArea);
            wrefresh(myLookUpMenuWin);

        } // while

    }// if (NumChoices!=0)



    if (NumChoices!=0)
    {
        unpost_menu(myLookUpMenu);
        free_menu(myLookUpMenu);
        for(int i = 0; i < NumChoices; ++i)
            free_item(myItems[i]);
    }
    delwin(myLookUpMenuWin);
    endwin();


    if (NumChoices!=0)
    {
        screensDefResults.clear();
        intptrs.clear();
    }



    return retValue;// not used yet

}
int screensDefDisplayTableLookup(FIELD *screensDefFields[],WINDOW *winSqlMsgArea)
{

    ITEM **myItems;
    int c,r;

    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;
    std::vector<tables> tablesDefResults;
    std::vector<tables>::iterator tablesDefit;
    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;

    static FIELD*tempFptr;



    short formCurrentField;
    tempFptr = current_field(screensDefForm);
    formCurrentField = field_index(tempFptr);



    NumChoices=GetAllTables(tablesDefResults,winSqlMsgArea);


    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (tablesDefit = tablesDefResults.begin(); tablesDefit != tablesDefResults.end(); ++tablesDefit)
    {

        myItems[r] = new_item(tablesDefit->tableId.c_str(),tablesDefit->tableName.c_str());

        intptrs.push_back(&tablesDefit->id);

        set_item_userptr(myItems[r], (int *)*intptrs.back());
        ++r;

    }
    //debugMsg("inside tablesDefDisplayLookup","D",0);

    if (NumChoices!=0)
    {
        /* Create menu */
        myLookUpMenu = new_menu((ITEM **)myItems);
        set_menu_fore(myLookUpMenu, COLOR_PAIR(1) | A_REVERSE);
        set_menu_back(myLookUpMenu, COLOR_PAIR(1));
        /* Create the window to be associated with the menu */
        //newwin(rows,cols,y_org,x_org)
        //derwin(WINDOW *orig, int nlines, int ncols,int begin_y, int begin_x);
        int winSize = max(NumChoices,5);
        if (winSize<4) winSize=4;
        myLookUpMenuWin = newwin(winSize, 40, 2, 4);
        set_menu_win(myLookUpMenu, myLookUpMenuWin);
        set_menu_sub(myLookUpMenu, derwin(myLookUpMenuWin, (winSize-2), 38, 1, 1));
        set_menu_format(myLookUpMenu, (winSize-2), 1);
        keypad(myLookUpMenuWin, TRUE);
        //set_menu_format(myLookUpMenu, 10, 1);
        set_menu_mark(myLookUpMenu, " > ");

        wattron(myLookUpMenuWin,COLOR_PAIR(1)); // color of box lines
        box(myLookUpMenuWin, 0, 0);
        refresh();
        post_menu(myLookUpMenu);
        wrefresh(myLookUpMenuWin);

        int result=0;

        while(exitMenu==false)
        {
            c = wgetch(myLookUpMenuWin);
            switch(c)
            {
            case KEY_DOWN:
                menu_driver(myLookUpMenu,REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(myLookUpMenu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(myLookUpMenu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(myLookUpMenu, REQ_SCR_UPAGE);
                break;
            case 9: // TAB
                exitMenu=true;
                break; // test exit without choice
            case KEY_ENTER:
            case 10:
            case 13:

                ITEM *cur;
                cur = current_item(myLookUpMenu);
                exitMenu=true;
                retValue = (int)item_userptr(cur);

                for (tablesDefit = tablesDefResults.begin(); tablesDefit != tablesDefResults.end(); ++tablesDefit)
                {
                    if (tablesDefit->id==retValue)
                    {

                        set_field_buffer(screensDefFields[SD_SCREEN_TABLE_ID], 0,  tablesDefit->tableId.c_str());
                        set_field_buffer(screensDefFields[SD_SCREEN_TABLE_NAME], 0,  tablesDefit->tableName.c_str());
                        set_current_field(screensDefForm,screensDefFields[SD_SCREEN_TABLE_NAME]);
                        break;
                    }

                }
                break;
            } //switch
            box(myLookUpMenuWin, 0, 0);
            wrefresh(winFormArea);
            wrefresh(myLookUpMenuWin);

        } // while

    }// if (NumChoices!=0)


    if (NumChoices!=0)
    {
        unpost_menu(myLookUpMenu);
        free_menu(myLookUpMenu);
        for(int i = 0; i < NumChoices; ++i)
            free_item(myItems[i]);
    }
    delwin(myLookUpMenuWin);
    endwin();


    if (NumChoices!=0)
    {
        tablesDefResults.clear();
        intptrs.clear();
    }



    return retValue;// not used yet

}
void SetscreensDef(FIELD *screensDefFields[],std::vector<dbscreensDef>::iterator screensDefit,bool EditingRow)
{
    char NumToCharPointer[10];

    if (EditingRow==true)
    {


        sprintf(NumToCharPointer, "%i", screensDefit->idscreens);
        set_field_buffer(screensDefFields[0], 0, NumToCharPointer);
        set_field_buffer(screensDefFields[SD_SCREEN_ID], 0,  screensDefit->ScreenLayoutId.c_str());
        set_field_buffer(screensDefFields[SD_SCREEN_NAME], 0,  screensDefit->ScreenName.c_str());
        set_field_buffer(screensDefFields[SD_SCREEN_TABLE_ID], 0,  screensDefit->tableId.c_str());


    }
    else
    {


        for (int i=0;i<9;i++)
        {

            if (i % 2==0)
                set_field_buffer(screensDefFields[i], 0,  "");
        }


    }

}
std::vector<dbscreensDef>  GetScreen(std::string lookupId,WINDOW *winSqlMsgArea)
{

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = 0;
	int rows=0,col, cols;
	string sql;
	char* csql;
	char msg[100];

    std::vector<dbscreensDef> screensDefResults;



    bool result=true;

    screensDefResults.clear();

    sql="SELECT * ";
    sql.append (" FROM screens WHERE ScreenLayoutId = ");
    sql.append ("'");
    sql.append (lookupId);
    sql.append("'");
    //debugMsg(sql.c_str(),"",99);
    csql=sql.c_str();

       // open the database file
       rc = sqlite3_open("devTool.db", &db);
       if( rc ){
        sprintf(msg,"Can't open database: %s",sqlite3_errmsg(db));
        sqlErr(msg,sql,winSqlMsgArea);

       }else{

         rc = sqlite3_prepare_v2(db, csql, -1, &stmt, 0);
         if( rc ){
            sprintf(msg,"SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
            sqlErr(msg,sql,winSqlMsgArea);

         }else{
           cols = sqlite3_column_count(stmt);
           // execute the statement

           do{
            rows++;
            dbscreensDef screensDef;
             rc = sqlite3_step(stmt);
             switch( rc ){
               case SQLITE_DONE:
                 break;
               case SQLITE_ROW:

                 for( col=0; col<cols; col++){

                   const char *txt = (const char*)sqlite3_column_text(stmt,col);


                   switch (col)
                   {
                    case 0:
                        screensDef.idscreens=atoi(txt);
                        break;
                    case 1:
                        screensDef.ScreenLayoutId=txt;
                        break;
                    case 2:
                        screensDef.ScreenName=txt;
                        break;
                    case 3:
                        screensDef.tableId=txt;
                        break;


                   }



                 }//for
                 screensDefResults.push_back(screensDef);
                 break;
               default:
                  sprintf(msg,"Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                  sqlErr(msg,sql,winSqlMsgArea);

                 break;
             } //switch
           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         }
         // close the database file
         sqlite3_close(db);
       }



return screensDefResults;





} // end function

int GetAllScreens(std::vector<dbscreensDef> &screensDefResults,WINDOW *winSqlMsgArea)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = 0;
    int rows=0,col, cols;
    string sql;
    char* csql;
    char msg[100];


    screensDefResults.clear();

    sql="SELECT * ";
    sql.append (" FROM screens ORDER BY ScreenLayoutId ");
    csql=sql.c_str();

       // open the database file
       rc = sqlite3_open("devTool.db", &db);
       if( rc ){
        sprintf(msg,"Can't open database: %s",sqlite3_errmsg(db));
        sqlErr(msg,sql,winSqlMsgArea);

       }else{

         rc = sqlite3_prepare_v2(db, csql, -1, &stmt, 0);
         if( rc ){
            sprintf(msg,"SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
            sqlErr(msg,sql,winSqlMsgArea);

         }else{
           cols = sqlite3_column_count(stmt);

           do{

            dbscreensDef screensDef;
             rc = sqlite3_step(stmt);
             switch( rc ){
               case SQLITE_DONE:
                 break;
               case SQLITE_ROW:
                rows++;

                 for( col=0; col<cols; col++){

                   const char *txt = (const char*)sqlite3_column_text(stmt,col);


                   switch (col)
                   {
                    case 0:
                        screensDef.idscreens=atoi(txt);
                        break;
                    case 1:
                        screensDef.ScreenLayoutId=txt;
                        break;
                    case 2:
                        screensDef.ScreenName=txt;
                        break;
                    case 3:
                        screensDef.tableId=txt;
                        break;


                   }



                 }//for
                 screensDefResults.push_back(screensDef);
                 break;
               default:
                  sprintf(msg,"Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                  sqlErr(msg,sql,winSqlMsgArea);

                 break;
             } //switch
           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         }
         // close the database file
         sqlite3_close(db);
       }

 return screensDefResults.size();

} // end function


