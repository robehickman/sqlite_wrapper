main: sqlite_wrapper.a

test: sqlite_wrapper.a
	gcc -g test.c -Lsrc/ -lsqlite_wrapper -pthread -ldl -o test

jni: sqlite_wrapper.a sqliteWrapper.so

clean:
	$(MAKE) --directory=src/ clean
	$(MAKE) --directory=jni/ clean

sqlite_wrapper.a:
	$(MAKE) --directory=src/

sqliteWrapper.so:
	$(MAKE) --directory=jni/
