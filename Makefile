SRC_DIR := ./src
OBJ_DIR := ./obj
BUILD_DIR := ./build
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -lboost_system -lboost_filesystem -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lsqlite3 -lpthread
CPPFLAGS := -std=c++17

$(BUILD_DIR)/punkpin: $(OBJ_FILES)
	g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<
