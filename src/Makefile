CXX=g++
MKDIR=mkdir -p
RM=rm -rf
GDB=gdb

CXXFLAGS=-std=c++14 -O2
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

BUILD_DIR=build
SOURCE_FILES=Source.cpp kugla.cpp ivica.cpp
OBJECT_FILES=$(patsubst %,$(BUILD_DIR)/%,$(patsubst %.cpp,%.o,$(SOURCE_FILES)))

BINARY=bilijar.out

.PHONY: all run debug clean

all: $(BINARY)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BINARY): $(SOURCE_FILES)
	$(MKDIR) $(BUILD_DIR)
	$(MAKE) $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) $(OBJECT_FILES) -o $@ $(LDFLAGS)

run: $(BINARY)
	./$(BINARY)

debug: $(BINARY)
	$(GDB) $(BINARY)

clean:
	$(RM) $(BUILD_DIR) $(BINARY)
