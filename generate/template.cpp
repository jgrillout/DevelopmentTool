
dataEntryStart:

    bool quit=false;
    dataEntrySetupForm(dataEntryFields,winFullScreen);
    post_form(dataEntryForm);
    set_current_field(dataEntryForm,dataEntryFields[0]);


    wattron(winFullScreen,A_REVERSE);
    mvwprintw(winFullScreen,22, 2, "F2 Lookup");
    mvwprintw(winFullScreen,22, 12, "F3 Delete");
    mvwprintw(winFullScreen,22, 22, "F4 Save");
    mvwprintw(winFullScreen,22, 32, "F5 New");
    mvwprintw(winFullScreen,22, 42, "F7 Exit");
    wattroff(winFullScreen,A_REVERSE);
    box(winFormArea, 0, 0);
    PrintInMiddle(winFormArea, 0, 0, 80, " %title% ", COLOR_PAIR(1));

    wrefresh(winFormArea);
    wrefresh(winFullScreen);
    refresh();

    if (firstTime==true)
    {
        firstTime=false;
        dataEntryFormCleanup(dataEntryFields);
        goto dataEntryStart;
    }


    while (quit!=true)
    {

        tempFptr = current_field(dataEntryForm);
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
            form_driver(dataEntryForm, REQ_NEXT_FIELD);
            // IF changed to REQ_END_LINE THEN DATE & ENUM TYPES DON'T CLEAR ON TYPING 1ST CHAR
            form_driver(dataEntryForm, REQ_BEG_LINE); 
            break;

        case KEY_UP:
            form_driver(dataEntryForm, REQ_PREV_FIELD);
            // IF changed to REQ_END_LINE THEN DATE & ENUM TYPES DON'T CLEAR ON TYPING 1ST CHAR
            form_driver(dataEntryForm, REQ_BEG_LINE); 
            break;


        case KEY_LEFT:
            form_driver(dataEntryForm, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(dataEntryForm, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(dataEntryForm, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(dataEntryForm, REQ_DEL_CHAR);
            break;
        case KEY_END:
            form_driver(dataEntryForm, REQ_BEG_FIELD);
            break;
        case KEY_HOME:
            form_driver(dataEntryForm, REQ_BEG_FIELD);
            break;

        default:

            form_driver(dataEntryForm, ch);
            break;
        }
        wrefresh(winFormArea);

    }
if (confirmAction(winMsgArea,winFullScreen,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)

    {

        dataEntryFormCleanup(dataEntryFields);

    }
    else
    {
        dataEntryFormCleanup(dataEntryFields);
        goto dataEntryStart;
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







