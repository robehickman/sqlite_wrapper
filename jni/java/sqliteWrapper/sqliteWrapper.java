package sqliteWrapper;

import sqliteWrapper.resultSet;

public class sqliteWrapper
{
    private long cPointer = 0;

    protected void finalize() {
        delete();
    }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    public sqliteWrapper(String dbFile)
    {
        cPointer = JniInit(dbFile);

        if(JniCheckInitFail(cPointer) == 1)
            throw new NullPointerException();
    }

    public resultSet execute(String sql)
    {
        if(JniCheckInitFail(cPointer) == 1)
            throw new NullPointerException();

        long cResultSet = JniExecute(cPointer, sql);
        return new resultSet(cResultSet);
    }

    public void setBinary(String table_name, String target_col,
        String id_col_name, String row_id, byte[] binary)
    {
        if(JniCheckInitFail(cPointer) == 1)
            throw new NullPointerException();

        JniSetBinary(cPointer, table_name, target_col,
            id_col_name, row_id, binary);
    }

    public byte[] getBinary(String table_name, String target_col,
        String id_col_name, String row_id)
    {
        if(JniCheckInitFail(cPointer) == 1)
            throw new NullPointerException();

        return JniGetBinary(cPointer, table_name, target_col,
            id_col_name, row_id);
    }

    public void delete()
    {
        if(cPointer != 0)
        {
            JniDelete(cPointer);
        }
    }

    public native long JniInit(String dbFile);
    public native int  JniCheckInitFail(long jdb);
    public native long JniExecute(long jdb, String sql);
    public native long JniSetBinary(long jdb, String table_name,
        String target_col, String id_col_name, String row_id, byte[] binary);
    public native byte[] JniGetBinary(long jdb, String table_name,
        String target_col, String id_col_name, String row_id);
    public native void JniDelete(long jdb);

    static {
        System.loadLibrary("sqliteWrapper");
    }
}
