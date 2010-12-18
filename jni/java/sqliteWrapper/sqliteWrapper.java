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
        long cResultSet = JniExecute(cPointer, sql);
        return new resultSet(cResultSet);
    }

    public void delete()
    {
        JniDelete(cPointer);
    }

    public native long JniInit(String dbFile);
    public native int  JniCheckInitFail(long jdb);
    public native long JniExecute(long jdb, String sql);
    public native void JniDelete(long jdb);

    static {
        System.loadLibrary("sqliteWrapper");
    }
}
