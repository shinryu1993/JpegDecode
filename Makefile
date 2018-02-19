USE_OPENCV3 := 0

COMPILER := g++
EXEC := TestJpegDecode

SRC_DIR := ./src
OBJ_DIR := ./obj
SRCS := $(wildcard $(addprefix $(SRC_DIR)/, *.cpp))
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
CXXFLAGS = -std=c++11 -O2 -Wall -DOPENCV
INCLUDE_DIR := -I./src/ -I/usr/local/include
LIB_DIR := -L/usr/local/lib
LIBS := -ljpeg -lopencv_core -lopencv_highgui
DEPS := $(wildcard src/*.h) Makefile
ifeq ($(USE_OPENCV3), 1)
    LIBS += -lopencv_imgcodecs
endif

all: obj $(EXEC)

$(EXEC): $(OBJS)
	$(COMPILER) -o $@ $^ $(INCLUDE_DIR) $(LIB_DIR) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(COMPILER) -o $@ -c $< $(CXXFLAGS) $(INCLUDE_DIR)

obj:
	@mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(EXEC)
