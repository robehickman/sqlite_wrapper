import sqliteWrapper.sqliteWrapper;
import sqliteWrapper.resultSet;

class test
{
    public static void main(String[] args)
    {
        sqliteWrapper wrapper = new sqliteWrapper("test2.sqlite");
        resultSet set = wrapper.execute("CREATE TABLE IF NOT EXISTS test (id integer primary key, d1 TEXT)");
        if(set.checkError() == 1)
            System.out.println(set.getErrorMsg());

        wrapper.execute("insert into test (d1) values ('400')");
        wrapper.execute("insert into test (d1) values ('some pointless text')");

        set = wrapper.execute("select * from test");
        if(set.checkError() == 1)
            System.out.println(set.getErrorMsg());


        System.out.println("---");

        for(int i = 0; i <set.getRowCount(); i ++)
        {
            System.out.println(set.getValue("d1", i));
        }

        System.out.println("---");

        wrapper.delete();
    }

}
