.PHONY: all genbuild delbuild build run clean install

all: clean build run

genbuild:
	pemk genbuild

delbuild:
	pemk delbuild

build:
	pemk build

run:
# conditionals should be written without indentation
ifeq ($(OS),Windows_NT)
	./build/bin/Debug/picoturtle
else
	./build/bin/picoturtle
endif

clean:
	pemk clean

install:
	cmake --build ./build --target install