#compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -mwindows

SRC_DIR := src
BUILD_DIR := build

SRCS := \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/hooks/hook.cpp \
    $(SRC_DIR)/overlay/overlay.cpp \
    $(SRC_DIR)/input/input.cpp

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET := $(BUILD_DIR)/macro_key_tool.exe


all: stop $(TARGET)

#link all object files into final binary
$(TARGET): $(OBJS)
	@powershell -Command "New-Item -ItemType Directory -Path '$(BUILD_DIR)' -Force" >nul 2>&1
	$(CXX) $(CXXFLAGS) -o $@ $^

#compile .cpp into .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@powershell -Command "New-Item -ItemType Directory -Path '$(dir $@)' -Force" >nul 2>&1
	$(CXX) $(CXXFLAGS) -c $< -o $@

stop:
	@powershell -Command "if (Get-Process -Name macro_key_tool -ErrorAction SilentlyContinue) { Stop-Process -Name macro_key_tool -Force; Write-Host 'Stopped running instance.' }"

run: all
	@cmd /C start "" $(TARGET)

clean:
	@powershell -Command "if (Get-Process -Name macro_key_tool -ErrorAction SilentlyContinue) { Stop-Process -Name macro_key_tool -Force }"
	@if exist build rmdir /S /Q build
