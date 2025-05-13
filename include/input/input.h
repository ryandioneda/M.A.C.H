#pragma once
#include <windows.h>

bool isCtrlHeld();
bool isCtrlHeldLongEnough();
bool isEscapePressed();
bool isSearchTrigger(WPARAM wParam, KBDLLHOOKSTRUCT *kb);
