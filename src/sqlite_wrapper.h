/*
Convenience wrapper for the SqLite database.
*/

#ifndef SQLITE_WRAPPER
#define SQLITE_WRAPPER

#include "sqlite3.h"

/****************************************************************//**
* Result set structure, members should be considered private
********************************************************************/ 
typedef struct {
    char *error_msg;
    int  num_rows;
    int  num_cols;
    char ***result_set;
} sw_result_set;

/****************************************************************//**
* Get a value from the result set, returns null if col name does
not exist or row index is out of bounds. The pointers returned from
this function are freed by the delete method and must not be
freed manually.
********************************************************************/ 
char * sw_result_set__get_value(
    sw_result_set *result_set,
///<  Instance of the result set object
    char * col_name,
///< Name of the column to get
    int row_num
///< Number of the row to get
    ); 

/****************************************************************//**
* Get the number of rows in the result set
********************************************************************/ 
int sw_result_set__get_row_count(
    sw_result_set *result_set
///<  Instance of the result set object
    );

/****************************************************************//**
* Get the number of columns in the result set
********************************************************************/ 
int sw_result_set__get_col_count(
    sw_result_set *result_set
///<  Instance of the result set object
    );

/****************************************************************//**
* Returns 1 if an error occurred while running a query.
********************************************************************/ 
int sw_result_set__check_error(sw_result_set *result_set);

/****************************************************************//**
* Returns the SqLite error message. The pointer returned by this
function is managed and must not be freed manually.
********************************************************************/ 
char *sw_result_set__get_error_msg(sw_result_set *result_set);

/****************************************************************//**
 * Free all memory allocated by the result set object 
********************************************************************/ 
void sw_result_set__delete(
    sw_result_set *result_set
///<  Instance of the result set object
    );

/*
End of result set object
*/


/****************************************************************//**
 * Connect to sq lite database file, returns 1 on error.
********************************************************************/ 
int sqlite_wrapper__init(sqlite3 **db, char *file_name);

/****************************************************************//**
 * Execute a SQL statement and return a result set object with
the results.
********************************************************************/ 
sw_result_set *sqlite_wrapper__execute(sqlite3 *db, char *sql);

#endif
