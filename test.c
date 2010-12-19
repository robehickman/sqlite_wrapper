#include <unistd.h>
#include <sqlite3.h>
#include <stdio.h>

#include "src/sqlite_wrapper.h"

sqlite3 *db;
int rc;

int main()
{
    sqlite_wrapper__init(&db, "test.sqlite");

    sw_result_set *result = sqlite_wrapper__execute(db,
        "select * from stocks where go_check = 1");

    if(sw_result_set__check_error(result) == 1)
        printf(sw_result_set__get_error_msg(result));
    else
    {
        printf(sw_result_set__get_value(result, "id", 0));
    }

    printf("\n");

    sw_result_set__delete(result);

    sqlite3_close(db);
}
