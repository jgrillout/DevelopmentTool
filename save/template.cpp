static FORM *dataEntryForm;
static WINDOW *winFormArea;
static WINDOW *winFullScreen;
static WINDOW *winMsgArea;
using namespace std;
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

    dataEntry(winMsgArea,winFullScreen);
}

int dataEntry(WINDOW *winMsgArea, WINDOW *winFullScreen)
{

    short formCurrentField;
    bool firstTime=true,quit=false;
    int key=0, ch=0;
    bool EditingRow=false;
    bool NewRow=false;

    static FIELD *dataEntryFields[10];
    static FIELD*tempFptr;

dataEntryStart:

    quit=false;
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
    move(2,11);
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
            break;
        case KEY_ENTER:
        case 9:
        case 10:
        case 13:
            form_driver(dataEntryForm, REQ_NEXT_FIELD);
            form_driver(dataEntryForm, REQ_BEG_LINE);
            break;

        case KEY_UP:
            form_driver(dataEntryForm, REQ_PREV_FIELD);
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
            form_driver(dataEntryForm, REQ_END_FIELD);
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
    dataEntryFormCleanup(dataEntryFields);
    return 0;

}


void  dataEntryFormCleanup(FIELD *dataEntryFields[])
{
    unpost_form(dataEntryForm);
    free_form(dataEntryForm);
    for (int i=0; i<3; i++)
    {
        free_field(dataEntryFields[i]);
    }
    delwin(winFormArea);

    endwin();
}
