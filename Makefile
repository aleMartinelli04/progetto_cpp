bin/main.exe: build/main.o build/tests.o build/point.o
	mkdir -p bin/
	g++ build/main.o build/tests.o build/point.o -o bin/main.exe

build/main.o: main.cpp
	mkdir -p build/
	g++ -c main.cpp -o build/main.o

build/tests.o: tests.cpp
	mkdir -p build/
	g++ -c tests.cpp -o build/tests.o

build/point.o: point.cpp
	mkdir -p build/
	g++ -c point.cpp -o build/point.o

.PHONY: exec
exec: bin/main.exe
	./bin/main.exe

.PHONY: clean
clean:
	rm -rf bin/
	rm -rf build/

.PHONY: cleanall
cleanall:
	rm -rf bin/
	rm -rf build/
	rm -rf html/
	rm -rf *.txt

.PHONY: doc
doc:
	doxygen

.PHONY: valgrind
valgrind: bin/main.exe
	valgrind bin/main.exe
