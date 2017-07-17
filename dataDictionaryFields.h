#include <mysql++.h>
#include <ssqls.h>

//
//#if !defined(EXPAND_MY_SSQLS_STATICS)
//#   define MYSQLPP_SSQLS_NO_STATICS
//#endif





// The following is calling a very complex macro which will create
// struct "dataDictionaryFields", which has the member variables
// plus methods to help populate the class from a MySQL row.  See the
// SSQLS sections in the user manual for further details.


sql_create_5(dataDictionaryFields,
             1, 5, // The meaning of these values is covered in the user manual
             mysqlpp::sql_int, id,
             mysqlpp::sql_char, FieldId,
             mysqlpp::sql_char, TableId,
             mysqlpp::sql_int, idFieldsid,
              mysqlpp::sql_int, idTablesid

            );

