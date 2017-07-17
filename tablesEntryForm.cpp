
#include "main.hpp"
static FORM *TablesEntryForm;
static WINDOW *winFormArea;
static WINDOW *winFullScreen;
static WINDOW *winMsgArea;
using namespace std;


int tablesEntry(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea)
{

    short formCurrentField;
    bool firstTime=true,quit=false;
    int key=0, ch=0;
    bool EditingRow=false;
    bool NewRow=true;

    static FIELD *tablesEntryFields[7];
    static FIELD*tempFptr;

tablesEntryStart:

    quit=false;
    tablesEntrySetupForm(tablesEntryFields,winFullScreen);
    post_form(TablesEntryForm);
    set_current_field(TablesEntryForm,tablesEntryFields[TE_TABLE_ID]);

    wattron(winFullScreen,A_REVERSE);
    mvwprintw(winFullScreen,22, 2, "F2 Lookup");
    mvwprintw(winFullScreen,22, 12, "F3 Delete");
    mvwprintw(winFullScreen,22, 22, "F4 Save");
    mvwprintw(winFullScreen,22, 32, "F5 New");
    mvwprintw(winFullScreen,22, 42, "F7 Exit");
    wattroff(winFullScreen,A_REVERSE);
    box(winFormArea, 0, 0);
    //move(2,11);
    wrefresh(winFormArea);
    wrefresh(winFullScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        tablesEntryFormCleanup(tablesEntryFields);
        goto tablesEntryStart;
    }


    while (quit!=true)
    {

        tempFptr = current_field(TablesEntryForm);
        formCurrentField = field_index(tempFptr);
        ch=getch();
        switch (ch)
        {

        case KEY_F(1):
            break;
        case KEY_F(2):
            if (formCurrentField==TE_TABLE_ID)
            {
                def_prog_mode();
                endwin();
                tablesEntryFieldsDisplayLookup(tablesEntryFields,winSqlMsgArea,NewRow,EditingRow);
                refresh();
                pos_form_cursor(TablesEntryForm);

            }
            break;

        case  KEY_F(3):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Delete Field",COLOR_PAIR(1),KEY_F(3))==true)
            {
                tablesEntryDeleteTable(tablesEntryFields,winSqlMsgArea);
                pos_form_cursor(TablesEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                goto tablesEntryStart;
                break;
            }
            else
            {
                pos_form_cursor(TablesEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                goto tablesEntryStart;
                break;
            }

        case KEY_F(4):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Save",COLOR_PAIR(1),KEY_F(4))==true)
            {
                if (NewRow==true)
                {

                    tablesEntryInsertTable(tablesEntryFields,winSqlMsgArea);
                }
                else
                {

                    tablesEntryUpdateTable(tablesEntryFields,winSqlMsgArea);
                }
                 pos_form_cursor(TablesEntryForm);
                 touchwin(winFullScreen);
                 wrefresh(winFullScreen);
                 goto tablesEntryStart;
                 break;
            }
            else
            {

                pos_form_cursor(TablesEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                break;
            }

        case KEY_F(5):
            goto tablesEntryStart;
            break;

        case KEY_F(7):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
            }


            pos_form_cursor(TablesEntryForm);
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;

        case KEY_ENTER:
        case 9:
        case 10:
        case 13:


            if(formCurrentField==2)
            {



                //      sync buffer
                    form_driver(TablesEntryForm, REQ_NEXT_FIELD);
                    form_driver(TablesEntryForm, REQ_PREV_FIELD);

                if (tablesEntryVerify(tablesEntryFields,winFullScreen,winMsgArea,winSqlMsgArea,EditingRow,NewRow)==true)

                {



                    form_driver(TablesEntryForm, REQ_NEXT_FIELD);
                    form_driver(TablesEntryForm, REQ_BEG_LINE);

                }
                else
                {

                    form_driver(TablesEntryForm, REQ_NEXT_FIELD);
                    form_driver(TablesEntryForm, REQ_BEG_LINE);
                }
            }
            else
            {


                form_driver(TablesEntryForm, REQ_NEXT_FIELD);
                form_driver(TablesEntryForm, REQ_BEG_LINE);

            }

            break;


        case KEY_UP:


            form_driver(TablesEntryForm, REQ_PREV_FIELD);
            form_driver(TablesEntryForm, REQ_BEG_LINE);
            break;


        case KEY_LEFT:
            form_driver(TablesEntryForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(TablesEntryForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(TablesEntryForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(TablesEntryForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(TablesEntryForm, REQ_END_FIELD);
            break;
        case KEY_HOME:
            form_driver(TablesEntryForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(TablesEntryForm, ch);
            break;
        }
        wrefresh(winFormArea);

    }


    tablesEntryFormCleanup(tablesEntryFields);



    return 0;

}


void  tablesEntryFormCleanup(FIELD *tablesEntryFields[])
{
    unpost_form(TablesEntryForm);
    free_form(TablesEntryForm);
    for (int i=0; i<7; i++)
    {
        free_field(tablesEntryFields[i]);
    }
    delwin(winFormArea);

    endwin();
}

void tablesEntrySetupForm(FIELD *tablesEntryFields[],WINDOW *winFullScreen)

{

init_pair(1,COLOR_GREEN,COLOR_BLACK);
winFormArea = derwin(winFullScreen, 20, 78, 1, 1);
assert(winFormArea != NULL);
wattron(winFormArea,COLOR_PAIR(1));
wrefresh(winFormArea);




//              new_field(h,w,  tp,left, offscreen, nbuffers);
tablesEntryFields[0] = new_field(1,  5, 0, 0,0,0);// hidden  key id
tablesEntryFields[1] = new_field(1,  8, 2, 1,0,0);// Label Table Id
tablesEntryFields[TE_TABLE_ID] = new_field(1, 15, 2,15,0,0);// field Table Id
tablesEntryFields[3] = new_field(1,  10, 4, 1,0,0);// Label Table Name
tablesEntryFields[TE_TABLE_NAME] = new_field(1, 30, 4,15,0,0);// field Table Name
tablesEntryFields[5] = new_field(1,  8, 6, 1,0,0);// Label database
tablesEntryFields[TE_DATABASE] = new_field(1,  10, 6,15,0,0);// field database

tablesEntryFields[7] = NULL;
 for (int i=0;i<7;i++)
 {
	assert(tablesEntryFields[i] !=NULL);

 }
 set_field_buffer(tablesEntryFields[0] ,0,"");//hidden key id

set_field_buffer(tablesEntryFields[1] ,0,"Table Id");//Label Table Id
set_field_buffer(tablesEntryFields[TE_TABLE_ID] ,0,"");// field Table Id
set_field_buffer(tablesEntryFields[3] ,0,"Table Name");//Label Name
set_field_buffer(tablesEntryFields[TE_TABLE_NAME] ,0,"");// field field name
set_field_buffer(tablesEntryFields[5] ,0,"Database");//Label Database
set_field_buffer(tablesEntryFields[TE_DATABASE] ,0,"");// field field name


set_field_type(tablesEntryFields[TE_TABLE_ID] ,TYPE_ALPHA,0);



for (int i=0;i<=7;i++)
{
	 if (i % 2 ==0)
		 set_field_back(tablesEntryFields[i], A_UNDERLINE|COLOR_PAIR(1));
	 else
        set_field_fore(tablesEntryFields[i],COLOR_PAIR(1));


}
set_field_opts(tablesEntryFields[0] ,  O_PUBLIC | O_AUTOSKIP);

set_field_opts(tablesEntryFields[1] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(tablesEntryFields[TE_TABLE_ID] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(tablesEntryFields[3] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(tablesEntryFields[TE_TABLE_NAME] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(tablesEntryFields[5] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(tablesEntryFields[TE_DATABASE] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

TablesEntryForm = new_form(tablesEntryFields);
assert(TablesEntryForm != NULL);
set_form_win(TablesEntryForm,winFullScreen);
set_form_sub(TablesEntryForm,winFormArea);
move(3,16);
}
int tablesEntryInsertTable(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];

    tables row;

    row.id = atoi(TrimWhitespaces(field_buffer(tablesEntryFields[0], 0)));
    row.tableId = TrimWhitespaces(field_buffer(tablesEntryFields[TE_TABLE_ID], 0));
    row.tableName = TrimWhitespaces(field_buffer(tablesEntryFields[TE_TABLE_NAME], 0));
    row.datbase = TrimWhitespaces(field_buffer(tablesEntryFields[TE_DATABASE], 0));



    sql = "INSERT INTO tables (tableId,tableName,datbase)";
    sql.append(" VALUES ( \"");
    sql.append (row.tableId);
    sql.append("\""",\"");
    sql.append (row.tableName);
    sql.append("\""",\"");
    sql.append (row.datbase);
    sql.append("\""")");
    //debugMsg("sql",sql.c_str(),0);

    bool result=true;
    result=execSQLAction(sql,winSqlMsgArea);
    return 0;
}

int tablesEntryUpdateTable(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];


    int key;


    key=atoi(TrimWhitespaces(field_buffer(tablesEntryFields[0], 0)));

    sql = "UPDATE tables SET ";
    sql.append ("tableId= \"");
    sql.append (TrimWhitespaces(field_buffer(tablesEntryFields[TE_TABLE_ID], 0)));
    sql.append("\""" , tableName= \"");
    sql.append(TrimWhitespaces(field_buffer(tablesEntryFields[TE_TABLE_NAME], 0)));
    sql.append("\""" , datbase= \"");
    sql.append(TrimWhitespaces(field_buffer(tablesEntryFields[TE_DATABASE], 0)));


    sql.append("\""" WHERE id = ");
    sprintf(NumToCharPointer, "%d", key);
    sql.append(NumToCharPointer);
    execSQLAction(sql,winSqlMsgArea);



    return 0;


}

int tablesEntryDeleteTable(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea)
{
    string sql;
    int key;
    key=atoi(TrimWhitespaces(field_buffer(tablesEntryFields[0], 0)));
    char NumToCharPointer[10];

    sprintf(NumToCharPointer, "%d", key);

    sql = "DELETE FROM tables WHERE id= ";
    sql.append(NumToCharPointer);

    execSQLAction(sql,winSqlMsgArea);


    return 0;
}



bool  tablesEntryVerify(FIELD *tablesEntryFields[],WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow)
{


    std::vector<tables>::iterator tablesit;
    std::vector<tables> tablesResults;

    char NumToCharPointer[10];
    short formCurrentField;
    bool Result=false;
    static FIELD*tempFptr;
    std::string lookup;


    tempFptr = current_field(TablesEntryForm);
    formCurrentField = field_index(tempFptr);


    switch (formCurrentField)
    {


    case 2: // Field  Id
        lookup=TrimWhitespaces(field_buffer(tablesEntryFields[TE_TABLE_ID], 0));
        tablesResults=GetTable(lookup,winSqlMsgArea);


        if (tablesResults.empty()==false)
        {

            EditingRow=true;
            NewRow=false;
            Result=true;
            tablesit=tablesResults.begin();
            SetTables(tablesEntryFields,tablesit,EditingRow);


        }
        else
        {

            EditingRow=false;
            NewRow=true;
            Result=false;
            set_current_field(TablesEntryForm,tablesEntryFields[TE_TABLE_ID]);
        }

        touchwin(winFullScreen);
        wrefresh(winFormArea);
        wrefresh(winFullScreen);
        pos_form_cursor(TablesEntryForm);
        refresh();
        break;

    default:
        break;
    }

    return Result;
}

int tablesEntryFieldsDisplayLookup(FIELD *tablesEntryFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow)
{

    ITEM **myItems;
    int c,r;

    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;
    std::vector<tables> tablesResults;
    std::vector<tables>::iterator tablesit;
    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;

    static FIELD*tempFptr;



    short formCurrentField;
    tempFptr = current_field(TablesEntryForm);
    formCurrentField = field_index(tempFptr);
    EditingRow= false;
    NewRow= false;


    NumChoices=GetAllTables(tablesResults,winSqlMsgArea);


    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (tablesit = tablesResults.begin(); tablesit != tablesResults.end(); ++tablesit)
    {

        myItems[r] = new_item(tablesit->tableId.c_str(),tablesit->tableName.c_str());

        intptrs.push_back(&tablesit->id);

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
                // need to define this
                SetTables(tablesEntryFields,tablesit,EditingRow); // tablesit is not  used have to pass something in this case
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

                for (tablesit = tablesResults.begin(); tablesit != tablesResults.end(); ++tablesit)
                {
                    if (tablesit->id==retValue)
                    {

                        EditingRow=true;
                        NewRow=false;
                        SetTables(tablesEntryFields,tablesit,EditingRow);
                        set_current_field(TablesEntryForm,tablesEntryFields[TE_TABLE_NAME]);


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
        tablesResults.clear();
        intptrs.clear();
    }

    delwin(myLookUpMenuWin);

    return retValue;// not used yet

}
void SetTables(FIELD *tablesEntryFields[],std::vector<tables>::iterator tablesit,bool EditingRow)
{
    char NumToCharPointer[10];
    //debugMsg("inside SetFields","",0);


    if (EditingRow==true)
    {



        sprintf(NumToCharPointer, "%i", tablesit->id);
        set_field_buffer(tablesEntryFields[0], 0, NumToCharPointer);
        set_field_buffer(tablesEntryFields[TE_TABLE_ID], 0,  tablesit->tableId.c_str());
        set_field_buffer(tablesEntryFields[TE_TABLE_NAME], 0,  tablesit->tableName.c_str());
        set_field_buffer(tablesEntryFields[TE_DATABASE], 0,  tablesit->datbase.c_str());




    }
    else
    {

        for (int i=0;i<7;i++)
        {
            if (i % 2==0)
                set_field_buffer(tablesEntryFields[i], 0,  "");
        }


    }

}
int GetAllTables(std::vector<tables> &tablesResults, WINDOW *winSqlMsgArea)
{

        int rows=0;
        sqlite3 *db;
        sqlite3_stmt *stmt;
        int rc = 0;
        int col, cols;
        string sql;
        char * csql;
        char msg[100];



        sql="SELECT * FROM tables ORDER BY tableId";
        csql=sql.c_str();

        tablesResults.clear();

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
            tables table;
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
                        table.id=atoi(txt);
                        break;
                    case 1:
                        table.tableId=txt;
                        break;
                    case 2:
                        table.tableName=txt;

                        break;
                    case 3:
                        table.datbase=txt;
                        break;

                   }



                 }
                 tablesResults.push_back(table);
                 break;
               default:
                  sprintf(msg,"Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                  sqlErr(msg,sql,winSqlMsgArea);

                 break;
             }
           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         }
         // close the database file
         sqlite3_close(db);
       }

    return rows;
}



