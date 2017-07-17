
#ifndef MAIN_HPP_INCLUDED
#include "main.hpp"
#endif

using namespace std;
char* TrimWhitespaces(char *str)
{
    char *end;

    // trim leading space
    while(isspace(*str))
        str++;

    if(*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while(end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end+1) = '\0';

    return str;
}
bool isValidDate(char* s)
{
//debugMsg("Inside isValidDate",s,0);

    char cmonth[2],cday[2],cyear[4];
    bool result,leapyear;
    int year,month,day,x;
    leapyear=false;
    result = true;
    x=0;// x is for debugging



    if (isdigit(s[0]) && isdigit(s[1]) && s[2]=='/' &&
            isdigit(s[3]) && isdigit(s[4])&& s[5]=='/' &&
            isdigit(s[6]) && isdigit(s[7]) && isdigit(s[8]) && isdigit(s[9]))
    {
        strncpy(cmonth, s, 2);

        month = atoi(cmonth);

        strncpy(cday, s+3, 2);
        day=atoi(cday);

        strncpy(cyear, s+6, 4);
        year=atoi(cyear);




        if ((year%4)==0)
            leapyear=true;

        if (month<0 || month>12)
        {
            x=1;
            result=false;
        }
        else
        {
            if (day<0 || day>31)
            {
                result=false;
                x=2;
            }
            else
            {
                if ((month==4||month==6||month==9|| month==11) && day>30)
                {
                    result=false;
                    x=3;
                }
                else
                {
                    if (leapyear==false)
                    {
                        if (month==2 && day>28)
                        {
                            result=false;
                            x=4;
                        }
                    }
                }

            }

        }
    }
    else
    {
        result=false;
        x=5;
    }


    return result;
}
void debugMsg(char *string1,char *string2,int val)
{

    wmove(stdscr,20,2);
    clrtoeol();
    mvwprintw(stdscr,20, 2, "%s, %s val=%d", string1, string2,val);
    refresh();
    getch();
}
void errMsg(WINDOW *win,int row,int col,char *msg)
{
    int outrow,outcol,width;
    width=79;
    beep();


    if(win == NULL)
    {
        debugMsg("win == NULL","",0);
        win = stdscr;
    }

    wmove(win,row,col-1);

    wclrtoeol(win);


    FindMiddle(win, row, col, outrow,outcol,width, msg);

    //wattron(win, color);
    attron(A_BLINK);
    mvwprintw(win, outrow, outcol, "%s", msg);
    //PrintInMiddle(win,row,col,79,msg, COLOR_PAIR(1));

    //box(win,0,0);
    attroff(A_BLINK);
    wrefresh(win);
    getch();
    wmove(win,col,row);
    wclrtoeol(win);
    //box(win,0,0);
    wrefresh(win);
    refresh();

}
string getInput( WINDOW *win, int startrow, int startcol, int width,int inputsize, char *prompt, chtype color)
{


    echo();

    wmove(win,startrow,startcol);
    //wclrtoeol(win);
    int outrow;
    int length,outcol,inputcol;
    char answer[10];

    FindMiddle(win, startrow, startcol, outrow,outcol,width, prompt);

    length = strlen(prompt);

    inputcol = outcol + length;
    string spaces50(50,32);
    mvwprintw(win, outrow, startcol, "%s", spaces50.c_str());
    //wattron(win, color);
    mvwprintw(win, outrow, outcol, "%s", prompt);
    //wattroff(win, color);
    box(win,0,0);
    wrefresh(win);
    wmove(win,outrow,inputcol+1);
    wgetstr(win,answer); //ncurses command but doesn't have ability to limit size
    //getnstr(answer, inputsize);// standard C command works
    mvwprintw(win, outrow, startcol+10, "%s", spaces50.c_str());
    noecho();

    return answer;
}
string askYNQuestion( WINDOW *win, int startrow, int startcol, int width, char *prompt, chtype color)
{

restart:
    echo();


    char xstr[]= {' '};
    int input;

    wmove(win,startrow,startcol);
    wclrtoeol(win);
    int outrow,outcol;

    FindMiddle(win, startrow, startcol, outrow,outcol,width, prompt);

    //wattron(win, color);
    mvwprintw(win, outrow, outcol, "%s", prompt);
    //wattroff(win, color);
    box(win,0,0);
    wrefresh(win);

    input=getchar();

    wmove(win,outrow,outcol);
    wclrtoeol(win);
    refresh();




    if(input=='y' || input=='Y')
    {
        return "Yes";
    }
    else
    {

        return "No";
    }
}

bool  confirmAction( WINDOW *msgwin, WINDOW *fullwin, int startrow, int startcol, int width, std::string prompt, chtype color, int keyToPress)
{


    string keyMessage,message;
    int c,outrow,outcol;
    bool result=false;

    wmove(msgwin,startrow,startcol);
    wclrtoeol(msgwin);
    refresh();



    switch(keyToPress)
    {
    case KEY_F(4):
        keyMessage="Press F4 to ";
        break;
    case KEY_F(3):
        keyMessage="Press F3 to ";
        break;
    case KEY_F(5):
        keyMessage="Press F5 to ";
        break;
    case KEY_F(7):
        keyMessage="Press F7 to ";
        break;
    case KEY_F(8):
        keyMessage="Press F8 to ";
        break;
    case KEY_NPAGE:
        keyMessage="Press Page Down to ";
        break;

    }
    message=keyMessage;
    message.append(prompt);

    FindMiddle(msgwin, startrow, startcol, outrow,outcol,width, message.c_str());


    mvwprintw(msgwin, outrow, outcol, "%s", message.c_str());

    box(msgwin,0,0);

    wrefresh(msgwin);
    c=getch();
    wmove(msgwin,startrow,startcol);
    wclrtoeol(msgwin);
    refresh();

    if (c==keyToPress)
    {


        result=true;
    }
    else
    {
        if (msgwin != stdscr)
        {

            result=false;
        }

    }

    return result;

}

void getNumericInput( WINDOW *msgwin, WINDOW *fullwin, int startrow, int startcol, int len,int width, char *prompt, char *input, chtype color)
{

tryAgain:
    echo();

    int length,outrow,outcol;
    length=strlen(prompt);
    std::string sLine(len, '_');
    FindMiddle(msgwin, startrow, startcol, outrow,outcol,width, prompt);
    mvwprintw(msgwin, outrow, outcol, "%s", prompt);
    mvwprintw(msgwin, outrow, outcol+length, " %s", sLine.c_str());

    wmove(msgwin,outrow, outcol+length+1);
    box(msgwin,0,0);

    wrefresh(msgwin);


    bool reject=false;


    wgetnstr(msgwin,input,len);


    if (input==NULL)
        reject=true;

    for (int i=0; i<len; i++)
    {
        if ( isspace(input[i]==false) && isdigit(input[i])==false)
            reject=true;

    }
    if (atoi(input)==0)
        reject=true;

    if (reject==true)
    {
        errMsg(msgwin,2,2,"Invalid Entry");
        if (confirmAction(msgwin,fullwin,2,2,78,"Exit",COLOR_PAIR(1),KEY_F(7))==true)
        {
            input[0]='*'; // flag for exit
            input[1]='\0';
            goto done;

        }
        else
        {
            wmove(msgwin,outrow,outcol);
            wclrtoeol(msgwin);
            refresh();
            goto tryAgain;
        }
    }




done:
    wmove(msgwin,outrow,outcol);
    wclrtoeol(msgwin);
    refresh();







}

void PrintInMiddle(WINDOW *win, int startrow, int startcol, int width, char *msg, chtype color)
{
    int outrow=0,outcol=0;

    FindMiddle(win, startrow, startcol, outrow,outcol,width, msg);


    mvwprintw(win, outrow, outcol, "%s", msg);
    wrefresh(win);
    refresh();

}


void FindMiddle(WINDOW *win, int startrow, int startcol, int &outrow, int &outcol,int width, char *msg)
{
    int length=0, col=0, row=0;
    float temp=0;


    if(win == NULL)
        debugMsg("FindMiddle","NULL window passed",0);

    if(win == NULL)
        win = stdscr;
    //getyx(win, row, col);

    col = startcol;

    row = startrow;


    length = strlen(msg);

    temp = (width - length)/ 2;
    outcol = startcol + (int)temp;
    outrow=row;


}


