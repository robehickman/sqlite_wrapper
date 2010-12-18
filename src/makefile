all: sqlite_wrapper.a

sqlite_wrapper.a: sqlite_wrapper.o sqlite3.o
	ar rcs libsqlite_wrapper.a sqlite3.o sqlite_wrapper.o

sqlite_wrapper.o: sqlite_wrapper.c
	gcc -c sqlite_wrapper.c

sqlite3.o: sqlite3.c
	gcc -c sqlite3.c

clean:
	rm -rf *o libsqlite_wrapper.a

