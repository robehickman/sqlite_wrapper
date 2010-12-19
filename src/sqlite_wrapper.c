/*
Convenience wrapper for the SqLite database.
*/

#include <stdlib.h>
#include <string.h>

#include "sqlite_wrapper.h"

int rc;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static sw_result_set *sw_result_set__new()
{
    sw_result_set *result_set = malloc(16); 

    if(result_set == 0)
        return 0;

    result_set->error_msg  = 0;
    result_set->num_rows   = 0;
    result_set->num_cols   = 0;
    result_set->result_set = 0;

    return result_set;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static int sw_result_set__append_row(sw_result_set *result_set, char **row)
{
    result_set->num_rows ++;

// reallocate outer array
    result_set->result_set = realloc
        (result_set->result_set, sizeof(char*) * result_set->num_rows);

// allocate memory for new row
    result_set->result_set[result_set->num_rows - 1] =
        malloc(sizeof(char*) * result_set->num_cols);

    int i;
    for(i = 0; i < result_set->num_cols; i ++)
    {
        result_set->result_set[result_set->num_rows - 1][i] =
            malloc(strlen(row[i]) + 1);

        strcpy(result_set->result_set[result_set->num_rows - 1][i], row[i]);
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char * sw_result_set__get_value(sw_result_set *result_set,
    char *col_name, int row_num)
{
    if(row_num > result_set->num_rows - 2 || row_num < 0)
        return 0;

    int col_index = -1;
    int i;
    for(i = 0; i < result_set->num_cols; i++)
    {
        if(!strcmp(result_set->result_set[0][i], col_name))
        {
            col_index = i;
            break;
        }
    }

    if(col_index == -1)
        return 0;

    return result_set->result_set[row_num + 1][col_index];
} 

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int sw_result_set__get_row_count(sw_result_set *result_set)
{
    if(result_set->num_rows == 0)
        return 0;
    else
        return result_set->num_rows - 1;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int sw_result_set__get_col_count(sw_result_set *result_set)
{
    return result_set->num_cols;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void sw_result_set__set_col_count(sw_result_set *result_set, int num_cols)
{
    result_set->num_cols = num_cols;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int sw_result_set__check_error(sw_result_set *result_set)
{
    if(result_set == 0)
        return 1;

    if(result_set->error_msg != 0)
        return 1;

    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char *sw_result_set__get_error_msg(sw_result_set *result_set)
{
    return result_set->error_msg;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void sw_result_set__delete(sw_result_set *result_set)
{
    if(result_set->error_msg != 0)
    {
        free(result_set->error_msg);
        result_set->error_msg = 0;
    }

    if(result_set->num_rows > 0)
    {
        if(result_set->num_cols > 0)
        {
            int row, col;
            for(row = 0; row < result_set->num_rows; row++)
            {
                for(col = 0; col < result_set->num_cols; col ++)
                    free(result_set->result_set[row][col]);

                free(result_set->result_set[row]);
            }
        }

        free(result_set->result_set);
    }

    free(result_set);
}


/*___________________________________________________________________
End of result set object
*/


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int sqlite_wrapper__init(sqlite3 **db, char *file_name)
{
    if(file_name == NULL)
        return 1;

    rc = sqlite3_open(file_name, db);

    if(rc != SQLITE_OK)
        return 1;

    return 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static int callback(void *result_set, int argc,
    char **argv, char **azColName)
{
    int i;
    char **row;
    row = malloc(sizeof(char*) * argc);

    if(row == 0)
        return 1;

    if(sw_result_set__get_col_count(result_set) == 0)
    {
        sw_result_set__set_col_count(result_set, argc);

        for(i=0; i<argc; i++)
            row[i] = azColName[i];
        
        sw_result_set__append_row(result_set, row);
    }

    for(i=0; i<argc; i++)
    {
        if(argv[i] == NULL)
            row[i] = "";
        else
            row[i] = argv[i];
    }

    sw_result_set__append_row(result_set, row);

    free(row);

    return 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sw_result_set *sqlite_wrapper__execute(sqlite3 *db, char *sql)
{
    char *zErrMsg = 0;

    sw_result_set *result_set = sw_result_set__new();

    if(result_set == 0)
        return 0;

    rc = sqlite3_exec(db, sql, callback, result_set, &zErrMsg);

    if(rc != SQLITE_OK)
    {
        result_set->error_msg = malloc(strlen(zErrMsg) + 1);
        strcpy(result_set->error_msg, zErrMsg);

        sqlite3_free(zErrMsg);
    }

    return result_set;
}

