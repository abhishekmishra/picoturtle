CC=g++
LIBS=`wx-config --libs`
CFLAGS=`wx-config --cflags`
MKDIR_P=mkdir -p
OUT_DIR=./bin/release
OBJ_DIR=./obj
SRC_DIR=./src

all:	directories pt

## see https://stackoverflow.com/questions/1950926/create-directories-using-make-file
## for creating output directories
directories: $(OUT_DIR) $(OBJ_DIR)

$(OUT_DIR):
	$(MKDIR_P) $(OUT_DIR)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

# see https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
# Get list of object files, with paths
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))

pt: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ./obj/*.o ./bin/release/*
	rm -f ./obj/*.o ./bin/debug/*
