#compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -mwindows

#target and source
TARGET := macro_key_tool.exe
SRC := main.cpp

#default target
all: stop build

#kills any prev instances of program
stop:
	@powershell -Command "if (Get-Process -Name macro_key_tool -ErrorAction SilentlyContinue) { Stop-Process -Name macro_key_tool -Force; Write-Host 'Stopped macro_key_tool.exe' }"

#build executable
build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
	@echo "Build complete: $(TARGET)"

#run app detached from terminal
run: all
	@cmd /C start "" $(TARGET)

clean:
	del /Q $(TARGET)
