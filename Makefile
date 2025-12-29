bin/set.exe: build/main.o
	mkdir -p bin/
	g++ build/main.o -o bin/set.exe

build/main.o: main.cpp
	mkdir -p build/
	g++ -c main.cpp -o build/main.o

.PHONY: exec
exec: bin/set.exe
	./bin/set.exe

.PHONY: clean
clean:
	rm -r bin/
	rm -r build/

.PHONY: doc
doc:
	doxygen