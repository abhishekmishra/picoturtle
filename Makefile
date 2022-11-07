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
ifeq ($(OSFLAG),WIN32)
	pemk genbuild
else
	pemk genbuild -DCMAKE_TOOLCHAIN_FILE=${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake -DCMAKE_INSTALL_PREFIX=./install
endif

delbuild:
	pemk delbuild 

build:
	pemk build

run:
# conditionals should be written without indentation
ifeq ($(OSFLAG),WIN32)
	./build/bin/Debug/picoturtle -g
else ifeq ($(OSFLAG),OSX)
	open -n ./build/bin/picoturtle.app --args -g
else
	./build/bin/picoturtle -g
endif

clean:
	pemk clean

install:
	cmake --build ./build --target install
