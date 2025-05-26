#pragma once
#include <windows.h>
#include <winuser.h>

bool isCtrlHeld();
bool isCtrlHeldLongEnough();
bool isEscapePressed(WPARAM wParam, KBDLLHOOKSTRUCT *kb);
bool isOverlayTriggered(WPARAM wParam, KBDLLHOOKSTRUCT *kb);
