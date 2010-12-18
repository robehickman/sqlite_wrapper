main:
	javac test.java
	$(MAKE) --directory=sqliteWrapper/ 

run:
	java -Djava.library.path=../ test

clean:
	rm -Rf *.class
	$(MAKE) --directory=sqliteWrapper/ clean
