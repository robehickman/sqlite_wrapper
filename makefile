main: sqlite_wrapper.a

jni: sqlite_wrapper.a sqliteWrapper.so

clean:
	$(MAKE) --directory=src/ clean
	$(MAKE) --directory=jni/ clean

sqlite_wrapper.a:
	$(MAKE) --directory=src/

sqliteWrapper.so:
	$(MAKE) --directory=jni/
