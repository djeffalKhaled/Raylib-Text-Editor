#include "raylib.h" // CREDIT TO RAY
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/sds.c"
#include "include/sds.h" // CREDIT TO ANTIREZ, YOU SAVED ME
#include "include/sdsalloc.h"
#include "await.h"
#include "screen.h"
#include "save_load.h"

// Self libraries:
#include "strings.h"
#include "raylib_print.h"



int main(void) {
    // INITS
    int screenWidth = 1500;
    int screenHeight = 800;
    int maxUserText = 40;

    Rectangle saveRect = {(screenWidth - 200), (screenHeight - 100), 200, 100};
    Rectangle loadRect = {(screenWidth - 450), (screenHeight - 100), 200, 100};

    sds userText = sdsnew(""); 
    Vector2 textPos = {150, 50};
    
    InitWindow(screenWidth, screenHeight, "Raylib IDE");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        handleScreenSize(screenWidth, screenHeight);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (GuiButton(saveRect, "Save")) {
            saveToText(userText);
        }
        if (GuiButton(loadRect, "Load")) {
            if (loadFromText() != NULL) userText = loadFromText();
        }
        if (loadFromText() != NULL) raylib_Debug(loadFromText(), 50);


        Rectangle textEditorRect = {0, 0, screenWidth, (screenHeight - 200)};
        GuiDrawRectangle(textEditorRect, 10, BLUE, WHITE);
        raylib_Debug(intToStr(sdslen(userText)), 0);

        int width = MeasureText(userText, 50);
        Vector2 vect2 = {width + 50, 50};
        raylib_textCursor(vect2);

        
        
       

        if (sdslen(userText) < maxUserText) {
            // main writing script
            char charUserText = GetCharPressed();
            raylib_Debug(charToString(charUserText), screenWidth - 100);
            userText = sdscat(userText, charToString(charUserText));
        } else {
            userText = sdscat(userText, "\n");
            maxUserText += 40;
        }

        
        
        

        if (GetKeyPressed() == KEY_ENTER) {
            userText = sdscat(userText, "\n");
        }   

        if (IsKeyDown(KEY_RIGHT_SHIFT)) {
            delay(100);
            sdsrange(userText, 0, strlen(userText) - 2); // removes the last char using substring
            if (sdslen(userText) == 1) userText = sdsnew("");
        } 
        
        raylib_println(userText, textPos);

        

        
        


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
