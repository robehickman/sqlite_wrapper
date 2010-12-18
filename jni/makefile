main:
	gcc -I ../src -I /usr/lib/jvm/java-6-openjdk/include/ \
		-L ../src/ \
		-shared -o libsqliteWrapper.so src/sqlite_wrapper_jni.c -lsqlite_wrapper

	$(MAKE) --directory=java/
	

clean:
	rm -Rf libsqliteWrapper.so
	$(MAKE) --directory=java/ clean

