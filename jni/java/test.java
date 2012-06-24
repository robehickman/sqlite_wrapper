import sqliteWrapper.sqliteWrapper;
import sqliteWrapper.resultSet;
import java.io.PrintStream;

class test
{
    public static void main(String[] args)
    {
        sqliteWrapper wrapper = new sqliteWrapper("test2.sqlite");
        resultSet set = wrapper.execute("CREATE TABLE IF NOT EXISTS test (id integer primary key, d1 TEXT, d2 BLOB)");
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

        byte binary[] = {'a','b','c','d','e','f','g','h','i','j'};
        wrapper.setBinary("test", "d2", "id", "1", binary);
        binary = wrapper.getBinary("test", "d2", "id", "1");
        System.out.println(new String(binary));

        System.out.println("---");

        wrapper.delete();
    }

}
