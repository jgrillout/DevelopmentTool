#include "main.hpp"
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int Customers_CUSTID_LABEL(0);
const int Customers_CUSTID_FIELD(1);
const int Customers_NAME_LABEL(2);
const int Customers_NAME_FIELD(3);
const int Customers_ADDRESS_LABEL(4);
const int Customers_ADDRESS_FIELD(5);
const int Customers_CITY_LABEL(6);
const int Customers_CITY_FIELD(7);
const int Customers_STATE_LABEL(8);
const int Customers_STATE_FIELD(9);
const int Customers_ZIP_LABEL(10);
const int Customers_ZIP_FIELD(11);
const int Customers_ACTIVE_LABEL(12);
const int Customers_ACTIVE_FIELD(13);
const int Customers_INVTYPE_LABEL(14);
const int Customers_INVTYPE_FIELD(15);
const int Customers_INVDATE_LABEL(16);
const int Customers_INVDATE_FIELD(17);
static WINDOW *winFormArea;
static WINDOW *winFullScreen;
static WINDOW *winMsgArea;
static FORM *CustomersEntryForm;
int CustomersEntry(WINDOW *winMsgArea,WINDOW *winFullScreen);

void CustomersEntrySetupForm(FIELD *CustomersEntryFields[],WINDOW *winFullScreen);

void CustomersEntryFormCleanup(FIELD *CustomersEntryFields[]);

int  main(void)
{
	 initscr();
	 start_color();
	 init_pair(1,COLOR_GREEN,COLOR_BLACK);
	 cbreak();
	 noecho();
	 keypad(stdscr, TRUE);
	 wattron(stdscr,COLOR_PAIR(1));
	 winFullScreen = newwin(24, 80, 0, 0);
	 assert(winFullScreen != NULL);
	 wattron(winFullScreen,COLOR_PAIR(1));
	 wrefresh(winFullScreen);
	 winMsgArea = newwin(4, 78, 19, 1);
	 assert(winMsgArea != NULL);
	 wattron(winMsgArea,COLOR_PAIR(1));
	 wrefresh(winMsgArea);
	 refresh();
	 CustomersEntry(winMsgArea,winFullScreen);
}
int CustomersEntry(WINDOW *winMsgArea,WINDOW *winFullScreen)
{
short formCurrentField;
int key=0, ch=0;
static FIELD*tempFptr;
bool firstTime=true;
FIELD *CustomersEntryFields[18];

CustomersEntryStart:

    bool quit=false;
    CustomersEntrySetupForm(CustomersEntryFields,winFullScreen);
    post_form(CustomersEntryForm);
    set_current_field(CustomersEntryForm,CustomersEntryFields[0]);


    wattron(winFullScreen,A_REVERSE);
    mvwprintw(winFullScreen,22, 2, "F2 Lookup");
    mvwprintw(winFullScreen,22, 12, "F3 Delete");
    mvwprintw(winFullScreen,22, 22, "F4 Save");
    mvwprintw(winFullScreen,22, 32, "F5 New");
    mvwprintw(winFullScreen,22, 42, "F7 Exit");
    wattroff(winFullScreen,A_REVERSE);
    box(winFormArea, 0, 0);
    PrintInMiddle(winFormArea, 0, 0, 80, " Customers ", COLOR_PAIR(1));

    wrefresh(winFormArea);
    wrefresh(winFullScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        CustomersEntryFormCleanup(CustomersEntryFields);
        goto CustomersEntryStart;
    }


    while (quit!=true)
    {

        tempFptr = current_field(CustomersEntryForm);
        formCurrentField = field_index(tempFptr);
        ch=getch();
        switch (ch)
        {

        case KEY_F(1):
            break;
        case KEY_F(2):
            break;

        case  KEY_F(3):
            break;

        case KEY_F(4):
            break;

        case KEY_F(5):
            break;

        case KEY_F(7):
	    quit=true;
            break;
        case KEY_ENTER:
        case 9:
        case 10:
        case 13:
            form_driver(CustomersEntryForm, REQ_NEXT_FIELD);
            // IF changed to REQ_END_LINE THEN DATE & ENUM TYPES DON'T CLEAR ON TYPING 1ST CHAR
            form_driver(CustomersEntryForm, REQ_BEG_LINE); 
            break;

        case KEY_UP:
            form_driver(CustomersEntryForm, REQ_PREV_FIELD);
            // IF changed to REQ_END_LINE THEN DATE & ENUM TYPES DON'T CLEAR ON TYPING 1ST CHAR
            form_driver(CustomersEntryForm, REQ_BEG_LINE); 
            break;


        case KEY_LEFT:
            form_driver(CustomersEntryForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(CustomersEntryForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(CustomersEntryForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(CustomersEntryForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(CustomersEntryForm, REQ_BEG_FIELD);
            break;
        case KEY_HOME:
            form_driver(CustomersEntryForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(CustomersEntryForm, ch);
            break;
        }
        wrefresh(winFormArea);

    }
if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)

    {

        CustomersEntryFormCleanup(CustomersEntryFields);

    }
    else
    {
        CustomersEntryFormCleanup(CustomersEntryFields);
        goto CustomersEntryStart;
    }

    return 0;

}
static bool MyDateFieldChecker(FIELD * fld, const void * data GCC_UNUSED)
{


    char* sourcedate;
    char year[5];
    char month[3];
    char day[3];
    char TestDate[11];


    sourcedate=TrimWhitespaces(field_buffer(fld, 0));

    if (sourcedate[4]=='/' || sourcedate[4]=='-') // date is probably yyyy/mm/dd or yyyy-mm-dd
                                                  // so switch it to mm/dd/yyyy
    {
        //debugMsg("changing date","",0);
        strncpy (year, sourcedate, 4 );
        year[4] = '\0';
        strncpy (month, sourcedate+5, 2 );
        month[2] = '\0';
        strncpy (day, sourcedate+8, 2 );
        day[2] = '\0';

//        debugMsg("year",year,0);
//        debugMsg("month",month,0);
//        debugMsg("day",day,0);


        TestDate[0] = month[0];
        TestDate[1] = month[1];
        TestDate[2] = '/';
        TestDate[3]= day[0];
        TestDate[4] = day[1];
        TestDate[5] = '/';
        TestDate[6] = year[0];
        TestDate[7] = year[1];
        TestDate[8] = year[2];
        TestDate[9] = year[3];
        TestDate[10] = '\0';


    }
    else
    {

        strncpy (TestDate, sourcedate, 10 );
        TestDate[10]='\0';

    }
     //debugMsg("TestDate",TestDate,0);
     return isValidDate(TestDate);
}
static bool MyDateCharChecker(int ch, const void * data GCC_UNUSED)
{

    //debugMsg("MyDateCharChecker","",0);
    return ((isdigit(ch) || ch == '/' || ch=='-') ? TRUE : FALSE);

}
FIELDTYPE *PE_CustomDateType = new_fieldtype(MyDateFieldChecker, MyDateCharChecker);








void CustomersEntrySetupForm(FIELD *CustomersEntryFields[],WINDOW *winFullScreen)

{

init_pair(1,COLOR_GREEN,COLOR_BLACK);
winFormArea = derwin(winFullScreen, 20, 78, 1, 1);
assert(winFormArea != NULL);
wattron(winFormArea,COLOR_PAIR(1));
 wrefresh(winFormArea);
//              new_field(h,w,  tp,left, offscreen, nbuffers);
CustomersEntryFields[Customers_CUSTID_LABEL] = new_field(1, 7,1,1,0,0);// Label Cust Id
CustomersEntryFields[Customers_CUSTID_FIELD] = new_field(1, 6,1,9,0,0);// field CUSTID
CustomersEntryFields[Customers_NAME_LABEL] = new_field(1, 4,2,1,0,0);// Label Name
CustomersEntryFields[Customers_NAME_FIELD] = new_field(1, 30,2,9,0,0);// field NAME
CustomersEntryFields[Customers_ADDRESS_LABEL] = new_field(1, 7,3,1,0,0);// Label Address
CustomersEntryFields[Customers_ADDRESS_FIELD] = new_field(1, 30,3,9,0,0);// field ADDRESS
CustomersEntryFields[Customers_CITY_LABEL] = new_field(1, 4,4,1,0,0);// Label City
CustomersEntryFields[Customers_CITY_FIELD] = new_field(1, 30,4,9,0,0);// field CITY
CustomersEntryFields[Customers_STATE_LABEL] = new_field(1, 5,4,40,0,0);// Label State
CustomersEntryFields[Customers_STATE_FIELD] = new_field(1, 2,4,46,0,0);// field STATE
CustomersEntryFields[Customers_ZIP_LABEL] = new_field(1, 3,4,49,0,0);// Label Zip
CustomersEntryFields[Customers_ZIP_FIELD] = new_field(1, 5,4,53,0,0);// field ZIP
CustomersEntryFields[Customers_ACTIVE_LABEL] = new_field(1, 6,6,1,0,0);// Label Active
CustomersEntryFields[Customers_ACTIVE_FIELD] = new_field(1, 3,6,9,0,0);// field ACTIVE
CustomersEntryFields[Customers_INVTYPE_LABEL] = new_field(1, 12,8,1,0,0);// Label Invoice Type
CustomersEntryFields[Customers_INVTYPE_FIELD] = new_field(1, 7,8,19,0,0);// field INVTYPE
CustomersEntryFields[Customers_INVDATE_LABEL] = new_field(1, 17,10,1,0,0);// Label Last Invoice Date
CustomersEntryFields[Customers_INVDATE_FIELD] = new_field(1, 10,10,19,0,0);// field INVDATE
CustomersEntryFields[18] = NULL;
 for (int i=0;i<18;i++)
 {
	assert(CustomersEntryFields[i] !=NULL);
 }
set_field_buffer(CustomersEntryFields[Customers_CUSTID_LABEL] ,0,"Cust Id");//Label Cust Id
set_field_buffer(CustomersEntryFields[Customers_CUSTID_FIELD] ,0,"");// field CUSTID
set_field_buffer(CustomersEntryFields[Customers_NAME_LABEL] ,0,"Name");//Label Name
set_field_buffer(CustomersEntryFields[Customers_NAME_FIELD] ,0,"");// field NAME
set_field_buffer(CustomersEntryFields[Customers_ADDRESS_LABEL] ,0,"Address");//Label Address
set_field_buffer(CustomersEntryFields[Customers_ADDRESS_FIELD] ,0,"");// field ADDRESS
set_field_buffer(CustomersEntryFields[Customers_CITY_LABEL] ,0,"City");//Label City
set_field_buffer(CustomersEntryFields[Customers_CITY_FIELD] ,0,"");// field CITY
set_field_buffer(CustomersEntryFields[Customers_STATE_LABEL] ,0,"State");//Label State
set_field_buffer(CustomersEntryFields[Customers_STATE_FIELD] ,0,"");// field STATE
set_field_buffer(CustomersEntryFields[Customers_ZIP_LABEL] ,0,"Zip");//Label Zip
set_field_buffer(CustomersEntryFields[Customers_ZIP_FIELD] ,0,"");// field ZIP
set_field_buffer(CustomersEntryFields[Customers_ACTIVE_LABEL] ,0,"Active");//Label Active
set_field_buffer(CustomersEntryFields[Customers_ACTIVE_FIELD] ,0,"");// field ACTIVE
set_field_buffer(CustomersEntryFields[Customers_INVTYPE_LABEL] ,0,"Invoice Type");//Label Invoice Type
set_field_buffer(CustomersEntryFields[Customers_INVTYPE_FIELD] ,0,"");// field INVTYPE
set_field_buffer(CustomersEntryFields[Customers_INVDATE_LABEL] ,0,"Last Invoice Date");//Label Last Invoice Date
set_field_buffer(CustomersEntryFields[Customers_INVDATE_FIELD] ,0,"");// field INVDATE
set_field_fore(CustomersEntryFields[Customers_CUSTID_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_CUSTID_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_CUSTID_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_CUSTID_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
set_field_fore(CustomersEntryFields[Customers_NAME_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_NAME_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_NAME_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_NAME_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
set_field_fore(CustomersEntryFields[Customers_ADDRESS_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_ADDRESS_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_ADDRESS_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_ADDRESS_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
set_field_fore(CustomersEntryFields[Customers_CITY_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_CITY_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_CITY_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_CITY_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
set_field_type(CustomersEntryFields[Customers_STATE_FIELD] ,TYPE_ALPHA,0);
set_field_fore(CustomersEntryFields[Customers_STATE_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_STATE_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_STATE_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_STATE_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
set_field_fore(CustomersEntryFields[Customers_ZIP_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_ZIP_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_ZIP_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_ZIP_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
char * Row06Col09choices[] = { "Yes","No", (char *)0 };
set_field_type(CustomersEntryFields[Customers_ACTIVE_FIELD],TYPE_ENUM, Row06Col09choices, false,false);
set_field_fore(CustomersEntryFields[Customers_ACTIVE_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_ACTIVE_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_ACTIVE_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_ACTIVE_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
char * Row08Col19choices[] = { "Detail","Summary", (char *)0 };
set_field_type(CustomersEntryFields[Customers_INVTYPE_FIELD],TYPE_ENUM, Row08Col19choices, false,false);
set_field_fore(CustomersEntryFields[Customers_INVTYPE_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_INVTYPE_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_INVTYPE_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_INVTYPE_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
set_field_type(CustomersEntryFields[Customers_INVDATE_FIELD],PE_CustomDateType);
set_field_fore(CustomersEntryFields[Customers_INVDATE_LABEL],COLOR_PAIR(1));
set_field_opts(CustomersEntryFields[Customers_INVDATE_LABEL], O_PUBLIC | O_AUTOSKIP|O_VISIBLE); 
set_field_back(CustomersEntryFields[Customers_INVDATE_FIELD], COLOR_PAIR(1)|A_UNDERLINE); 
set_field_opts(CustomersEntryFields[Customers_INVDATE_FIELD], A_UNDERLINE|O_PUBLIC |O_STATIC|O_BLANK|O_EDIT|O_ACTIVE|O_NULLOK|O_VISIBLE|JUSTIFY_LEFT); 
CustomersEntryForm = new_form(CustomersEntryFields);
assert(CustomersEntryForm != NULL);
set_form_win(CustomersEntryForm,winFullScreen);
set_form_sub(CustomersEntryForm,winFormArea);
move(2,10);
}
void CustomersEntryFormCleanup(FIELD *CustomersEntryFields[])

{

unpost_form( CustomersEntryForm);

 free_form(CustomersEntryForm);

 for (int i=0; i<18;i++)

	{

		 free_field(CustomersEntryFields[i]); 

	}

	delwin(winFormArea);

	endwin(); 

}

