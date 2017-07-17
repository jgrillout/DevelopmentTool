
#include "main.hpp"
static FORM *FieldsEntryForm;
static WINDOW *winFormArea;
static WINDOW *winFullScreen;
static WINDOW *winMsgArea;
using namespace std;


int fieldsEntry(WINDOW *winMsgArea, WINDOW *winFullScreen,WINDOW *winSqlMsgArea)
{

    short formCurrentField;
    bool firstTime=true,quit=false;
    int key=0, ch=0;
    bool EditingRow=false;
    bool NewRow=true;

    static FIELD *fieldsEntryFields[17];
    static FIELD*tempFptr;

fieldsEntryStart:

    quit=false;
    fieldsEntrySetupForm(fieldsEntryFields,winFullScreen);
    post_form(FieldsEntryForm);
    set_current_field(FieldsEntryForm,fieldsEntryFields[FE_FIELD_ID]);

    wattron(winFullScreen,A_REVERSE);
    mvwprintw(winFullScreen,22, 2, "F2 Lookup");
    mvwprintw(winFullScreen,22, 12, "F3 Delete");
    mvwprintw(winFullScreen,22, 22, "F4 Save");
    mvwprintw(winFullScreen,22, 32, "F5 New");
    mvwprintw(winFullScreen,22, 42, "F7 Exit");
    wattroff(winFullScreen,A_REVERSE);
    box(winFormArea, 0, 0);

    wrefresh(winFormArea);
    wrefresh(winFullScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        FieldsEntryFormCleanup(fieldsEntryFields);
        goto fieldsEntryStart;
    }


    while (quit!=true)
    {

        tempFptr = current_field(FieldsEntryForm);
        formCurrentField = field_index(tempFptr);
        ch=getch();
        switch (ch)
        {

        case KEY_F(1):
            break;
        case KEY_F(2):
            if (formCurrentField==FE_FIELD_ID)
            {
                def_prog_mode();
                endwin();

                fieldsEntryFieldsDisplayLookup(fieldsEntryFields,winSqlMsgArea,NewRow,EditingRow);

                refresh();
                pos_form_cursor(FieldsEntryForm);

            }
            break;

        case  KEY_F(3):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Delete Field",COLOR_PAIR(1),KEY_F(3))==true)
            {
                fieldsEntryDeleteField(fieldsEntryFields,winSqlMsgArea);
                pos_form_cursor(FieldsEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                goto fieldsEntryStart;
                break;
            }
            else
            {
                pos_form_cursor(FieldsEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                goto fieldsEntryStart;
                break;
            }

        case KEY_F(4):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Save",COLOR_PAIR(1),KEY_F(4))==true)
            {
                if (NewRow==true)
                {

                    fieldsEntryInsertField(fieldsEntryFields,winSqlMsgArea);
                }
                else
                {

                    fieldsEntryUpdateField(fieldsEntryFields,winSqlMsgArea);
                }
                 pos_form_cursor(FieldsEntryForm);
                 touchwin(winFullScreen);
                 wrefresh(winFullScreen);
                 goto fieldsEntryStart;
                 break;
            }
            else
            {

                pos_form_cursor(FieldsEntryForm);
                touchwin(winFullScreen);
                wrefresh(winFullScreen);
                break;
            }

        case KEY_F(5):
            goto fieldsEntryStart;
            break;

        case KEY_F(7):
            if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
            }

            pos_form_cursor(FieldsEntryForm);
            touchwin(winFullScreen);
            wrefresh(winFullScreen);
            break;

            quit=true;
            break;
        case KEY_ENTER:
        case 9:
        case 10:
        case 13:

            if (formCurrentField==6) // field LENGTH
            {

                mvwprintw(winFormArea,18, 2,"0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC,5=ENUM,6=YN,7=Date");
                wrefresh(winFormArea);

            }
            else
            {
            mvwprintw(winFormArea,18, 2,"                                                        ");
            wrefresh(winFormArea);
            }

            if(formCurrentField==FE_FIELD_ID||formCurrentField==FE_FIELD_TYPE)
            {
                //debugMsg("formCurrentField","KEY_ENTER",formCurrentField);


                //      sync buffer
                    form_driver(FieldsEntryForm, REQ_NEXT_FIELD);
                    form_driver(FieldsEntryForm, REQ_PREV_FIELD);

                if (fieldsEntryVerify(fieldsEntryFields,winFullScreen,winMsgArea,winSqlMsgArea,EditingRow,NewRow)==true)

                {



                    form_driver(FieldsEntryForm, REQ_NEXT_FIELD);
                    form_driver(FieldsEntryForm, REQ_BEG_LINE);

                }
                else
                {

                    form_driver(FieldsEntryForm, REQ_NEXT_FIELD);
                    form_driver(FieldsEntryForm, REQ_BEG_LINE);
                }
            }
            else
            {


                form_driver(FieldsEntryForm, REQ_NEXT_FIELD);
                form_driver(FieldsEntryForm, REQ_BEG_LINE);

            }

            break;


        case KEY_UP:
            if (formCurrentField==10)// field Precision
            {

                mvwprintw(winFormArea,18, 2,"0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC,5=ENUM,6=YN,7=Date");
                wrefresh(winFormArea);

            }
            else
            {
                mvwprintw(winFormArea,18, 2,"                                                        ");
                wrefresh(winFormArea);
            }

            form_driver(FieldsEntryForm, REQ_PREV_FIELD);
            form_driver(FieldsEntryForm, REQ_BEG_LINE);
            break;


        case KEY_LEFT:
            form_driver(FieldsEntryForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(FieldsEntryForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(FieldsEntryForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(FieldsEntryForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(FieldsEntryForm, REQ_END_FIELD);
            break;
        case KEY_HOME:
            form_driver(FieldsEntryForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(FieldsEntryForm, ch);
            break;
        }
        wrefresh(winFormArea);

    }


        FieldsEntryFormCleanup(fieldsEntryFields);



    return 0;

}


void  FieldsEntryFormCleanup(FIELD *fieldsEntryFields[])
{
    unpost_form(FieldsEntryForm);
    free_form(FieldsEntryForm);
    for (int i=0; i<3; i++)
    {
        free_field(fieldsEntryFields[i]);
    }
    delwin(winFormArea);

    endwin();
}

void fieldsEntrySetupForm(FIELD *fieldsEntryFields[],WINDOW *winFullScreen)

{

init_pair(1,COLOR_GREEN,COLOR_BLACK);
winFormArea = derwin(winFullScreen, 20, 78, 1, 1);
assert(winFormArea != NULL);
wattron(winFormArea,COLOR_PAIR(1));
 wrefresh(winFormArea);




//              new_field(h,w,  tp,left, offscreen, nbuffers);
fieldsEntryFields[0] = new_field(1,  5, 0, 0,0,0);// hidden  key id
fieldsEntryFields[1] = new_field(1,  8, 2, 1,0,0);// Label Field Id
fieldsEntryFields[FE_FIELD_ID] = new_field(1, 15, 2,15,0,0);// field Field Id
fieldsEntryFields[3] = new_field(1,  4, 4, 1,0,0);// Label Name
fieldsEntryFields[FE_FIELD_NAME] = new_field(1, 30, 4,15,0,0);// field Name
fieldsEntryFields[5] = new_field(1,  6, 6, 1,0,0);// Label Length
fieldsEntryFields[FE_FIELD_LEN] = new_field(1,  3, 6,15,0,0);// field Length
fieldsEntryFields[7] = new_field(1,  4, 8, 1,0,0);// Label Type
fieldsEntryFields[FE_FIELD_TYPE] = new_field(1,  2, 8,15,0,0);// field Type
fieldsEntryFields[9] = new_field(1,  9,10, 1,0,0);// Label FieldPrecision
fieldsEntryFields[FE_FIELD_PRE] = new_field(1,  2,10,15,0,0);// field FieldPrecision
fieldsEntryFields[11] = new_field(1, 12,12,1,0,0);// Label Primary Key?
fieldsEntryFields[FE_FIELD_PRIMKEY] = new_field(1, 3, 12,15,0,0);// field Primary Key?
fieldsEntryFields[13] = new_field(1, 9, 14, 1,0,0);// Label Required?
fieldsEntryFields[FE_FIELD_REQ] = new_field(1, 3, 14,15,0,0);// field Required?
fieldsEntryFields[15] = new_field(1, 11, 16,1,0,0);// field Enum Values
fieldsEntryFields[FE_FIELD_ENUMS] = new_field(1, 40, 16,15,0,0);// field Enum Values
fieldsEntryFields[17] = NULL;
 for (int i=0;i<17;i++)
 {
	assert(fieldsEntryFields[i] !=NULL);

 }
 set_field_buffer(fieldsEntryFields[0] ,0,"");//hidden key id

set_field_buffer(fieldsEntryFields[1] ,0,"Field Id");//Label Field Id
set_field_buffer(fieldsEntryFields[FE_FIELD_ID] ,0,"");// field FIELDID
set_field_buffer(fieldsEntryFields[3] ,0,"Name");//Label Name
set_field_buffer(fieldsEntryFields[FE_FIELD_NAME] ,0,"");// field FIELDNAME
set_field_buffer(fieldsEntryFields[5] ,0,"Length");//Label Length
set_field_buffer(fieldsEntryFields[FE_FIELD_LEN] ,0,"");// field LENGTH
set_field_buffer(fieldsEntryFields[7] ,0,"Type");//Label Type
set_field_buffer(fieldsEntryFields[FE_FIELD_TYPE] ,0,"");// field TYPE
set_field_buffer(fieldsEntryFields[9] ,0,"Precision");//Label Precision
set_field_buffer(fieldsEntryFields[FE_FIELD_PRE] ,0,"");// field PREC
set_field_buffer(fieldsEntryFields[11] ,0,"Primary Key?");//Label Primary Key?
set_field_buffer(fieldsEntryFields[FE_FIELD_PRIMKEY] ,0,"");// field KEY
set_field_buffer(fieldsEntryFields[13] ,0,"Required?");//Label Required?
set_field_buffer(fieldsEntryFields[FE_FIELD_REQ] ,0,"");// field Required?
set_field_buffer(fieldsEntryFields[15] ,0,"Enum Values");//Label Enum Values
set_field_buffer(fieldsEntryFields[FE_FIELD_ENUMS] ,0,"");// field Enum Values

set_field_type(fieldsEntryFields[FE_FIELD_LEN] ,TYPE_NUMERIC,0,99);
set_field_type(fieldsEntryFields[FE_FIELD_TYPE] ,TYPE_NUMERIC,0,99);
set_field_type(fieldsEntryFields[FE_FIELD_PRE] ,TYPE_NUMERIC,0,999);

char * yesno[] = { "Yes", "No", (char *)0 };

set_field_type(fieldsEntryFields[FE_FIELD_PRIMKEY],TYPE_ENUM, yesno, false,false);

set_field_type(fieldsEntryFields[FE_FIELD_REQ],TYPE_ENUM, yesno, false,false);

for (int i=0;i<=17;i++)
{
	 if (i % 2 ==0)
		 set_field_back(fieldsEntryFields[i], A_UNDERLINE|COLOR_PAIR(1));
	 else
		 set_field_fore(fieldsEntryFields[i],COLOR_PAIR(1));
}
set_field_opts(fieldsEntryFields[0] ,  O_PUBLIC | O_AUTOSKIP);

set_field_opts(fieldsEntryFields[1] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_ID] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(fieldsEntryFields[3] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_NAME] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);
set_field_opts(fieldsEntryFields[5] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_LEN] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);
set_field_opts(fieldsEntryFields[7] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_TYPE] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(fieldsEntryFields[9] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_PRE] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(fieldsEntryFields[11] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_PRIMKEY] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(fieldsEntryFields[13] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_REQ] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
set_field_opts(fieldsEntryFields[15] ,  O_VISIBLE |O_PUBLIC | O_AUTOSKIP);
set_field_opts(fieldsEntryFields[FE_FIELD_ENUMS] , O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
FieldsEntryForm = new_form(fieldsEntryFields);
assert(FieldsEntryForm != NULL);
set_form_win(FieldsEntryForm,winFullScreen);
set_form_sub(FieldsEntryForm,winFormArea);
move(3,16);
}
int fieldsEntryInsertField(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];

    fields row;

    row.idFieldsid = atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[0], 0)));
    row.fieldId = TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_ID], 0));
    row.Name = TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_NAME], 0));
    row.Length = atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_LEN], 0)));
    row.Type = atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_TYPE], 0)));
    row.FieldPrecision = atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_PRE], 0)));
    row.isPrimaryKey = TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_PRIMKEY], 0));
    row.isRequired = TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_REQ], 0));
    row.EmumValues = TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_ENUMS], 0));


    sql = "INSERT INTO fields (fieldId,Name,Type,Length,FieldPrecision,isPrimaryKey,isRequired,EmumValues)";
    sql.append(" VALUES ( \"");
    sql.append (row.fieldId);
    sql.append("\""",\"");
    sql.append (row.Name);
    sql.append("\""",\"");
    sprintf(NumToCharPointer, "%d", row.Type);
    sql.append (NumToCharPointer);
    sql.append("\""",\"");
    sprintf(NumToCharPointer, "%d", row.Length);
    sql.append (NumToCharPointer);
    sql.append("\""",\"");
    sprintf(NumToCharPointer, "%d", row.FieldPrecision);
    sql.append (NumToCharPointer);
    sql.append("\""",\"");
    sql.append (row.isPrimaryKey);
    sql.append("\""",\"");
    sql.append (row.isRequired);
    sql.append("\""",\"");
    sql.append (row.EmumValues);
    sql.append("\""")");
    //debugMsg("sql",sql.c_str(),0);

    bool result=true;
    result=execSQLAction(sql,winSqlMsgArea);



    return 0;
}

int fieldsEntryUpdateField(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];


    int key;


    key=atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[0], 0)));

    sql = "UPDATE fields SET ";
    sql.append ("fieldId= \"");
    sql.append (TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_ID], 0)));
    sql.append("\""" , Name= \"");
    sql.append(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_NAME], 0)));
    sprintf(NumToCharPointer, "%d", atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_TYPE], 0))));
    sql.append("\""" , Type= \"");
    sql.append(NumToCharPointer);
    sprintf(NumToCharPointer, "%d", atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_LEN], 0))));
    sql.append("\""" , Length= \"");
    sql.append(NumToCharPointer);
    sprintf(NumToCharPointer, "%d", atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_PRE], 0))));
    sql.append("\""" , FieldPrecision= \"");
    sql.append(NumToCharPointer);
    sql.append("\""" , isPrimaryKey= \"");
    sql.append(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_PRIMKEY], 0)));
    sql.append("\""" , isRequired= \"");
    sql.append(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_REQ], 0)));
    sql.append("\""" , EmumValues= \"");
    sql.append(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_ENUMS], 0)));

    sql.append("\""" WHERE idFieldsid = ");
    sprintf(NumToCharPointer, "%d", key);
    sql.append(NumToCharPointer);
    execSQLAction(sql,winSqlMsgArea);


    return 0;


}

int fieldsEntryDeleteField(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea)
{
    string sql;
    int key;
    key=atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[0], 0)));
    char NumToCharPointer[10];

    sprintf(NumToCharPointer, "%d", key);

    sql = "DELETE FROM fields WHERE idFieldsid= ";
    sql.append(NumToCharPointer);

    execSQLAction(sql,winSqlMsgArea);


    return 0;
}



bool  fieldsEntryVerify(FIELD *fieldsEntryFields[],WINDOW *winFullScreen, WINDOW *winMsgArea,WINDOW *winSqlMsgArea,bool &EditingRow, bool &NewRow)
{




    char NumToCharPointer[10];
    short formCurrentField;
    bool Result=false;
    static FIELD*tempFptr;
    std::string lookup;
    std::vector<fields>::iterator fieldsit;
    std::vector<fields> FieldsResults;

    tempFptr = current_field(FieldsEntryForm);
    formCurrentField = field_index(tempFptr);


    switch (formCurrentField)
    {


    case FE_FIELD_ID: // Field  Id
        lookup=TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_ID], 0));
        FieldsResults=GetField(lookup,winSqlMsgArea);

        if (FieldsResults.empty()==false)
        {

            EditingRow=true;
            NewRow=false;
            Result=true;
            fieldsit=FieldsResults.begin();
            fieldsEntrySetFields(fieldsEntryFields,fieldsit,EditingRow);


        }
        else
        {

            EditingRow=false;
            NewRow=true;
            Result=false;
            set_current_field(FieldsEntryForm,fieldsEntryFields[FE_FIELD_ID]);
        }

        touchwin(winFullScreen);
        wrefresh(winFormArea);
        wrefresh(winFullScreen);
        pos_form_cursor(FieldsEntryForm);
        refresh();
        break;
    case FE_FIELD_TYPE:

            if (atoi(TrimWhitespaces(field_buffer(fieldsEntryFields[FE_FIELD_TYPE], 0)))==6)// type = YN
            {

                set_field_buffer(fieldsEntryFields[FE_FIELD_ENUMS],0,"Yes,No");
                touchwin(winFullScreen);
                wrefresh(winFormArea);
                wrefresh(winFullScreen);
                pos_form_cursor(FieldsEntryForm);
                refresh();
            }

            break;

    default:
        break;
    }

    return Result;
}

int fieldsEntryFieldsDisplayLookup(FIELD *fieldsEntryFields[],WINDOW *winSqlMsgArea,bool &NewRow,bool &EditingRow)
{

    ITEM **myItems;
    int c,r;

    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;
    std::vector<fields> FieldsResults;
    std::vector<fields>::iterator Fieldsit;
    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;

    static FIELD*tempFptr;



    short formCurrentField;
    tempFptr = current_field(FieldsEntryForm);
    formCurrentField = field_index(tempFptr);
    EditingRow= false;
    NewRow= false;


    NumChoices=GetAllFields(FieldsResults,winSqlMsgArea);

    NumChoices=FieldsResults.size();


    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (Fieldsit = FieldsResults.begin(); Fieldsit != FieldsResults.end(); ++Fieldsit)
    {


        myItems[r] = new_item(Fieldsit->fieldId.c_str(),Fieldsit->Name.c_str());
        intptrs.push_back(&Fieldsit->idFieldsid);
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
                fieldsEntrySetFields(fieldsEntryFields,Fieldsit,EditingRow); // Fieldsit is not  used have to pass something in this case
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

                for (Fieldsit = FieldsResults.begin(); Fieldsit != FieldsResults.end(); ++Fieldsit)
                {
                    if (Fieldsit->idFieldsid==retValue)
                    {

                        EditingRow=true;
                        NewRow=false;
                        fieldsEntrySetFields(fieldsEntryFields,Fieldsit,EditingRow);
                        set_current_field(FieldsEntryForm,fieldsEntryFields[FE_FIELD_NAME]);


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
        FieldsResults.clear();
        intptrs.clear();
    }



    return retValue;// not used yet

}
void fieldsEntrySetFields(FIELD *fieldsEntryFields[],std::vector<fields>::iterator Fieldsit,bool EditingRow)
{
    char NumToCharPointer[10];

    if (EditingRow==true)
    {



        sprintf(NumToCharPointer, "%i", Fieldsit->idFieldsid);
        set_field_buffer(fieldsEntryFields[0], 0, NumToCharPointer);
        set_field_buffer(fieldsEntryFields[FE_FIELD_ID], 0,  Fieldsit->fieldId.c_str());
        set_field_buffer(fieldsEntryFields[FE_FIELD_NAME], 0,  Fieldsit->Name.c_str());
        sprintf(NumToCharPointer, "%i", Fieldsit->Length);
        set_field_buffer(fieldsEntryFields[FE_FIELD_LEN], 0,  NumToCharPointer);
        sprintf(NumToCharPointer, "%i", Fieldsit->Type);
        set_field_buffer(fieldsEntryFields[FE_FIELD_TYPE], 0,  NumToCharPointer);
        sprintf(NumToCharPointer, "%i", Fieldsit->FieldPrecision);
        set_field_buffer(fieldsEntryFields[FE_FIELD_PRE], 0,  NumToCharPointer);
        set_field_buffer(fieldsEntryFields[FE_FIELD_PRIMKEY], 0,  Fieldsit->isPrimaryKey.c_str());
        set_field_buffer(fieldsEntryFields[FE_FIELD_REQ], 0,  Fieldsit->isRequired.c_str());
        set_field_buffer(fieldsEntryFields[FE_FIELD_ENUMS], 0,  Fieldsit->EmumValues.c_str());



    }
    else
    {

        for (int i=0;i<17;i++)
        {
            if (i % 2==0)
                set_field_buffer(fieldsEntryFields[i], 0,  "");
        }


    }

}



