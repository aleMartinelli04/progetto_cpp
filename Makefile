PROJ_NAME = esempio

bin/$(PROJ_NAME).exe: build/main.o build/$(PROJ_NAME).o
	mkdir -p bin/
	g++ build/main.o build/$(PROJ_NAME).o -o bin/$(PROJ_NAME).exe

build/main.o: main.cpp
	mkdir -p build/
	g++ -c main.cpp -o build/main.o

build/$(PROJ_NAME).o: $(PROJ_NAME).cpp
	mkdir -p build/
	g++ -c $(PROJ_NAME).cpp -o build/$(PROJ_NAME).o

.PHONY: exec
exec: bin/$(PROJ_NAME).exe
	./bin/$(PROJ_NAME).exe

.PHONY: clean
clean:
	rm -r bin/
	rn -r build/

.PHONY: doc
doc:
	doxygen