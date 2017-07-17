#include <mysql++.h>
#include <ssqls.h>

//
//#if !defined(EXPAND_MY_SSQLS_STATICS)
//#   define MYSQLPP_SSQLS_NO_STATICS
//#endif





// The following is calling a very complex macro which will create
// struct "mysqlScreenFields", which has the member variables
// plus methods to help populate the class from a MySQL row.  See the
// SSQLS sections in the user manual for further details.


sql_create_13(myScreenFields,
             1, 13, // The meaning of these values is covered in the user manual
             mysqlpp::sql_int, idscreenFields,
             mysqlpp::sql_char, ScreenLayoutId,
             mysqlpp::sql_char, labelText,
             mysqlpp::sql_int, labelRow,
             mysqlpp::sql_int, labelCol,
             mysqlpp::sql_char, fieldName,
             mysqlpp::sql_int, fieldType,
             mysqlpp::sql_int, fieldRow,
             mysqlpp::sql_int, fieldCol,
             mysqlpp::sql_int, fieldLen,
             mysqlpp::sql_int, fieldPrec,
             mysqlpp::sql_char, fieldEditable,
             mysqlpp::sql_char, fieldRequired



            );

