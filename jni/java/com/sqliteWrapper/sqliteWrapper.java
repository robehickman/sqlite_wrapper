package com.sqliteWrapper;

public class sqliteWrapper
{
    public static void main(String[] args)
    {
        sqliteWrapper wrapper = new sqliteWrapper("test2.sql");
        wrapper.execute("CREATE TABLE test (d1 TEXT)");
    }



    private long cPointer = 0;

    protected void finalize() {
        delete();
    }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    public sqliteWrapper(String dbFile)
    {
        cPointer = JniInit(dbFile);
// needs error checking
    }

    public void execute(String sql)
    {
        JniExecute(cPointer, sql);
    }

    public void delete()
    {
        JniDelete(cPointer);
    }

    public native long JniInit(String dbFile);
    public native long JniExecute(long jdb, String sql);
    public native void JniDelete(long jdb);

    static {
        System.loadLibrary("SqliteWrapper");
    }
}
