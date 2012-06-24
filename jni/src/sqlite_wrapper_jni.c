#include <jni.h>
#include <sqlite3.h>
#include <stdio.h>

#include "sqlite_wrapper.h"

/*___________________________________________________________________
Sqlite wrapper object
*/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jlong Java_sqliteWrapper_sqliteWrapper_JniInit
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

    printf("opening\n");

    return jdb;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int Java_sqliteWrapper_sqliteWrapper_JniCheckInitFail
    (JNIEnv* env, jobject obj, jlong jdb)
{
    sqlite3 *db = *(sqlite3 **)&jdb; 

    if(db == 0)
        return 1;
    return 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jlong Java_sqliteWrapper_sqliteWrapper_JniExecute
    (JNIEnv* env, jobject obj, jlong jdb, jstring sql)
{
    const jbyte *sql_string = (*env)->GetStringUTFChars(env, sql, 0);
    sqlite3 *db = *(sqlite3 **)&jdb; 
    jlong jresult_set   = 0 ;

    sw_result_set *result_set = sqlite_wrapper__execute(db, (char*) sql_string);

    (*env)->ReleaseStringUTFChars(env, sql, sql_string);
 
    *(sw_result_set **)&jresult_set = result_set; 

    printf("running\n");

    return jresult_set;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Java_sqliteWrapper_sqliteWrapper_JniSetBinary
    (JNIEnv* env, jobject obj, jlong jdb, jstring jtable_name,
        jstring jtarget_col, jstring jid_col_name, jstring jrow_id, jbyteArray jbinary)
{
    sqlite3 *db = *(sqlite3 **)&jdb;
    const jbyte *table_name  = (*env)->GetStringUTFChars(env, jtable_name, 0);
    const jbyte *target_col  = (*env)->GetStringUTFChars(env, jtarget_col, 0);
    const jbyte *id_col_name = (*env)->GetStringUTFChars(env, jid_col_name, 0);
    const jbyte *row_id      = (*env)->GetStringUTFChars(env, jrow_id, 0);
    int  bin_len = (*env)->GetArrayLength(env, jbinary);
	char *binary = (*env)->GetByteArrayElements(env, jbinary, 0);

    const char *sql = "update \"%s\" set \"%s\" = ? where \"%s\" = \'%s\';";
    char *query = (char*) malloc(strlen(sql) + strlen(table_name) +
        strlen(target_col) + strlen(id_col_name) + strlen(row_id) + 1);
    sprintf(query, sql, table_name, target_col, id_col_name, row_id);

    int r;
    sqlite3_stmt *stmt;
    r = sqlite3_prepare(db, query, -1, &stmt, 0);
    r = sqlite3_bind_blob(stmt, 1, binary, bin_len, SQLITE_TRANSIENT);
    r = sqlite3_step(stmt);
    r = sqlite3_finalize(stmt);

    printf(sqlite3_errmsg(db));


    free(query);

    (*env)->ReleaseStringUTFChars(env, jtable_name, table_name);
    (*env)->ReleaseStringUTFChars(env, jtarget_col, target_col);
    (*env)->ReleaseStringUTFChars(env, jid_col_name, id_col_name);
    (*env)->ReleaseStringUTFChars(env, jrow_id, row_id);

    (*env)->ReleaseByteArrayElements(env, jbinary, binary, JNI_ABORT);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jbyteArray Java_sqliteWrapper_sqliteWrapper_JniGetBinary
    (JNIEnv* env, jobject obj, jlong jdb, jstring jtable_name,
        jstring jtarget_col, jstring jid_col_name, jstring jrow_id)
{
    sqlite3 *db = *(sqlite3 **)&jdb;
    const jbyte *table_name  = (*env)->GetStringUTFChars(env, jtable_name, 0);
    const jbyte *target_col  = (*env)->GetStringUTFChars(env, jtarget_col, 0);
    const jbyte *id_col_name = (*env)->GetStringUTFChars(env, jid_col_name, 0);
    const jbyte *row_id      = (*env)->GetStringUTFChars(env, jrow_id, 0);

    const char *sql = "select \"%s\" from \"%s\" where \"%s\" = \'%s\';";
    char *query = (char*) malloc(strlen(sql) + strlen(table_name) +
        strlen(target_col) + strlen(id_col_name) + strlen(row_id) + 1);
    sprintf(query, sql, target_col, table_name, id_col_name, row_id);


    int r;
    sqlite3_stmt *stmt;
    r=sqlite3_prepare(db,query, -1,&stmt,0);
    r=sqlite3_step(stmt);

    // No data / not binary 
    /*
    int nm=sqlite3_column_count(stmt);
    if(!(nm == 1 && sqlite3_column_type(stmt,0) == SQLITE_BLOB))
    {
    }
    */

    int   length = sqlite3_column_bytes(stmt,0);
    unsigned char *binary = (unsigned char *)sqlite3_column_blob(stmt,0);

    jbyteArray jbinary = (*env)->NewByteArray(env, length); 
    (*env)->SetByteArrayRegion(env, jbinary, 0, length, binary);

    sqlite3_finalize(stmt);

    (*env)->ReleaseStringUTFChars(env, jtable_name, table_name);
    (*env)->ReleaseStringUTFChars(env, jtarget_col, target_col);
    (*env)->ReleaseStringUTFChars(env, jid_col_name, id_col_name);
    (*env)->ReleaseStringUTFChars(env, jrow_id, row_id);

    return jbinary;

    /*
    r=sqlite3_prepare(sqlitedb,"SELECT * FROM wibble", -1,&stmt,0);
    r=sqlite3_step(stmt);
    int nm=sqlite3_column_count(stmt);
    while(r) {
    for (int i=0;i<nm;++i) {
    char *name=(char *)sqlite3_column_name(stmt,i);
    switch (sqlite3_column_type(stmt,i)) {
    case SQLITE_INTEGER:
    {
    int p=sqlite3_column_int(stmt,i);
    }
    break;
    case SQLITE_FLOAT:
    {
    double p=sqlite3_column_double(stmt,i);
    }
    break;
    case SQLITE_TEXT:
    {
    char *p=(char *)sqlite3_column_text(stmt,i);
    int qq=1;
    }
    break;
    case SQLITE_BLOB:
    {
    int ll=sqlite3_column_bytes(stmt,i);
    void *p=(void *)sqlite3_column_blob(stmt,i);
    int qq=1;
    }
    break;
    case SQLITE_NULL:
    break;
    }
    }
    r=sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    */
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Java_sqliteWrapper_sqliteWrapper_JniDelete
    (JNIEnv* env, jobject obj, jlong jdb)
{
    sqlite3 *db = *(sqlite3 **)&jdb; 
    sqlite3_close(db);

    printf("closing\n");
}


/*___________________________________________________________________
Result set object
*/


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
jstring Java_sqliteWrapper_resultSet_JniGetValue
    (JNIEnv* env, jobject obj, jlong jresult_set, jstring jcol_name, int row_num)
{
    const jbyte *col_name = (*env)->GetStringUTFChars(env, jcol_name, 0);
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 

    char *item = sw_result_set__get_value(result_set, (char*) col_name, row_num);

    (*env)->ReleaseStringUTFChars(env, jcol_name, col_name);

    return (*env)->NewStringUTF(env, item);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int Java_sqliteWrapper_resultSet_JniGetRowCount
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    return sw_result_set__get_row_count(result_set);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int Java_sqliteWrapper_resultSet_JniGetColCount
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    return sw_result_set__get_col_count(result_set);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int Java_sqliteWrapper_resultSet_JniCheckError
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    return sw_result_set__check_error(result_set);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
jstring Java_sqliteWrapper_resultSet_JniGetErrorMsg
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    char *msg = sw_result_set__get_error_msg(result_set);

    return (*env)->NewStringUTF(env, msg);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
void Java_sqliteWrapper_resultSet_JniDelete
    (JNIEnv* env, jobject obj, jlong jresult_set)
{
    sw_result_set *result_set = *(sw_result_set **)&jresult_set; 
    sw_result_set__delete(result_set);
}

