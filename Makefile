# 컴파일러 설정
CXX := g++
WX_CONFIG := wx-config

# 디렉토리 및 파일 설정
SRC_DIR := src
OBJ_DIR := obj

# 컴파일러 및 링커 플래그 설정
CXXFLAGS := -std=c++11 -g $(shell $(WX_CONFIG) --cxxflags) -I$(SRC_DIR)
LDFLAGS := $(shell $(WX_CONFIG) --libs)

# 하위 디렉토리까지 포함하여 모든 .cpp 파일을 검색
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# 변수 값 출력 (디버깅용)
$(info SRC_FILES = $(SRC_FILES))
$(info OBJ_FILES = $(OBJ_FILES))

TARGET := app

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

# 각 .cpp 파일에 대해 개별 .o 파일 생성
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	find $(OBJ_DIR) -type f -name '*.o' -delete
