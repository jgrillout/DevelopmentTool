#ifndef MAIN_HPP_INCLUDED
#include "main.hpp"
#endif
static FORM *screenFieldForm;
static WINDOW *winFormArea;
using namespace std;


int screenField(WINDOW *winFullScreen,WINDOW *winSqlMsgArea,WINDOW *winMsgArea,string tableId,termScreenFields sf[], int &numsf,string ScreenLayoutId,bool screenYXlocked[][79], bool &deleted, bool &saved,bool NewField)
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



    static FIELD *screenEntryFields[34];
    static FIELD*tempFptr;
    char NumToCharPointer[10];


screenFieldStart:

    quit=false;
    screenFieldSetupForm(screenEntryFields,winFieldScreen);

    post_form(screenFieldForm);


    set_field_buffer(screenEntryFields[SF_SCREEN_LOC_ID],0,sf[numsf].ScreenLocId.c_str());
    set_field_buffer(screenEntryFields[SF_FIELD_ID],0,sf[numsf].fieldId.c_str());

    set_field_buffer(screenEntryFields[SF_FIELD_NAME],0,sf[numsf].fieldName.c_str());

    sprintf(NumToCharPointer, "%d", sf[numsf].fieldType);
    set_field_buffer(screenEntryFields[SF_LABEL_TEXT],0,sf[numsf].labelText.c_str());
    sprintf(NumToCharPointer, "%d", sf[numsf].labelRow);
    set_field_buffer(screenEntryFields[SF_LABEL_ROW],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", sf[numsf].labelCol);
    set_field_buffer(screenEntryFields[SF_LABEL_COL],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", sf[numsf].fieldType);
    set_field_buffer(screenEntryFields[SF_FIELD_TYPE],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", sf[numsf].fieldRow);
    set_field_buffer(screenEntryFields[SF_FIELD_ROW],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", sf[numsf].fieldCol);
    set_field_buffer(screenEntryFields[SF_FIELD_COL],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", sf[numsf].fieldLen);
    set_field_buffer(screenEntryFields[SF_FIELD_LEN],0,NumToCharPointer);
    sprintf(NumToCharPointer, "%d", sf[numsf].fieldPrec);
    set_field_buffer(screenEntryFields[SF_FIELD_PRE],0,NumToCharPointer);
    set_field_buffer(screenEntryFields[SF_FIELD_EDIT],0,sf[numsf].fieldEditable.c_str());
    set_field_buffer(screenEntryFields[SF_FIELD_REQ],0,sf[numsf].fieldRequired.c_str());
    set_field_buffer(screenEntryFields[SF_FIELD_VIS],0,sf[numsf].fieldVisible.c_str());
    sprintf(NumToCharPointer, "%d", sf[numsf].fieldTabOrder);
    set_field_buffer(screenEntryFields[SF_FIELD_TAB],0,NumToCharPointer);
    set_field_buffer(screenEntryFields[SF_FIELD_ENUMS],0,sf[numsf].fieldEmumValues.c_str());

    displayFunctionKeys(winFieldScreen);

    box(winFormArea, 0, 0);
    box(winFieldScreen, 0, 0);

//    wrefresh(winMsgArea);
    wrefresh(winFormArea);
    wrefresh(winFieldScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        screenFieldFormCleanup(screenEntryFields);
        goto screenFieldStart;
    }

    set_current_field(screenFieldForm,screenEntryFields[SF_FIELD_ID]);

    move(3,19);
    tempFptr = current_field(screenFieldForm);
    formCurrentField = field_index(tempFptr);


    while (quit!=true)
    {


        tempFptr = current_field(screenFieldForm);
        formCurrentField = field_index(tempFptr);


        ch=getch();
        switch (ch)
        {
        case KEY_F(2):

             if (formCurrentField==SF_FIELD_ID)
            {
                def_prog_mode();
                endwin();

                screenFieldDisplayLookup2(screenEntryFields,tableId,winSqlMsgArea);


                refresh();
                pos_form_cursor(screenFieldForm);


            }

            else if (formCurrentField==SF_SCREEN_LOC_ID)
            {

                def_prog_mode();
                endwin();

                screenFieldDisplayLookup(ScreenLayoutId,sf,numsf,screenEntryFields,winSqlMsgArea);

                refresh();
                pos_form_cursor(screenFieldForm);

            }
            break;

        case  KEY_F(3)://delete
            if (confirmAction(winMsgArea,winFieldScreen,2,2,78,"Delete Field",COLOR_PAIR(1),KEY_F(3))==true)

            {
                quit=true;
                screenFieldDeleteField(sf,numsf,ScreenLayoutId,screenYXlocked,winFullScreen,winSqlMsgArea);
                deleted=true;

            }
            pos_form_cursor(screenFieldForm);
            touchwin(winFieldScreen);
            wrefresh(winFormArea);
            wrefresh(winFieldScreen);
            break;

        case KEY_F(4):// save
            //      sync buffer
            form_driver(screenFieldForm, REQ_NEXT_FIELD);
            form_driver(screenFieldForm, REQ_PREV_FIELD);

            if (confirmAction(winMsgArea,winFieldScreen,2,2,78,"Save",COLOR_PAIR(1),KEY_F(4))==true)
            {
                quit=true;
                saved=true;

                sf[numsf].labelText = TrimWhitespaces(field_buffer(screenEntryFields[SF_LABEL_TEXT], 0));
                sf[numsf].labelRow = atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_LABEL_ROW], 0)));
                sf[numsf].labelCol = atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_LABEL_COL], 0)));
                sf[numsf].fieldId = TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_ID], 0));
                sf[numsf].fieldName = TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_NAME], 0));
                sf[numsf].fieldRow=atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_ROW], 0)));
                sf[numsf].fieldCol=atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_COL], 0)));
                sf[numsf].fieldLen=atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_LEN], 0)));
                sf[numsf].fieldPrec=atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_PRE], 0)));
                sf[numsf].fieldType=atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_TYPE], 0)));
                sf[numsf].fieldEditable = TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_EDIT], 0));
                sf[numsf].fieldRequired = TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_REQ], 0));
                sf[numsf].fieldVisible = TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_VIS], 0));
                sf[numsf].fieldTabOrder = atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_TAB], 0)));
                sf[numsf].fieldEmumValues = TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_ENUMS], 0));

            }
            //debugMsg("NewField","",NewField);
            if (NewField==true)

                screenFieldInsertField(sf,numsf,winSqlMsgArea);

            else
                {
                    screenFieldUpdateField(sf,numsf,screenYXlocked,winSqlMsgArea);

                }
            break;



        case KEY_F(7): //exit
            if (confirmAction(winMsgArea,winFieldScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
            {
                quit=true;
            }

            pos_form_cursor(screenFieldForm);
            touchwin(winFieldScreen);
            wrefresh(winFormArea);
            wrefresh(winFieldScreen);
            break;
        case KEY_ENTER:
        case 9: // TAB
        case 10:
        case 13:

            //      sync buffer
            form_driver(screenFieldForm, REQ_NEXT_FIELD);
            form_driver(screenFieldForm, REQ_PREV_FIELD);
            screenFieldVerify(sf,numsf,screenEntryFields,winFullScreen,winFieldScreen,winMsgArea,winSqlMsgArea,ScreenLayoutId,tableId);

            if (formCurrentField==SF_LABEL_COL)
            {

                mvwprintw(winFieldScreen,22, 2, "0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC,5=ENUM,6=YN,7=Date                     ");
                wrefresh(winFieldScreen);

            }


        if (formCurrentField!=SF_LABEL_COL)
            displayFunctionKeys(winFieldScreen);

        form_driver(screenFieldForm, REQ_NEXT_FIELD);
        form_driver(screenFieldForm, REQ_BEG_LINE);
        break;


        case KEY_UP:

                //      sync buffer
                form_driver(screenFieldForm, REQ_NEXT_FIELD);
                form_driver(screenFieldForm, REQ_PREV_FIELD);
                if (formCurrentField==SF_FIELD_ROW)
                {

                    mvwprintw(winFieldScreen,22, 2, "0=ANY,1=ALPHA,2=ALNUM,3=INT,4=NUMERIC,5=ENUM,6=YN,7=Date                     ");
                    wrefresh(winFieldScreen);

                }

                if (formCurrentField!=SF_FIELD_ROW)
                    displayFunctionKeys(winFieldScreen);

                form_driver(screenFieldForm, REQ_PREV_FIELD);
                form_driver(screenFieldForm, REQ_BEG_LINE);

                break;
        case KEY_LEFT:
            form_driver(screenFieldForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(screenFieldForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(screenFieldForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(screenFieldForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(screenFieldForm, REQ_END_FIELD);
            break;
        case KEY_HOME:
            form_driver(screenFieldForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(screenFieldForm, ch);
            break;
        }
        wrefresh(winFormArea);


    }


    screenFieldFormCleanup(screenEntryFields);
    refresh();
    delwin(winFieldScreen);
    //endwin();
    return 0;

}



bool  screenFieldVerify(termScreenFields sf[],int numsf,FIELD *screenEntryFields[],WINDOW *winFullScreen,WINDOW *winFieldScreen, WINDOW *winMsgArea, WINDOW * winSqlMsgArea,string ScreenLayoutId,string tableId)
{


    static FIELD*tempFptr;
    std::string lookup;
    short formCurrentField;

    tempFptr = current_field(screenFieldForm);
    formCurrentField = field_index(tempFptr);



    std::vector<dataDictionary>::iterator dataDictionaryit;
    std::vector<dataDictionary> dataDictionaryResults;



 switch (formCurrentField)
    {
        case SF_FIELD_ID:
        dataDictionaryResults=GetDictionaryFields(tableId,winSqlMsgArea);
        lookup=TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_ID], 0));
        for (dataDictionaryit = dataDictionaryResults.begin(); dataDictionaryit != dataDictionaryResults.end(); ++dataDictionaryit)
            {

                if (lookup==dataDictionaryit->fieldId)
                {
                    setScreenEntryFieldsFromDataDictionary(screenEntryFields,dataDictionaryit,true,winSqlMsgArea);
                    set_current_field(screenFieldForm,screenEntryFields[SF_FIELD_NAME]);
                    break;
                }

              }

        set_current_field(screenFieldForm,screenEntryFields[SF_FIELD_NAME]);

        wrefresh(winFieldScreen);
        wrefresh(winFullScreen);
        pos_form_cursor(screenFieldForm);
        refresh();
        break;
    case SF_FIELD_TYPE:

        if (atoi(TrimWhitespaces(field_buffer(screenEntryFields[SF_FIELD_TYPE], 0)))==6)// type = YN
        {

            set_field_buffer(screenEntryFields[SF_FIELD_ENUMS],0,"Yes,No");

            wrefresh(winFormArea);
            wrefresh(winFieldScreen);
            wrefresh(winFullScreen);
            pos_form_cursor(screenFieldForm);
            refresh();
        }
    break;

    default:
        break;
    }

}


void screenFieldSetupForm(FIELD *screenEntryFields[],WINDOW *winFieldScreen)
{


    winFormArea = derwin(winFieldScreen, 19, 65, 1, 1);
    assert(winFormArea != NULL);
    wattron(winFormArea,COLOR_PAIR(1));
    wrefresh(winFormArea);




//              new_field(h,w,  row,col, offscreen, nbuffers);

    screenEntryFields[0]             =new_field(1, 10,  0,   2,0,0);
    screenEntryFields[SF_KEY]        =new_field(1,  6,  0,  17,0,0);

//Screen Id           ____________________

    screenEntryFields[2]            = new_field(1,  15, 1, 2,0,0);// Label Screen Location
    screenEntryFields[SF_SCREEN_LOC_ID] = new_field(1, 10, 1, 18,0,0);// field Screen Location
//Field Id      ____________________

    screenEntryFields[4]           = new_field(1,  8, 2, 2,0,0);// Label Field Id
    screenEntryFields[SF_FIELD_ID] = new_field(1, 15, 2, 18,0,0);// field Field Id

//Field Name      ____________________

    screenEntryFields[6]              =new_field(1,  10,  3,   2,0,0);
    screenEntryFields[SF_FIELD_NAME]  =new_field(1,  30,  3,  18,0,0);

//Label           ____________________

    screenEntryFields[8]             =new_field(1,  5,  4,   2,0,0);
    screenEntryFields[SF_LABEL_TEXT] =new_field(1, 20,  4,  18,0,0);

//Label Row       __

    screenEntryFields[10]             =new_field(1,  9,  5,   2,0,0);
    screenEntryFields[SF_LABEL_ROW]   =new_field(1,  2,  5,  18,0,0);

//Label Column    __

    screenEntryFields[12]             =new_field(1, 12,  6,   2,0,0);
    screenEntryFields[SF_LABEL_COL]   =new_field(1,  2,  6,  18,0,0);

//Field Type      _  1=ALPHA,2=ALNUM,3=INT,4=NUMERIC
    screenEntryFields[14]            =new_field(1, 10,  7,   2,0,0);
    screenEntryFields[SF_FIELD_TYPE] =new_field(1,  1,  7,  18,0,0);

// Field Row       __

    screenEntryFields[16]            =new_field(1,  9,  8,  2,0,0);
    screenEntryFields[SF_FIELD_ROW]  =new_field(1,  2,  8, 18,0,0);

// Field Col       __

    screenEntryFields[18]            =new_field(1,  9,  9,   2,0,0);
    screenEntryFields[SF_FIELD_COL]  =new_field(1,  2,  9,  18,0,0);

//Field Len       __

    screenEntryFields[20]            =new_field(1,  9,  10,   2,0,0);
    screenEntryFields[SF_FIELD_LEN]  =new_field(1,  2,  10,  18,0,0);

 //Field Precision __

    screenEntryFields[22]            =new_field(1, 15,  11,   2,0,0);
    screenEntryFields[SF_FIELD_PRE]  =new_field(1,  2,  11,  18,0,0);

 //Field Editable? _


    screenEntryFields[24]            =new_field(1, 18,  12,   2,0,0);
    screenEntryFields[SF_FIELD_EDIT] =new_field(1,  1,  12,  18,0,0);

//Field Required?
    screenEntryFields[26]            =new_field(1, 18,  13,   2,0,0);
    screenEntryFields[SF_FIELD_REQ] =new_field(1,  1,  13,  18,0,0);


//Field Visible? _


    screenEntryFields[28]            =new_field(1, 18,  14,   2,0,0);
    screenEntryFields[SF_FIELD_VIS]  =new_field(1,  1,  14,  18,0,0);


//Field Tab Order       __

    screenEntryFields[30]            =new_field(1,  18,  15,   2,0,0);
    screenEntryFields[SF_FIELD_TAB]  =new_field(1,  2,   15,  18,0,0);

//Field Enums  __

    screenEntryFields[32]              =new_field(1,18,   16,   2,0,0);
    screenEntryFields[SF_FIELD_ENUMS]  =new_field(1,40,   16,  18,0,0);




    screenEntryFields[34] = NULL;


    for (int i=0;i<=33;i++)
        assert(screenEntryFields[i] != NULL);


    set_field_buffer(screenEntryFields[0], 0, "Key"); // hidden
    set_field_buffer(screenEntryFields[SF_KEY], 0, ""); //hidden
    set_field_buffer(screenEntryFields[2], 0, "Screen Location");
    set_field_buffer(screenEntryFields[SF_SCREEN_LOC_ID], 0, "");
    set_field_buffer(screenEntryFields[4], 0, "Field Id");
    set_field_buffer(screenEntryFields[SF_FIELD_ID], 0, "");
    set_field_buffer(screenEntryFields[6], 0, "Field Name");
    set_field_buffer(screenEntryFields[SF_FIELD_NAME], 0, "");
    set_field_buffer(screenEntryFields[8], 0, "Label");

    set_field_buffer(screenEntryFields[SF_LABEL_TEXT], 0, "");
    set_field_buffer(screenEntryFields[10], 0, "Label Row");
    set_field_buffer(screenEntryFields[SF_LABEL_ROW], 0, "");
    set_field_buffer(screenEntryFields[12], 0, "Label Column");
    set_field_buffer(screenEntryFields[SF_LABEL_COL], 0, "");


    set_field_buffer(screenEntryFields[14], 0, "Field Type");
    set_field_buffer(screenEntryFields[SF_FIELD_TYPE], 0, "");
    set_field_buffer(screenEntryFields[16], 0, "Field Row");
    set_field_buffer(screenEntryFields[SF_FIELD_ROW], 0, "");
    set_field_buffer(screenEntryFields[18], 0, "Field Column");
    set_field_buffer(screenEntryFields[SF_FIELD_COL], 0, "");
    set_field_buffer(screenEntryFields[20], 0, "Field Length");
    set_field_buffer(screenEntryFields[SF_FIELD_LEN], 0, "");
    set_field_buffer(screenEntryFields[22], 0, "Field Precision");
    set_field_buffer(screenEntryFields[SF_FIELD_PRE], 0, "");
    set_field_buffer(screenEntryFields[24], 0, "Field Editable?");
    set_field_buffer(screenEntryFields[SF_FIELD_EDIT], 0, "");
    set_field_buffer(screenEntryFields[26], 0, "Field Required?");
    set_field_buffer(screenEntryFields[SF_FIELD_REQ], 0, "");
    set_field_buffer(screenEntryFields[28], 0, "Field Visible?");
    set_field_buffer(screenEntryFields[SF_FIELD_VIS], 0, "");
    set_field_buffer(screenEntryFields[30], 0, "Field Tab Order");
    set_field_buffer(screenEntryFields[SF_FIELD_TAB], 0, "");
    set_field_buffer(screenEntryFields[32], 0, "Field Enums");
    set_field_buffer(screenEntryFields[SF_FIELD_ENUMS], 0, "");

    set_field_type(screenEntryFields[SF_KEY], TYPE_NUMERIC,0,1, 999999);// key
    set_field_type(screenEntryFields[SF_SCREEN_LOC_ID] ,TYPE_ALNUM,0);
    set_field_type(screenEntryFields[SF_FIELD_ID] ,TYPE_ALNUM,0);

    // documenation states by default a field will except any data input by the user so
    // dont do a set_field_type if you need to have a text field with spaces.

    set_field_type(screenEntryFields[SF_FIELD_TYPE], TYPE_NUMERIC,0,1, 4);
    set_field_type(screenEntryFields[SF_LABEL_ROW], TYPE_NUMERIC,0,1, 24);
    set_field_type(screenEntryFields[SF_LABEL_COL], TYPE_NUMERIC,0,1, 79);

    //SF_FIELD_NAME allow any chars
    //set_field_type(screenEntryFields[SF_FIELD_NAME], TYPE_ALNUM, 20);

    set_field_type(screenEntryFields[SF_FIELD_ROW], TYPE_NUMERIC,0,1, 24);
    set_field_type(screenEntryFields[SF_FIELD_COL], TYPE_NUMERIC,0,1, 79);
    set_field_type(screenEntryFields[SF_FIELD_LEN], TYPE_NUMERIC,0,1, 60);
    set_field_type(screenEntryFields[SF_FIELD_PRE], TYPE_NUMERIC,0,1, 4);
    set_field_type(screenEntryFields[SF_FIELD_TAB], TYPE_NUMERIC,0,1, 50);



    char * yesno[] = { "Yes", "No", (char *)0 };

    set_field_type(screenEntryFields[SF_FIELD_EDIT],TYPE_ENUM, yesno, false,false);
    set_field_type(screenEntryFields[SF_FIELD_REQ],TYPE_ENUM, yesno, false,false);
    set_field_type(screenEntryFields[SF_FIELD_VIS],TYPE_ENUM, yesno, false,false);


    for (int i=0; i<=33; ++i)

        if (i % 2 !=0) // this is a field
            set_field_back(screenEntryFields[i], A_UNDERLINE|COLOR_PAIR(1));
        else // this is a label
            set_field_fore(screenEntryFields[i], COLOR_PAIR(1));

    set_field_opts(screenEntryFields[0], O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_KEY],  O_PUBLIC |O_AUTOSKIP);
    set_field_opts(screenEntryFields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_SCREEN_LOC_ID], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[4], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_LABEL_TEXT], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
    set_field_opts(screenEntryFields[6], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_LABEL_ROW], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);
    set_field_opts(screenEntryFields[8], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_LABEL_COL], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);
    set_field_opts(screenEntryFields[10], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_ID], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

    set_field_opts(screenEntryFields[12], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_NAME], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

    set_field_opts(screenEntryFields[14], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_TYPE], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);
    set_field_opts(screenEntryFields[16], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_ROW], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);
    set_field_opts(screenEntryFields[18], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_COL], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);

    set_field_opts(screenEntryFields[20], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_LEN], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);

    set_field_opts(screenEntryFields[22], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_PRE], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

    set_field_opts(screenEntryFields[24], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_EDIT], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

    set_field_opts(screenEntryFields[26], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_REQ], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

    set_field_opts(screenEntryFields[28], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_VIS], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_LEFT|O_NULLOK);

    set_field_opts(screenEntryFields[30], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_TAB], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);

    set_field_opts(screenEntryFields[32], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(screenEntryFields[SF_FIELD_ENUMS], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE|O_STATIC|O_BLANK|JUSTIFY_RIGHT|O_NULLOK);


    screenFieldForm = new_form(screenEntryFields);
    assert(screenFieldForm != NULL);
    set_form_win(screenFieldForm,winFieldScreen);
    set_form_sub(screenFieldForm,winFormArea);
}


void  screenFieldFormCleanup(FIELD *screenEntryFields[])
{
    unpost_form(screenFieldForm);
    free_form(screenFieldForm);
    for (int i=0; i<=33; ++i)
    {
        free_field(screenEntryFields[i]);
    }
    delwin(winFormArea);
    wclear(stdscr);

}

int displayFunctionKeys(WINDOW *winFieldScreen)
{
    mvwprintw(winFieldScreen,22, 2, "                                                                          ");
    mvwprintw(winFieldScreen,22, 12, "F3 Delete");
    mvwprintw(winFieldScreen,22, 22, "F4 Save");
    mvwprintw(winFieldScreen,22, 42, "F7 Exit");

    wrefresh(winFieldScreen);
    refresh();
}

int screenFieldInsertField(termScreenFields sf[],int numsf,WINDOW *winSqlMsgArea)
{

    string sql;
    char NumToCharPointer[10];
    int identity;
    dbScreenFields row;




    row.ScreenLayoutId = sf[numsf].ScreenLayoutId;
    sprintf(NumToCharPointer, "Row%02dCol%02d", sf[numsf].fieldRow,sf[numsf].fieldCol);



    row.ScreenLocId = NumToCharPointer;
    row.tableId = sf[numsf].tableId;
    row.fieldId = sf[numsf].fieldId;
    row.fieldName = sf[numsf].fieldName;
    row.labelText =sf[numsf].labelText;
    row.labelRow = sf[numsf].labelRow;
    row.labelCol = sf[numsf].labelCol;

    row.fieldType = sf[numsf].fieldType;
    row.fieldRow = sf[numsf].fieldRow;
    row.fieldCol = sf[numsf].fieldCol;
    row.fieldLen = sf[numsf].fieldLen;
    row.fieldPrec = sf[numsf].fieldPrec;
    row.fieldEditable =sf[numsf].fieldEditable;
    row.fieldRequired =sf[numsf].fieldRequired;
    row.fieldVisible =sf[numsf].fieldVisible;
    row.fieldTabOrder=sf[numsf].fieldTabOrder;
    row.fieldEmumValues =sf[numsf].fieldEmumValues;



    sql = "INSERT INTO screenFields ";
    sql.append ("(ScreenLayoutId,ScreenLocId,tableId,FieldId,fieldName,labelText,labelRow, labelCol,fieldType,fieldRow,");
    sql.append("fieldCol,fieldLen,fieldPrec,fieldEditable,fieldRequired,fieldVisible,fieldTabOrder,fieldEmumValues) ");
    sql.append("VALUES ('" );
    sql.append (row.ScreenLayoutId);
    sql.append("','");
    sql.append (row.ScreenLocId);
    sql.append("','");
    sql.append (row.tableId);
    sql.append("','");
    sql.append (row.fieldId);
    sql.append("','");
    sql.append (row.fieldName);
    sql.append("','");
    sql.append (row.labelText);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.labelRow);
    sql.append(NumToCharPointer);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.labelCol);
    sql.append(NumToCharPointer);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.fieldType);
    sql.append(NumToCharPointer);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.fieldRow);
    sql.append(NumToCharPointer);
    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.fieldCol);
    sql.append(NumToCharPointer);

    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.fieldLen);
    sql.append(NumToCharPointer);

    sql.append("','");
    sprintf(NumToCharPointer, "%d", row.fieldPrec);
    sql.append(NumToCharPointer);

    sql.append("','");
    sql.append (row.fieldEditable);
    sql.append("','");

    sql.append (row.fieldRequired);
    sql.append("','");
    sql.append (row.fieldVisible);
    sql.append("','");

    sprintf(NumToCharPointer, "%d", row.fieldTabOrder);
    sql.append(NumToCharPointer);

    sql.append("','");
    sql.append (row.fieldEmumValues);

    sql.append("')");
    //debugMsg("sql ",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);


    return identity;
}

int screenFieldDeleteField(termScreenFields sf[],int numsf,string ScreenLayoutId,bool screenYXlocked[][79],WINDOW* winFullScreen,WINDOW* winSqlMsgArea)
{

    int fieldEndCol=0,curRow=0,len=0;
    curRow=sf[numsf].fieldRow;
    len=strlen(sf[numsf].labelText.c_str());
    std::string sLabelSpace(len++, ' ');
    std::string sFieldSpace(sf[numsf].fieldLen, ' ');

    mvwaddstr(winFullScreen,sf[numsf].labelRow,sf[numsf].labelCol,sLabelSpace.c_str());
    mvwaddstr(winFullScreen,sf[numsf].fieldRow,sf[numsf].fieldCol,sFieldSpace.c_str());


    mvwhline(winFullScreen,sf[numsf].fieldRow, sf[numsf].fieldCol,' ', sf[numsf].fieldLen);
    wmove(winFullScreen,sf[numsf].fieldRow,sf[numsf].fieldCol-1);

    wrefresh(winFullScreen);
    refresh();

    // unlock the positions
     fieldEndCol= sf[numsf].fieldCol+sf[numsf].fieldLen;
                for (int x=sf[numsf].fieldCol;x<fieldEndCol;x++)
                    screenYXlocked [curRow] [x]=false;

    string sql;
    char NumToCharPointer[10];

    sql = "DELETE FROM screenFields ";
    sql.append("WHERE ScreenLayoutId= ");
    sql.append("\"""");
    sql.append (ScreenLayoutId);
    sql.append("\"");


    sql.append(" AND ScreenLocId= ");
    sql.append("\"");

    sql.append(sf[numsf].ScreenLocId);

    sql.append("\"");
    //debugMsg("sql",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);









    sf[numsf].fieldName="";
    sf[numsf].labelText="";
    sf[numsf].labelCol=0;
    sf[numsf].labelRow=0;
    sf[numsf].fieldType=0;
    sf[numsf].fieldRow=0;
    sf[numsf].fieldCol=0;
    sf[numsf].fieldLen=0;
    sf[numsf].fieldPrec=0;
    sf[numsf].fieldEditable="";
    sf[numsf].fieldRequired="";
    sf[numsf].fieldVisible="";
    sf[numsf].fieldTabOrder=0;
    sf[numsf].fieldEmumValues="";




}


int screenFieldUpdateField(termScreenFields sf[],int numsf,bool screenYXlocked[][79],WINDOW* winSqlMsgArea)
{
    int fieldEndCol=0,curRow=0;
    curRow=sf[numsf].fieldRow;


    // unlock the positions
     fieldEndCol= sf[numsf].fieldCol+sf[numsf].fieldLen;
                for (int x=sf[numsf].fieldCol;x<fieldEndCol;x++)
                    screenYXlocked [curRow] [x]=false;


    string sql;


    sql = "DELETE FROM screenFields ";
    sql.append("WHERE ScreenLayoutId= ");
    sql.append("\"""");
    sql.append (sf[numsf].ScreenLayoutId);
    sql.append("\"");
    sql.append(" AND ScreenLocId= ");
    sql.append("\"");
    sql.append(sf[numsf].ScreenLocId);
    sql.append("\"");
     //debugMsg("sql ",sql.c_str(),0);
    execSQLAction(sql,winSqlMsgArea);
    screenFieldInsertField(sf,numsf,winSqlMsgArea);

}

int screenFieldDisplayLookup2(FIELD *screenEntryFields[],string tableId,WINDOW *winSqlMsgArea)
{

    ITEM **myItems;
    int c,r;
    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;
    std::vector<fields> fieldsResults;
    std::vector<fields>::iterator fieldsit;
    std::vector<dataDictionary>::iterator dataDictionaryit;
    std::vector<dataDictionary> dataDictionaryResults;

    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;
    bool EditingRow= false;
    bool NewRow= false;
    static FIELD*tempFptr;
    string lookup="";



    short formCurrentField;
    tempFptr = current_field(screenFieldForm);
    formCurrentField = field_index(tempFptr);


    dataDictionaryResults=GetDictionaryFields(tableId,winSqlMsgArea);
    NumChoices= dataDictionaryResults.size();


    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;

    GetAllFields(fieldsResults,winSqlMsgArea);


    // dataDictionaryit is limited to those rows where  the table equals the table Id of the screen def
    for (dataDictionaryit = dataDictionaryResults.begin(); dataDictionaryit != dataDictionaryResults.end(); ++dataDictionaryit)
    {


        lookup=dataDictionaryit->fieldId;


        for (fieldsit= fieldsResults.begin();fieldsit!= fieldsResults.end();++fieldsit)
        {
            if (fieldsit->fieldId==lookup)
            {
                myItems[r] = new_item(fieldsit->fieldId.c_str(),fieldsit->Name.c_str());
                break;
            }
        }

        intptrs.push_back(&dataDictionaryit->id);

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

                setScreenEntryFieldsFromDataDictionary(screenEntryFields,dataDictionaryit,EditingRow,winSqlMsgArea); // dataDictionaryit is not  used have to pass something in this case

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


                for (dataDictionaryit = dataDictionaryResults.begin(); dataDictionaryit != dataDictionaryResults.end(); ++dataDictionaryit)
                    {
                        if (dataDictionaryit->id==retValue)
                        {
                            EditingRow=true;
                            NewRow=false;

                            setScreenEntryFieldsFromDataDictionary(screenEntryFields,dataDictionaryit,EditingRow,winSqlMsgArea);
                            set_current_field(screenFieldForm,screenEntryFields[SF_FIELD_NAME]);

                            break;

                        }

                    }

//
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
        dataDictionaryResults.clear();
        fieldsResults.clear();
        intptrs.clear();
    }



    return retValue;// not used yet


}

int screenFieldDisplayLookup(std::string ScreenLayoutId,termScreenFields sf[],int numsf,FIELD *screenEntryFields[],WINDOW *winSqlMsgArea)
{

    return 0; // decided not to allow F2 lookup on screen id

    ITEM **myItems;
    int c,r;

    MENU *myLookUpMenu;
    WINDOW *myLookUpMenuWin;
    int NumChoices=0,retValue=0;

    std::vector<dbScreenFields> screenEntryFieldsResults;
    std::vector<dbScreenFields>::iterator screenEntryFieldsit;
    std::vector <int*> intptrs;
    char NumToCharPointer[10];
    bool exitMenu = false;
    bool EditingRow= false;
    bool NewRow= false;
    static FIELD*tempFptr;



    short formCurrentField;
    tempFptr = current_field(screenFieldForm);
    formCurrentField = field_index(tempFptr);


    NumChoices=GetAllScreenEntryFields(screenEntryFieldsResults,ScreenLayoutId,winSqlMsgArea);


    myItems = (ITEM **)calloc(NumChoices+1, sizeof(ITEM *));

    r=0;
    for (screenEntryFieldsit = screenEntryFieldsResults.begin(); screenEntryFieldsit != screenEntryFieldsResults.end(); ++screenEntryFieldsit)
    {

        myItems[r] = new_item(screenEntryFieldsit->ScreenLocId.c_str(),screenEntryFieldsit->fieldName.c_str());

        intptrs.push_back(&screenEntryFieldsit->idscreenFields);

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
                setScreenEntryFieldsFromScreenFields(sf,numsf,screenEntryFields,screenEntryFieldsit,EditingRow); // screenEntryFieldsit is not  used have to pass something in this case
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

                for (screenEntryFieldsit = screenEntryFieldsResults.begin(); screenEntryFieldsit != screenEntryFieldsResults.end(); ++screenEntryFieldsit)
                {
                    if (screenEntryFieldsit->idscreenFields==retValue)
                    {
                        EditingRow=true;
                        NewRow=false;

                        setScreenEntryFieldsFromScreenFields(sf,numsf,screenEntryFields,screenEntryFieldsit,EditingRow);
                        set_current_field(screenFieldForm,screenEntryFields[1]);

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
        screenEntryFieldsResults.clear();
        intptrs.clear();
    }



    return retValue;// not used yet


}
void setScreenEntryFieldsFromDataDictionary(FIELD *screenEntryFields[],std::vector<dataDictionary>::iterator dataDictionaryit,bool EditingRow,WINDOW *winSqlMsgArea)
{
    char NumToCharPointer[10];
    std::vector<fields>::iterator fieldsit;
    std::vector<fields> fieldsResults;
    string lookup;

    if (EditingRow==true)
    {
        lookup=dataDictionaryit->fieldId;
        fieldsResults=GetField(lookup,winSqlMsgArea);
        fieldsit= fieldsResults.begin();

        set_field_buffer(screenEntryFields[SF_FIELD_ID], 0,  fieldsit->fieldId.c_str());
        set_field_buffer(screenEntryFields[SF_FIELD_NAME], 0,  fieldsit->Name.c_str());
        sprintf(NumToCharPointer, "%i", fieldsit->Type);
        set_field_buffer(screenEntryFields[SF_FIELD_TYPE], 0,  NumToCharPointer);
        sprintf(NumToCharPointer, "%i", fieldsit->Length);
        set_field_buffer(screenEntryFields[SF_FIELD_LEN], 0,  NumToCharPointer);
        sprintf(NumToCharPointer, "%i", fieldsit->FieldPrecision);
        set_field_buffer(screenEntryFields[SF_FIELD_PRE], 0,  NumToCharPointer);
        set_field_buffer(screenEntryFields[SF_FIELD_ENUMS], 0,  fieldsit->EmumValues.c_str());
    }


    }

void setScreenEntryFieldsFromScreenFields(termScreenFields sf[],int numsf,FIELD *screenEntryFields[],std::vector<dbScreenFields>::iterator screenFieldsit,bool EditingRow)
{
    char NumToCharPointer[10];

    if (EditingRow==true)
    {
        set_field_buffer(screenEntryFields[SF_SCREEN_LOC_ID], 0,  screenFieldsit->ScreenLocId.c_str());
        set_field_buffer(screenEntryFields[SF_FIELD_ID], 0,  screenFieldsit->fieldId.c_str());
        set_field_buffer(screenEntryFields[SF_FIELD_NAME], 0,  screenFieldsit->fieldName.c_str());
        set_field_buffer(screenEntryFields[SF_LABEL_TEXT], 0,  screenFieldsit->labelText.c_str());

        sprintf(NumToCharPointer, "%i", screenFieldsit->labelRow);
        set_field_buffer(screenEntryFields[SF_LABEL_ROW], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenFieldsit->labelCol);
        set_field_buffer(screenEntryFields[SF_LABEL_COL], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenFieldsit->fieldType);
        set_field_buffer(screenEntryFields[SF_FIELD_TYPE], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenFieldsit->fieldRow);
        set_field_buffer(screenEntryFields[SF_FIELD_ROW], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenFieldsit->fieldCol);
        set_field_buffer(screenEntryFields[SF_FIELD_COL], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenFieldsit->fieldLen);
        set_field_buffer(screenEntryFields[SF_FIELD_LEN], 0,  NumToCharPointer);

        sprintf(NumToCharPointer, "%i", screenFieldsit->fieldPrec);
        set_field_buffer(screenEntryFields[SF_FIELD_PRE], 0,  NumToCharPointer);

        set_field_buffer(screenEntryFields[SF_FIELD_EDIT], 0,  screenFieldsit->fieldEditable.c_str());set_field_buffer(screenEntryFields[SF_FIELD_EDIT], 0,  screenFieldsit->fieldEditable.c_str());
        set_field_buffer(screenEntryFields[SF_FIELD_REQ], 0,  screenFieldsit->fieldRequired.c_str());
        set_field_buffer(screenEntryFields[SF_FIELD_VIS], 0,  screenFieldsit->fieldVisible.c_str());

        sprintf(NumToCharPointer, "%i", screenFieldsit->fieldTabOrder);
        set_field_buffer(screenEntryFields[SF_FIELD_TAB], 0,  NumToCharPointer);
        set_field_buffer(screenEntryFields[SF_FIELD_ENUMS], 0,  screenFieldsit->fieldEmumValues.c_str());

        sf[numsf].ScreenLocId=screenFieldsit->ScreenLocId;
        sf[numsf].fieldId=screenFieldsit->fieldId;
        sf[numsf].labelText=screenFieldsit->labelText;
        sf[numsf].labelRow=screenFieldsit->labelRow;
        sf[numsf].labelCol=screenFieldsit->labelCol;
        sf[numsf].fieldName=screenFieldsit->fieldName;
        sf[numsf].fieldType=screenFieldsit->fieldType;
        sf[numsf].fieldRow=screenFieldsit->fieldRow;
        sf[numsf].fieldCol=screenFieldsit->fieldCol;
        sf[numsf].fieldLen=screenFieldsit->fieldLen;
        sf[numsf].fieldPrec=screenFieldsit->fieldPrec;
        sf[numsf].fieldEditable=screenFieldsit->fieldEditable;
        sf[numsf].fieldRequired=screenFieldsit->fieldRequired;
        sf[numsf].fieldVisible=screenFieldsit->fieldVisible;
        sf[numsf].fieldTabOrder=screenFieldsit->fieldTabOrder;
        sf[numsf].fieldEmumValues = screenFieldsit->fieldEmumValues;



    }
    else
    {

    for (int i=0; i<=33; ++i)

        if (i % 2 !=0)
            set_field_buffer(screenEntryFields[i], 0,  "");


    }

}


