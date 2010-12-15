#include <unistd.h>
#include <sqlite3.h>
#include <stdio.h>

#include "sqlite_wrapper.h"

sqlite3 *db;
int rc;

int main()
{
    unlink("test.sqlite");

    sqlite_wrapper_setup(&db, "test.sqlite");

    sqlite3_exec(db,
        "CREATE TABLE table1 (id INTEGER PRIMARY KEY, data1 TEXT, data2 INTEGER);", 0, 0, 0);

    sqlite3_exec(db,
        "CREATE TABLE table2 (id INTEGER PRIMARY KEY, data1 TEXT, data2 INTEGER);", 0, 0, 0);

    sqlite3_exec(db,
        "INSERT INTO table1 (data1, data2) VALUES ('test', 10);", 0, 0, 0);

    sqlite3_exec(db,
        "INSERT INTO table1 (data1, data2) VALUES ('test 3', 1043);", 0, 0, 0);

    sw_result_set *result = sqlite_wrapper_execute(db, "SELECT * FROM table1;");

    if(sw_result_set__check_error(result) == 1)
        printf(sw_result_set__get_error_msg(result));

    printf(sw_result_set__get_value(result, "id", 0));
    printf("\n");

    sw_result_set__delete(result);

    sqlite3_close(db);
}
