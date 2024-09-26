# 컴파일러 설정
CXX := g++
WX_CONFIG := wx-config

# 컴파일러 및 링커 플래그 설정
CXXFLAGS := -std=c++11 -g $(shell $(WX_CONFIG) --cxxflags)
LDFLAGS := $(shell $(WX_CONFIG) --libs)

# 디렉토리 및 파일 설정
SRC_DIR := src
OBJ_DIR := obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# 변수 값 출력 (디버깅용)
$(info SRC_FILES = $(SRC_FILES))
$(info OBJ_FILES = $(OBJ_FILES))

TARGET := app

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
