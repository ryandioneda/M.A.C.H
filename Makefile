# compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -mwindows -Iinclude
LDFLAGS := -lole32 -lshell32 -luuid

SRC_DIR := src
BUILD_DIR := build

SRCS := \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/hooks/hook.cpp \
    $(SRC_DIR)/overlay/overlayClass.cpp \
    $(SRC_DIR)/overlay/overlayControl.cpp \
    $(SRC_DIR)/overlay/overlayRender.cpp \
    $(SRC_DIR)/overlay/overlayState.cpp \
    $(SRC_DIR)/overlay/overlayWindow.cpp \
    $(SRC_DIR)/input/input.cpp \
    $(SRC_DIR)/config/configManager.cpp \
    $(SRC_DIR)/config/configPaths.cpp \
    $(SRC_DIR)/config/fileOps.cpp \


OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET := $(BUILD_DIR)/mach.exe

all: stop $(TARGET)

$(TARGET): $(OBJS)
	@powershell -Command "New-Item -ItemType Directory -Path '$(BUILD_DIR)' -Force" >nul 2>&1
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

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
