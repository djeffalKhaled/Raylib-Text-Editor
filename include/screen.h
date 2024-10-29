// Handles all screen related functions
#ifndef SCREEN_H
#define SCREEN_H
#include "../raylib.h"

void centerWindow(int screenWidth, int screenHeight) {
    // credit to omegapy
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor); 
    int monitoHeight = GetMonitorHeight(monitor); 
    SetWindowPosition((int)(monitorWidth / 2) - (int)(screenWidth / 2), (int)(monitoHeight / 2) - (int)(screenHeight / 2));
}

void handleScreenSize(int screenWidth, int screenHeight) {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
 	{
        if (!IsWindowFullscreen()) {
            int monitor = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
            ToggleFullscreen();
        } else {
            ToggleFullscreen();
            centerWindow(screenWidth, screenHeight);
            SetWindowSize(screenWidth, screenHeight);
        }
    }
}


#endif // SCREEN_H