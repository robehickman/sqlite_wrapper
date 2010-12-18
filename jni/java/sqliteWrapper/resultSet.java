package sqliteWrapper;

public class resultSet
{
    private long cPointer = 0;

    protected void finalize() {
        delete();
    }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    public resultSet(long cPointer)
    {
        this.cPointer = cPointer;
    }

    public String getValue(String col_name, int row_num)
    {
        return JniGetValue(cPointer, col_name, row_num);
    }

    public int getRowCount()
    {
        return JniGetRowCount(cPointer);
    }

    public int getColCount()
    {
        return JniGetColCount(cPointer);
    }

    public int checkError()
    {
        return JniCheckError(cPointer);
    }

    public String getErrorMsg()
    {
        return JniGetErrorMsg(cPointer);
    }

    public void delete()
    {
        JniDelete(cPointer);
    }

    public native String JniGetValue(long jresult_set, String jcol_name, int row_num);
    public native int    JniGetRowCount(long jresult_set);
    public native int    JniGetColCount(long jresult_set);
    public native int    JniCheckError(long jresult_set);
    public native String JniGetErrorMsg(long jresult_set);
    public native void   JniDelete(long jresult_set);
}
