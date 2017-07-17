
#include "main.hpp"
#include "vector"
static FORM *dataDictionaryEntryForm;
static WINDOW *winFormArea;
static WINDOW *winFullScreen;
static WINDOW *winMsgArea;
using namespace std;


int dataDictionaryEntry(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea)
{

    short formCurrentField;
    bool firstTime=true,quit=false;
    int key=0, ch=0;
    bool EditingRow=false;
    bool NewRow=true;

    static FIELD *dataDictionaryEntryFields[31];
    static FIELD*tempFptr;
    vector<string>  dataDictionaryEntryFieldsId;

    string value="";
    std::vector<fields> fieldsResults;
    std::vector<fields>::iterator fieldsit;



dataDictionaryEntryStart:

    quit=false;

    dataDictionaryEntrySetupForm(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winFullScreen);
    post_form(dataDictionaryEntryForm);
    set_current_field(dataDictionaryEntryForm,dataDictionaryEntryFields[DD_TABLE_ID]);

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
        dataDictionaryFieldsEntryFormCleanup(dataDictionaryEntryFields);
        goto dataDictionaryEntryStart;
    }


    while (quit!=true)
    {

        tempFptr = current_field(dataDictionaryEntryForm);

        formCurrentField = field_index(tempFptr);

        ch=getch();
        switch (ch)
        {

        case KEY_F(1):
            break;
        case KEY_F(2):
            if (formCurrentField==DD_TABLE_ID)
            {

                def_prog_mode();
                endwin();
                dataDictionaryEntryDisplayTablesLookup(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winSqlMsgArea,NewRow,EditingRow);
                touchwin(winFormArea);
                refresh();

                pos_form_cursor(dataDictionaryEntryForm);
                break;
            }
            else if (formCurrentField>3)
            {

                def_prog_mode();
                endwin();
                dataDictionaryEntryDisplayFieldsLookup(dataDictionaryEntryFields,winSqlMsgArea,NewRow,EditingRow);
                refresh();
                pos_form_cursor(dataDictionaryEntryForm);
                break;
            }
            break;

        case  KEY_F(3):
             if(formCurrentField==DD_TABLE_ID)
            {
                if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Delete All Fields",COLOR_PAIR(1),KEY_F(3))==true)
                {

                    DeleteAllDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winSqlMsgArea);

                    pos_form_cursor(dataDictionaryEntryForm);
                    touchwin(winFullScreen);
                    wrefresh(winFullScreen);
                    goto dataDictionaryEntryStart;
                    break;
                }
                else
                {
                    pos_form_cursor(dataDictionaryEntryForm);
                    touchwin(winFullScreen);
                    wrefresh(winFullScreen);

                    break;
                }
            }
            else if (formCurrentField>3)
            {
                if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Delete This Field",COLOR_PAIR(1),KEY_F(3))==true)
                {


                    DeleteThisDictionaryField(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,formCurrentField,winSqlMsgArea);

                    pos_form_cursor(dataDictionaryEntryForm);
                    touchwin(winFullScreen);
                    wrefresh(winFullScreen);
                    break;
                }
                else
                {
                    pos_form_cursor(dataDictionaryEntryForm);
                    touchwin(winFullScreen);
                    wrefresh(winFullScreen);
                    break;
                }
            }
            else
            {
                pos_form_cursor(dataDictionaryEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                break;
            }



        case KEY_F(4):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Save",COLOR_PAIR(1),KEY_F(4))==true)
            {

                DeleteDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winSqlMsgArea);
                InsertDictionaryFields(dataDictionaryEntryFields,winSqlMsgArea);
                 pos_form_cursor(dataDictionaryEntryForm);
                 touchwin(winFullScreen);
                 wrefresh(winFullScreen);
                 goto dataDictionaryEntryStart;
                 break;
            }
            else
            {

                pos_form_cursor(dataDictionaryEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                break;
            }

        case KEY_F(5):
            if (formCurrentField>3)
            {

                FieldInsert(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,formCurrentField,winSqlMsgArea);
                break;
            }
            else
            {
                goto dataDictionaryEntryStart;
                break;
            }

        case KEY_F(7):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
            }

            pos_form_cursor(dataDictionaryEntryForm);
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;

        case KEY_ENTER:
        case 9:
        case 10:
        case 13:

            //      sync buffer
            form_driver(dataDictionaryEntryForm, REQ_NEXT_FIELD);
            form_driver(dataDictionaryEntryForm, REQ_PREV_FIELD);
            if(formCurrentField==DD_TABLE_ID)
            {

//                    //      sync buffer
//                    form_driver(dataDictionaryEntryForm, REQ_NEXT_FIELD);
//                    form_driver(dataDictionaryEntryForm, REQ_PREV_FIELD);

                if (dataDictionaryEntryVerify(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winFullScreen,winMsgArea,winSqlMsgArea,EditingRow,NewRow)==true)

                {


                    form_driver(dataDictionaryEntryForm, REQ_NEXT_FIELD);
                    form_driver(dataDictionaryEntryForm, REQ_BEG_LINE);

                }
                else
                {

                    form_driver(dataDictionaryEntryForm, REQ_NEXT_FIELD);
                    form_driver(dataDictionaryEntryForm, REQ_BEG_LINE);
                }
            }
            else if(formCurrentField>=4 && (formCurrentField %2)==0)
            {

                 saveFieldInVector(dataDictionaryEntryFields, dataDictionaryEntryFieldsId,formCurrentField);
                 value=TrimWhitespaces(field_buffer(dataDictionaryEntryFields[formCurrentField], 0));

                 if (value>"")
                 {

                    fieldsResults=GetField(value,winSqlMsgArea);
                    fieldsit= fieldsResults.begin();
                    set_field_buffer(dataDictionaryEntryFields[formCurrentField+1], 0,  fieldsit->Name.c_str());
                 }
                 form_driver(dataDictionaryEntryForm, REQ_NEXT_FIELD);
                 form_driver(dataDictionaryEntryForm, REQ_BEG_LINE);
            }

            else if(formCurrentField<4)
            {


                form_driver(dataDictionaryEntryForm, REQ_NEXT_FIELD);
                form_driver(dataDictionaryEntryForm, REQ_BEG_LINE);

            }

            break;


        case KEY_UP:

            form_driver(dataDictionaryEntryForm, REQ_PREV_FIELD);
            form_driver(dataDictionaryEntryForm, REQ_BEG_LINE);
            break;


        case KEY_LEFT:
            form_driver(dataDictionaryEntryForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(dataDictionaryEntryForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(dataDictionaryEntryForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(dataDictionaryEntryForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(dataDictionaryEntryForm, REQ_END_FIELD);
            break;
        case KEY_HOME:
            form_driver(dataDictionaryEntryForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(dataDictionaryEntryForm, ch);
            break;
        }
        wrefresh(winFormArea);

    }


        dataDictionaryFieldsEntryFormCleanup(dataDictionaryEntryFields);



    return 0;

}


void  dataDictionaryFieldsEntryFormCleanup(FIELD *dataDictionaryEntryFields[])
{
    unpost_form(dataDictionaryEntryForm);
    free_form(dataDictionaryEntryForm);
    for (int i=0; i<30; i++)
    {

        free_field(dataDictionaryEntryFields[i]);
    }
    delwin(winFormArea);

    endwin();
}

void dataDictionaryEntrySetupForm(FIELD *dataDictionaryEntryFields[],
vector<string> &dataDictionaryEntryFieldsId,WINDOW *winFullScreen)
{

    dataDictionaryEntryFieldsId.clear();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    winFormArea = derwin(winFullScreen, 20, 78, 1, 1);
    assert(winFormArea != NULL);
    wattron(winFormArea,COLOR_PAIR(1));
    wrefresh(winFormArea);




//              new_field(h,w,  tp,left, offscreen, nbuffers);
dataDictionaryEntryFields[0] = new_field(1,  5, 0, 0,0,0);// hidden  key id
dataDictionaryEntryFields[1] = new_field(1,  8, 1, 1,0,0);// Label Table Id
dataDictionaryEntryFields[DD_TABLE_ID] = new_field(1, 20, 1,15,0,0);// field Table Id
dataDictionaryEntryFields[3] = new_field(1,  55, 3, 1,0,0);// Label fields
int r=4; //field row

for (int i=4;i<30;i++)//46
 {

    if (i % 2 ==0)
        dataDictionaryEntryFields[i] = new_field(1, 20, r,1,0,0);
    else
    {
        dataDictionaryEntryFields[i] = new_field(1, 30, r,26,0,0);
        r++;
    }
 }

dataDictionaryEntryFields[31] = NULL;
 for (int i=0;i<30;i++)
 {
	assert(dataDictionaryEntryFields[i] !=NULL);

 }
set_field_buffer(dataDictionaryEntryFields[0] ,0,"");//hidden key id

set_field_buffer(dataDictionaryEntryFields[1] ,0,"Table Id");//Label Field Id
set_field_buffer(dataDictionaryEntryFields[DD_TABLE_ID] ,0,"");// field FIELDID
set_field_buffer(dataDictionaryEntryFields[3] ,0,"************************fields*************************");//Label table id

set_field_opts(dataDictionaryEntryFields[0] , O_PUBLIC | O_AUTOSKIP);
set_field_opts(dataDictionaryEntryFields[1] , O_VISIBLE| O_PUBLIC | O_AUTOSKIP);
set_field_opts(dataDictionaryEntryFields[2] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_fore(dataDictionaryEntryFields[2],COLOR_PAIR(1));
set_field_opts(dataDictionaryEntryFields[3] , O_VISIBLE| O_PUBLIC | O_AUTOSKIP);

set_field_back(dataDictionaryEntryFields[1], COLOR_PAIR(1));

set_field_back(dataDictionaryEntryFields[3], COLOR_PAIR(1));

set_field_type(dataDictionaryEntryFields[2] ,TYPE_ALNUM,0);
set_field_back(dataDictionaryEntryFields[2], A_UNDERLINE|COLOR_PAIR(1));
set_field_opts(dataDictionaryEntryFields[2] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);



for (int i=4;i<30;i++)//46
{

	 if (i % 2 ==0)
	 {

         set_field_type(dataDictionaryEntryFields[i] ,TYPE_ALNUM,0);
		 set_field_back(dataDictionaryEntryFields[i], A_UNDERLINE|COLOR_PAIR(1));
		 set_field_opts(dataDictionaryEntryFields[i] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
     }
	 else
	 {

		 set_field_fore(dataDictionaryEntryFields[i],COLOR_PAIR(1));
		 set_field_opts(dataDictionaryEntryFields[i] , O_VISIBLE| O_PUBLIC | O_AUTOSKIP);

    }
}


dataDictionaryEntryForm = new_form(dataDictionaryEntryFields);
assert(dataDictionaryEntryForm != NULL);
set_form_win(dataDictionaryEntryForm,winFullScreen);
set_form_sub(dataDictionaryEntryForm,winFormArea);
move(2,16);

}
int InsertDictionaryFields(FIELD *dataDictionaryEntryFields[],WINDOW *winSqlMsgArea)
{
    char NumToCharPointer[10];
    string sql;

    std::vector<tables>::iterator tablesit;
    std::vector<tables> tablesResults;

    std::vector<fields> fieldsResults;
    std::vector<fields>::iterator fieldsit;



    dataDictionary row;

    row.tableId = TrimWhitespaces(field_buffer(dataDictionaryEntryFields[DD_TABLE_ID], 0));
    tablesResults=GetTable(row.tableId,winSqlMsgArea);

    tablesit=tablesResults.begin();
    row.idTablesid= tablesit->id;

     for (int i=4;i<30;i++)
        {

            if (i % 2 ==0)
            {


                row.fieldId = TrimWhitespaces(field_buffer(dataDictionaryEntryFields[i], 0));
                if (row.fieldId >"")
                {
                    fieldsResults=GetField(row.fieldId,winSqlMsgArea);
                    fieldsit= fieldsResults.begin();

                    row.idFieldsid = fieldsit->idFieldsid;

                    sql = "INSERT INTO dataDictionary (tableId,fieldId,idFieldsid,idTablesid)";
                    sql.append(" VALUES ( \"");
                    sql.append (row.tableId);
                    sql.append("\""",\"");
                    sql.append (row.fieldId);
                    sql.append("\""",\"");
                    sprintf(NumToCharPointer, "%d", row.idFieldsid);
                    sql.append (NumToCharPointer);
                    sql.append("\""",\"");
                    sprintf(NumToCharPointer, "%d", row.idTablesid);
                    sql.append (NumToCharPointer);
                    sql.append("\""")");
                    //debugMsg("sql",sql.c_str(),0);
                    bool result=true;
                    result=execSQLAction(sql,winSqlMsgArea);


                }
            }
        }
    return 0;
}



int DeleteDictionaryFields(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea)
{



    string sql,tableId;

    tableId=TrimWhitespaces(field_buffer(dataDictionaryEntryFields[DD_TABLE_ID], 0));



    sql = "DELETE FROM dataDictionary WHERE tableId= ";
    sql.append("\"");
    sql.append(tableId);
    sql.append("\"");
    execSQLAction(sql,winSqlMsgArea);


    return 0;
}
int DeleteAllDictionaryFields(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea)
{


    DeleteDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winSqlMsgArea);

    CleardataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId);


    return 0;
}



bool  dataDictionaryEntryVerify(FIELD *dataDictionaryEntryFields[],
vector<string> &dataDictionaryEntryFieldsId,WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow)
{


    std::vector<dataDictionary>::iterator dataDictionaryit;
    std::vector<dataDictionary> dataDictionaryResults;

    char NumToCharPointer[10];
    short formCurrentField;
    bool Result=false;
    static FIELD*tempFptr;
    std::string lookup,tableId;


    tempFptr = current_field(dataDictionaryEntryForm);
    formCurrentField = field_index(tempFptr);

    switch (formCurrentField)
    {


    case DD_TABLE_ID:
        lookup=TrimWhitespaces(field_buffer(dataDictionaryEntryFields[DD_TABLE_ID], 0));
        dataDictionaryResults=GetDictionaryFields(lookup,winSqlMsgArea);

        if (dataDictionaryResults.empty()==false)
        {

            EditingRow=true;
            NewRow=false;
            Result=true;

            dataDictionaryit=dataDictionaryResults.begin();
            tableId=dataDictionaryit->tableId;
            SetdataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,tableId,winSqlMsgArea);


        }
        else
        {


            CleardataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId);

            set_current_field(dataDictionaryEntryForm,dataDictionaryEntryFields[DD_TABLE_ID]);
        }

        touchwin(winFullScreen);
        wrefresh(winFormArea);
        wrefresh(winFullScreen);
        pos_form_cursor(dataDictionaryEntryForm);
        refresh();
        break;

    default:
        break;
    }

    return Result;
}

int dataDictionaryEntryDisplayTablesLookup(FIELD *dataDictionaryEntryFields[],
vector<string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow)
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
    tempFptr = current_field(dataDictionaryEntryForm);
    formCurrentField = field_index(tempFptr);
    EditingRow= false;
    NewRow= false;
    string tableId;

    NumChoices=GetAllTables(tablesResults,winSqlMsgArea);



    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (tablesit = tablesResults.begin(); tablesit != tablesResults.end(); ++tablesit)
    {

        myItems[r] = new_item(tablesit->tableId.c_str(),"");

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
                CleardataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId);
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

                        tableId=tablesit->tableId;

                        sprintf(NumToCharPointer,"%d",tablesit->id);
                        set_field_buffer(dataDictionaryEntryFields[0], 0,NumToCharPointer);
                        set_field_buffer(dataDictionaryEntryFields[DD_TABLE_ID], 0,tableId.c_str());

                        set_current_field(dataDictionaryEntryForm,dataDictionaryEntryFields[DD_TABLE_ID]);
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



    return retValue;// not used yet

}
int dataDictionaryEntryDisplayFieldsLookup(FIELD *dataDictionaryEntryFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow)
{


    ITEM **myItems;
    int c,r;

    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;
    std::vector<fields> fieldsResults;
    std::vector<fields>::iterator fieldsit;
    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;

    static FIELD*tempFptr;



    short formCurrentField;
    tempFptr = current_field(dataDictionaryEntryForm);
    formCurrentField = field_index(tempFptr);
    EditingRow= false;
    NewRow= false;


    NumChoices=GetAllFields(fieldsResults,winSqlMsgArea);



    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (fieldsit = fieldsResults.begin(); fieldsit != fieldsResults.end(); ++fieldsit)
    {

        myItems[r] = new_item(fieldsit->fieldId.c_str(),fieldsit->Name.c_str());

        intptrs.push_back(&fieldsit->idFieldsid);

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
                break;
            case KEY_ENTER:
            case 10:
            case 13:

                ITEM *cur;
                cur = current_item(myLookUpMenu);
                exitMenu=true;
                retValue = (int)item_userptr(cur);
                EditingRow=false;
                NewRow=true;

                for (fieldsit = fieldsResults.begin(); fieldsit != fieldsResults.end(); ++fieldsit)
                {
                    if (fieldsit->idFieldsid==retValue)
                    {

                        EditingRow=true;
                        NewRow=false;
                        set_field_buffer(dataDictionaryEntryFields[formCurrentField],0,TrimWhitespaces(fieldsit->fieldId.c_str()));
                        set_field_buffer(dataDictionaryEntryFields[formCurrentField+1],0,TrimWhitespaces(fieldsit->Name.c_str()));
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



//
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
        fieldsResults.clear();
        intptrs.clear();
    }



    return retValue;// not used yet

}
void SetdataDictionaryFields(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId,string tableId,WINDOW *winSqlMsgArea)
{

    int f=0;
    char NumToCharPointer[10];
    string fieldId="";

    std::vector<fields> fieldsResults;
    std::vector<fields>::iterator fieldsit;


    std::vector<tables> tablesResults;
    std::vector<tables>::iterator tablesit;


    std::vector<dataDictionary> dataDictionaryResults;
    std::vector<dataDictionary>::iterator dataDictionaryit;

    tablesResults=GetTable(tableId,winSqlMsgArea);
    tablesit=tablesResults.begin();

    dataDictionaryResults=GetDictionaryFields(tableId,winSqlMsgArea);


    CleardataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId);

    sprintf(NumToCharPointer,"%d",tablesit->id);
    set_field_buffer(dataDictionaryEntryFields[0], 0,NumToCharPointer);
    set_field_buffer(dataDictionaryEntryFields[DD_TABLE_ID], 0,tableId.c_str());



     f=3;
     for (dataDictionaryit=dataDictionaryResults.begin();dataDictionaryit!=dataDictionaryResults.end();++dataDictionaryit)
    {
            f++;
            fieldId=dataDictionaryit->fieldId;
            fieldsResults=GetField(fieldId,winSqlMsgArea);
            fieldsit= fieldsResults.begin();
            set_field_buffer(dataDictionaryEntryFields[f], 0,  fieldsit->fieldId.c_str());
            dataDictionaryEntryFieldsId.push_back(fieldsit->fieldId.c_str());
            f++;
            set_field_buffer(dataDictionaryEntryFields[f], 0,  fieldsit->Name.c_str());

    }

}

void SetdataDictionaryFieldsFromVector(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId,WINDOW *winSqlMsgArea)
{

// only called by FieldInsert
    if (dataDictionaryEntryFieldsId.size()==0)
        return;

    int f=0;
    char NumToCharPointer[10];
    string fieldId="",test="";

    std::vector<fields> fieldsResults;
    std::vector<fields>::iterator fieldsit;
    vector<string>::iterator it;

    for (int i=4;i<30;i++)
    {

        set_field_buffer(dataDictionaryEntryFields[i], 0,  "");
    }

    f=2;

     for(it=dataDictionaryEntryFieldsId.begin() ; it < dataDictionaryEntryFieldsId.end(); it++)
    {
        f+=2;
        if (f>28)
        break;
                fieldId=*it;

                if (fieldId.compare(test)!=0)
                {

                    fieldsResults=GetField(fieldId,winSqlMsgArea);
                    fieldsit= fieldsResults.begin();
                    set_field_buffer(dataDictionaryEntryFields[f], 0,  fieldsit->fieldId.c_str());
                    set_field_buffer(dataDictionaryEntryFields[f+1], 0,  fieldsit->Name.c_str());
                }
    }


}

void CleardataDictionaryFields(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId)
{

        dataDictionaryEntryFieldsId.clear();
        for (int i=4;i<30;i++)
        {

            set_field_buffer(dataDictionaryEntryFields[i], 0,  "");
        }


}
void DeleteThisDictionaryField(FIELD *dataDictionaryEntryFields[],
vector<string> &dataDictionaryEntryFieldsId,int formCurrentField,WINDOW *winSqlMsgArea)
{

    string sql,tableId,fieldId;
    std::vector<dataDictionary>::iterator dataDictionaryit;
    std::vector<dataDictionary> dataDictionaryResults;

    tableId=TrimWhitespaces(field_buffer(dataDictionaryEntryFields[DD_TABLE_ID], 0));
    fieldId=TrimWhitespaces(field_buffer(dataDictionaryEntryFields[formCurrentField], 0));


    sql = "DELETE FROM dataDictionary WHERE tableId= ";
    sql.append("\"");
    sql.append(tableId);
    sql.append("\"");
    sql.append(" AND  fieldId= ");
    sql.append("\"");
    sql.append(fieldId);
    sql.append("\"");
    execSQLAction(sql,winSqlMsgArea);
    CleardataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId);

    SetdataDictionaryFields(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,tableId,winSqlMsgArea);

}
void saveFieldInVector(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId,int formCurrentField)
{

    vector< string >::iterator it;
    int CurrentField[] {4,6,8,10,12,14,16,18,20,22,24,26,28};
    string value;
    value=TrimWhitespaces(field_buffer(dataDictionaryEntryFields[formCurrentField], 0));

    for (int i=0;i<13;i++)
    {
        if (formCurrentField==CurrentField[i])
        {

            if (i<dataDictionaryEntryFieldsId.size())
            {

                dataDictionaryEntryFieldsId.at(i)=value;
            }
            else
            {

                it = dataDictionaryEntryFieldsId.begin()+i;
                dataDictionaryEntryFieldsId.insert(it,value);
            }

            break;
        }
    }

}
void FieldInsert(FIELD *dataDictionaryEntryFields[],vector<string> &dataDictionaryEntryFieldsId,int formCurrentField,WINDOW *winSqlMsgArea)
{

    vector< string >::iterator it;
    int CurrentField[] {4,6,8,10,12,14,16,18,20,22,24,26,28};

    for (int i=0;i<13;i++)
    {
        if (formCurrentField==CurrentField[i])
        {


            it = dataDictionaryEntryFieldsId.begin()+i;
            dataDictionaryEntryFieldsId.insert(it,"");

            break;
        }
    }

    SetdataDictionaryFieldsFromVector(dataDictionaryEntryFields,dataDictionaryEntryFieldsId,winSqlMsgArea);
    set_field_buffer(dataDictionaryEntryFields[formCurrentField], 0, "");
    set_field_buffer(dataDictionaryEntryFields[formCurrentField+1], 0, "");
}






