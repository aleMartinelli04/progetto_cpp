bin/main.exe: build/main.o
	mkdir -p bin/
	g++ build/main.o -o bin/main.exe

build/main.o: main.cpp
	mkdir -p build/
	g++ -c main.cpp -o build/main.o

.PHONY: exec
exec: bin/main.exe
	./bin/main.exe

.PHONY: clean
clean:
	rm -r bin/
	rm -r build/

.PHONY: doc
doc:
	doxygen

.PHONY: valgrind
valgrind: bin/main.exe
	valgrind bin/main.exe
