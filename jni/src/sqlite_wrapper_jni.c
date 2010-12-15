#include <jni.h>
#include <sqlite3.h>

#include "sqlite_wrapper.h"


/*___________________________________________________________________
Sqlite wrapper object
*/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jlong Java_SqliteWrapper_JniInit
    (JNIEnv* env, jobject obj, jstring dbFile)
{
    const jbyte *file_name = (*env)->GetStringUTFChars(env, dbFile, 0);
    sqlite3 *db = 0;
    jlong jdb   = 0 ;
    int rc;

    rc = sqlite_wrapper__init(&db, (char*) file_name);

    if(rc == 1)
        return 0;

    (*env)->ReleaseStringUTFChars(env, dbFile, file_name);

    *(sqlite3 **)&jdb = db; 

    return jdb;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jlong Java_SqliteWrapper_JniExecute
    (JNIEnv* env, jobject obj, jlong jdb, jstring sql)
{
    const jbyte *sql_string = (*env)->GetStringUTFChars(env, sql, 0);
    sqlite3 *db = *(sqlite3 **)&jdb; 
    jlong jresult_set   = 0 ;

    sw_result_set *result_set = sqlite_wrapper__execute(db, (char*) sql_string);

    (*env)->ReleaseStringUTFChars(env, sql, sql_string);
 
    *(sw_result_set **)&jresult_set = result_set; 

    return jresult_set;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Java_SqliteWrapper_JniDelete
    (JNIEnv* env, jobject obj, jlong jdb)
{
    sqlite3 *db = *(sqlite3 **)&jdb; 
    sqlite3_close(db);
}


/*___________________________________________________________________
Result set object
*/


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jstring Java_ResultSet_JniGetValue
    (JNIEnv* env, jobject obj, jlong jresult_set, jstring jcol_name, int row_num)
{
    const jbyte *col_name = (*env)->GetStringUTFChars(env, jcol_name, 0);
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 

    char *item = sw_result_set__get_value(result_set, (char*) col_name, row_num);

    (*env)->ReleaseStringUTFChars(env, jcol_name, col_name);

    return (*env)->NewStringUTF(env, item);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int Java_ResultSet_JniGetRowCount
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    return sw_result_set__get_row_count(result_set);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int Java_ResultSet_JniGetColCount
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    return sw_result_set__get_col_count(result_set);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int Java_ResultSet_JniCheckError
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    return sw_result_set__check_error(result_set);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
jstring Java_ResultSet_JniGetErrorMsg
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    char *msg = sw_result_set__get_error_msg(result_set);

    return (*env)->NewStringUTF(env, msg);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
void Java_ResultSet_JniDelete
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    sw_result_set__delete(result_set);
}

