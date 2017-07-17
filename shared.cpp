
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
//        debugMsg("month","",month);
//        debugMsg("day","",day);
//        debugMsg("year","",year);



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
//    debugMsg("x","",x);
//    debugMsg("result","",result);

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

void PrintInMiddle(WINDOW *win, int startrow, int startcol, int width, char *msg, chtype color)
{
    int outrow,outcol;

    FindMiddle(win, startrow, startcol, outrow,outcol,width, msg);
    mvwprintw(win, outrow, outcol, "%s", msg);
    refresh();

}


void FindMiddle(WINDOW *win, int startrow, int startcol, int &outrow, int &outcol,int width, char *msg)
{
    int length, col, row;
    float temp;

    if(win == NULL)
        debugMsg("FindMiddle","NULL window passed",0);

    if(win == NULL)
        win = stdscr;
    getyx(win, row, col);
    if(startcol != 0)
        col = startcol;
    if(startrow != 0)
        row = startrow;
    if(width == 0)
        width = 80;

    length = strlen(msg);

    temp = (width - length)/ 2;
    outcol = startcol + (int)temp;
    outrow=row;


}

bool execSQLAction(std::string sql, WINDOW *winSqlMsgArea)
{
//    debugMsg("inside","execSQLAction",0);
//    wclear(winSqlMsgArea);
//    box(winSqlMsgArea,0,0);
//    mvwprintw(winSqlMsgArea, 7, 2, "%s",sql.c_str());
//    mvwprintw(winSqlMsgArea, 21, 2, "%s","Press Enter");
//    wrefresh(winSqlMsgArea);
//    refresh();
//    getch();


    sqlite3 *db;
    char *zErrMsg = 0;
    int rc=0,result=0;
    bool Result=false;
    char* csql;
    char msg[100];

    //debugMsg("sql",sql.c_str(),0);
    csql=sql.c_str();

    rc = sqlite3_open("devTool.db", &db);

       if( rc ){


         sprintf(msg, "Can't open database: %s\n", sqlite3_errmsg(db));
         sqlErr(msg,sql,winSqlMsgArea);
       }else{
    rc = sqlite3_exec(db, csql, 0,0, &zErrMsg);

   if( rc != SQLITE_OK ){
        sprintf(msg,"SQL error: %s\n", zErrMsg);

        sqlite3_free(zErrMsg);
        Result=false;
   }
   else
        Result=true;
}



   sqlite3_close(db);

    return Result;
}

std::vector<dataDictionary>  GetDictionaryFields(std::string lookupId,WINDOW *winSqlMsgArea)
{

    std::vector<dataDictionary> dataDictionaryResults;

     sqlite3 *db;
     sqlite3_stmt *stmt;
     int rc = 0;
     int col, cols,rows=0;
     string sql;
     char * csql;
     char msg[100];
     dataDictionaryResults.clear();

    sql="SELECT * ";
    sql.append (" FROM dataDictionary WHERE tableId = ");
    sql.append ("'");
    sql.append (lookupId);
    sql.append("'");


    //debugMsg(sql.c_str(),"",99
    csql=sql.c_str();
    // open the database file
       rc = sqlite3_open("devTool.db", &db);
    if (rc ){
         sprintf(msg, "Can't open database: %s\n", sqlite3_errmsg(db));
         sqlErr(msg,sql,winSqlMsgArea);
       }else{

         rc = sqlite3_prepare_v2(db, csql, -1, &stmt, 0);
         if( rc ){
           sprintf(msg, "SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
           sqlErr(msg,sql,winSqlMsgArea);;
         }else{
           cols = sqlite3_column_count(stmt);
           // execute the statement

           do{

            dataDictionary Dictionary;
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
                        Dictionary.id=atoi(txt);

                        break;
                    case 1:
                        Dictionary.fieldId=txt;

                        break;
                    case 2:
                        Dictionary.tableId=txt;

                        break;
                    case 3:
                        Dictionary.idFieldsid=atoi(txt);

                        break;
                    case 4:
                        Dictionary.labelCol=atoi(txt);

                        break;
                    case 5:
                        Dictionary.idTablesid=atoi(txt);

                        break;


                   }



                 }
                 dataDictionaryResults.push_back(Dictionary);
                 break;
               default:
                 sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
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
      //return rc!=SQLITE_DONE;

    return dataDictionaryResults;


}
std::vector<fields>  GetField(std::string lookupId,WINDOW *winSqlMsgArea)
{

    std::vector<fields> fieldsResults;

     sqlite3 *db;
     sqlite3_stmt *stmt;
     int rc = 0;
     int col, cols,rows=0;
     string sql;
     char * csql;
     char msg[100];

    fieldsResults.clear();

    sql="SELECT * ";
    sql.append (" FROM fields WHERE FieldId = ");
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
           sprintf(msg, "SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
           sqlErr(msg,sql,winSqlMsgArea);
         }else{
           cols = sqlite3_column_count(stmt);
           // execute the statement

           do{

            fields field;
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
                        field.idFieldsid=atoi(txt);
                        break;

                    case 1:
                        field.fieldId=txt;
                        break;
                    case 2:
                        field.Name=txt;
                        break;

                    case 3:
                        field.Type=atoi(txt);
                        break;

                    case 4:
                        field.Length=atoi(txt);
                        break;
                    case 5:
                        field.FieldPrecision=atoi(txt);
                        break;
                    case 6:
                        field.isPrimaryKey=txt;
                        break;
                    case 7:
                        field.isRequired=txt;
                        break;
                    case 8:
                        field.EmumValues=txt;
                        break;

                   }



                 }
                 fieldsResults.push_back(field);
                 break;
               default:
                 sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
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
      //return rc!=SQLITE_DONE;
    return fieldsResults;


}

std::vector<tables>  GetTable(std::string lookupId,WINDOW *winSqlMsgArea)
{


        std::vector<tables> tablesResults;

        int rows=0;
        sqlite3 *db;
        sqlite3_stmt *stmt;
        int rc = 0;
        int col, cols;
        string sql;
        char* csql;
        char msg[100];

        tablesResults.clear();

        sql="SELECT * ";
        sql.append (" FROM tables WHERE tableId = ");
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
           sprintf(msg, "SQL error: %d : %s\n", rc, sqlite3_errmsg(db));
           sqlErr(msg,sql,winSqlMsgArea);
         }else{
           cols = sqlite3_column_count(stmt);
           // execute the statement

           do{

            tables table;
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
                 sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                 sqlErr(msg,sql,winSqlMsgArea);
                 break;
             }
           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         }
       }
      //return rc!=SQLITE_DONE;


    return tablesResults;

}

int GetAllFields(std::vector<fields> &FieldsResults, WINDOW *winSqlMsgArea)
   {

     sqlite3 *db;
     sqlite3_stmt *stmt;
     int rc = 0;
     int col, cols, rows=0;
     string sql;
     char * csql;
     char msg[100];



     sql="SELECT idFieldsid,FieldId,Name,Type,Length,FieldPrecision,isPrimaryKey,isRequired,EmumValues from fields";
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

           do{
            fields field;
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
                        field.idFieldsid=atoi(txt);
                        break;
                    case 1:
                        field.fieldId=txt;
                        break;
                    case 2:
                        field.Name=txt;
                        break;
                    case 3:
                        field.Type=atoi(txt);
                        break;
                    case 4:
                        field.Length=atoi(txt);
                        break;
                    case 5:
                        field.FieldPrecision=atoi(txt);
                        break;
                    case 6:
                        field.isPrimaryKey=txt;
                        break;
                    case 7:
                        field.isRequired=txt;
                        break;

                    case 8:
                        field.EmumValues=txt;
                        break;
                   default:
                     sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
                     sqlErr(msg,sql,winSqlMsgArea);
                     break;
             } // switch

             } //for
             FieldsResults.push_back(field);
             } // switch

           }while( rc==SQLITE_ROW );
           // finalize the statement to release resources
           sqlite3_finalize(stmt);
         } //else
         // close the database file
         sqlite3_close(db);
       } //else

       return rows;
}


int sqlErr(char errMsg, std::string sql,WINDOW *winSqlMsgArea)
{

            wclear(winSqlMsgArea);
            box(winSqlMsgArea,0,0);
            PrintInMiddle(winSqlMsgArea,2,2,70,"SQL ERROR - Exception Error",COLOR_PAIR(1));
            mvwprintw(winSqlMsgArea, 2, 2, "%s",errMsg);
            mvwprintw(winSqlMsgArea, 5, 2, "%s","______________________________________________________________________");
            //mvwprintw(winSqlMsgArea, 6, 2, "%s",sql.c_str());
            mvwprintw(winSqlMsgArea, 21, 2, "%s","Press Enter");
            wrefresh(winSqlMsgArea);
            refresh();
            getch();


}

bool GetScreenEntryField(std::vector<dbScreenFields> &screenEntryFieldsResults,string ScreenLayoutId, string ScreenLocId,WINDOW *winSqlMsgArea)
{

         int rows=0;
         sqlite3 *db;
         sqlite3_stmt *stmt;
         int rc = 0;
         int col, cols;
         string sql;
         char * csql;
         char msg[100];




        sql= "SELECT * FROM screenFields WHERE ScreenLayoutId = ";
        sql.append ("'");
        sql.append (ScreenLayoutId);
        sql.append("'");
        sql.append(" AND ScreenLocId = ");
        sql.append ("'");
        sql.append(ScreenLocId);
        sql.append("'");
        //debugMsg("sql",sql.c_str(),0);
        csql=sql.c_str();

         screenEntryFieldsResults.clear();

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

            dbScreenFields fields;
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
                        fields.idscreenFields=atoi(txt);
                        break;
                    case 1:
                        fields.ScreenLayoutId=txt;
                        break;
                    case 2:
                        fields.ScreenLocId=txt;
                        break;
                    case 3:
                        fields.tableId=txt;
                        break;
                    case 4:
                        fields.fieldId=txt;
                        break;
                    case 5:
                        fields.fieldName=txt;
                        break;
                    case 6:
                        fields.labelText=txt;
                        break;
                    case 7:
                        fields.labelRow=atoi(txt);
                        break;
                    case 8:
                        fields.labelCol=atoi(txt);
                        break;
                    case 9:
                        fields.fieldType=atoi(txt);
                        break;
                    case 10:
                        fields.fieldRow=atoi(txt);
                        break;
                     case 11:
                        fields.fieldCol=atoi(txt);
                        break;
                    case 12:
                        fields.fieldLen=atoi(txt);
                        break;
                    case 13:
                        fields.fieldPrec=atoi(txt);
                        break;
                    case 14:
                        fields.fieldEditable=txt;
                        break;
                    case 15:
                        fields.fieldRequired=txt;
                        break;
                    case 16:
                        fields.fieldVisible=txt;
                        break;
                    case 17:
                        fields.fieldTabOrder=atoi(txt);
                        break;
                    case 18:
                        fields.fieldEmumValues=txt;
                        break;

                   }



                 }
                 screenEntryFieldsResults.push_back(fields);
                 break;
               default:
                 sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
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


if(rows>0)

    return false; // there is a row so its not a new field
else
    return true; // its a new field

}

int GetAllScreenEntryFields(std::vector<dbScreenFields> &screenEntryFieldsResults,string ScreenLayoutId, WINDOW *winSqlMsgArea)
{

     int rows=0;
     sqlite3 *db;
     sqlite3_stmt *stmt;
     int rc = 0;
     int col, cols;
     string sql;
     char * csql;
     char msg[100];




        sql= "SELECT * FROM screenFields WHERE ScreenLayoutId = ";
        sql.append ("'");
        sql.append (ScreenLayoutId);
        sql.append("'");
        sql.append(" ORDER BY ScreenLocId");
        //debugMsg("sql",sql.c_str(),0);
        csql=sql.c_str();

         screenEntryFieldsResults.clear();

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

           do{

            dbScreenFields fields;
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
                        fields.idscreenFields=atoi(txt);
                        break;
                    case 1:
                        fields.ScreenLayoutId=txt;
                        break;
                    case 2:
                        fields.ScreenLocId=txt;
                        break;
                    case 3:
                        fields.tableId=txt;
                        break;
                    case 4:
                        fields.fieldId=txt;
                        break;
                    case 5:
                        fields.fieldName=txt;
                        break;
                    case 6:
                        fields.labelText=txt;
                        break;
                    case 7:
                        fields.labelRow=atoi(txt);
                        break;
                    case 8:
                        fields.labelCol=atoi(txt);
                        break;
                    case 9:
                        fields.fieldType=atoi(txt);
                        break;
                    case 10:
                        fields.fieldRow=atoi(txt);
                        break;
                     case 11:
                        fields.fieldCol=atoi(txt);
                        break;
                    case 12:
                        fields.fieldLen=atoi(txt);
                        break;
                    case 13:
                        fields.fieldPrec=atoi(txt);
                        break;
                    case 14:
                        fields.fieldEditable=txt;
                        break;
                    case 15:
                        fields.fieldRequired=txt;
                        break;
                    case 16:
                        fields.fieldVisible=txt;
                        break;
                    case 17:
                        fields.fieldTabOrder=atoi(txt);
                        break;
                    case 18:
                        fields.fieldEmumValues=txt;
                        break;

                   }



                 }
                 screenEntryFieldsResults.push_back(fields);
                 break;
               default:
                 sprintf(msg, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
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

bool GetScreenText(std::vector<dbScreenText> &screenTextsResults,string ScreenLayoutId, string ScreenLocId,WINDOW *winSqlMsgArea)
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
        sql.append(" AND ScreenLocId = ");
        sql.append ("'");
        sql.append(ScreenLocId);
        sql.append("'");


        //debugMsg("sql",sql.c_str(),0);

        screenTextsResults.clear();

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
                 screenTextsResults.push_back(texts);
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



    if(rows>0)

        return false; // there is a row so its not a new field
    else
        return true; // its a new field

}

void zapWins(WINDOW *winFullScreen,WINDOW *winMsgArea,WINDOW *winSqlMsgArea)
{



/*
This is important: You should delete a window’s subwindows before
you can delete the main window. If you don’t, the memory used by the
subwindow will not be released in your program, and other, various ugly and
unpredictable errors may result.

*/
    delwin(winFullScreen);
    delwin(winMsgArea);
    delwin(winSqlMsgArea);

}
