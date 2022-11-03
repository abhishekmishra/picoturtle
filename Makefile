.PHONY: all genbuild delbuild build run clean install

# see https://gist.github.com/sighingnow/deee806603ec9274fd47
# for details on the following snipped to get the OS
# removed the flags about arch as it is not needed for now
OSFLAG :=
ifeq ($(OS),Windows_NT)
	OSFLAG = WIN32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG = LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG = OSX
	endif
endif

$(info OSFLAG is $(OSFLAG))

all: clean build run

genbuild:
	pemk genbuild

delbuild:
	pemk delbuild

build:
	pemk build

run:
# conditionals should be written without indentation
ifeq ($(OSFLAG),WIN32)
	./build/bin/Debug/picoturtle
else ifeq ($(OSFLAG),OSX)
	open -n ./build/bin/picoturtle.app
else
	./build/bin/picoturtle
endif

clean:
	pemk clean

install:
	cmake --build ./build --target install
