#include "raylib.h" // CREDIT TO RAY
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raydraw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/sds.c"
#include "include/sds.h" // CREDIT TO ANTIREZ, YOU SAVED ME
#include "include/sdsalloc.h"
#include "include/await.h"

// Self libraries:
#include "include/strings.h"
#include "include/raylib_print.h"
#include "include/save_load.h"
#include "include/screen.h"



int main(void) {
    // INITS
    int initScreenWidth = 1500;
    int initScreenHeight = 800;
    int scrollSpeed = 15;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Rectangle saveRect = {0, 0, 200, 50};
    Rectangle loadRect = {200, 0, 200, 50};
    Rectangle newRect = {400, 0, 200, 50};
    Rectangle emptyRect = {600, 0, 200, 50};
    Rectangle prevRect = {800, 0, 200, 50};
    Rectangle nextRect = {1000, 0, 200, 50};

    Rectangle textArea = {50, 100, initScreenWidth - 50, (initScreenHeight - 50) - 50};


    // Writing
    sds userText = sdsnew(""); 
    sds cursor = sdsnew("");
    int pagesLength = 10; // maximum pages
    sds *texts = (sds*)malloc(sizeof(sds) * pagesLength);
    texts = initStringArray(texts, pagesLength); // init to empty
    int pageIndex = 0;
    int page = 1;

    
    InitWindow(initScreenWidth, initScreenHeight, "Raylib IDE");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        handleScreenSize(initScreenWidth, initScreenHeight);

        Vector2 screen = getScreenSize(initScreenWidth, initScreenHeight);
        int screenWidth = screen.x;
        int screenHeight = screen.y;

        // Update sizes for fullscreen resizing
        Rectangle textEditorRect = {0, 50, screenWidth, (screenHeight - 50)};
        // allows text scroll
        textArea = setScrollable(textArea, scrollSpeed); 
        textArea.width = screenWidth - 50; // doesn't support height reposition yet


        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Raylib Text-Editor", screenWidth - 250, 10, 25, GRAY);

        // Attempt at a text cursor
        //int width = MeasureText(userText, 50);
        //Vector2 vect2 = {width, 50};
        //raylib_textCursor(vect2);

        
        // main writing script
        char charUserText = GetCharPressed();
        raylib_Debug(charToString(charUserText), screenWidth - 100);
        userText = sdscat(userText, charToString(charUserText));

        GuiDrawRectangle(textEditorRect, 10, GRAY, WHITE); // textArea background
        cursor = appendCursorAtmp1(sdslen(userText));
        DrawTextBoxed(GetFontDefault(), userText, textArea, 40, 10, true, BLACK);
        DrawTextBoxed(GetFontDefault(), cursor, textArea, 40, 10, true, RED);

        // Text Details Section
        DrawText(intToStr(page), screenWidth - 50, screenHeight - 50, 40, LIGHTGRAY);
        DrawText(intToStr(sdslen(userText)), 50, screenHeight - 50, 40, LIGHTGRAY);
        DrawText(intToStr(GetFPS()), screenWidth / 2, screenHeight - 50, 30, GREEN);
        
        if (GetKeyPressed() == KEY_ENTER) {
            userText = sdscat(userText, "\n");
        }   

        if (IsKeyDown(KEY_RIGHT_SHIFT)) {
            delay(100);
            sdsrange(userText, 0, strlen(userText) - 2); // removes the last char using substring
            if (sdslen(userText) == 1) userText = sdsnew("");
        }

        if (IsKeyDown(KEY_TAB)) {
            userText = sdscat(userText, "\t");
        }


        
        // Settings ---GUI---
        if (GuiButton(saveRect, "#02#Save")) {
            savePages(texts, pagesLength);
        }
        if (GuiButton(loadRect, "#04#Load")) {
            if (loadPages(texts, 5) != NULL) {
                texts = loadPages(texts, pagesLength);
                pageIndex = 0; page = 1;
                userText = texts[pageIndex];
            };
            printf("FILE LOAD: %s\n", userText);
        }

        if (GuiButton(newRect, "#08#New")) {
            // removes all pages content
            texts = initStringArray(texts, pagesLength);
            userText = sdsnew("");
            printf("TEXT: Init All pages\n");
        }
        if (GuiButton(emptyRect, "#143#Empty")) {
            // removes current page content
            userText = sdsnew("");
            texts[pageIndex] = sdsnew("");
            printf("TEXT: Init Current Page\n");
        }


        // Pages
        if (GuiButton(prevRect, "#118#Previous Pg")) {
            if (pageIndex > 0) {
                pageIndex--; page--;
                userText = texts[pageIndex];
                printf("PAGE INDEX: %d\n", pageIndex);
            }
        }
        if (GuiButton(nextRect, "#119#Next Pg")) {
            if (pageIndex < pagesLength) {
                texts[pageIndex] = userText;
                if (pageIndex != (pagesLength - 1)) {
                    pageIndex++; page++;
                    userText = texts[pageIndex];
                }
                printf("PAGE INDEX: %d\n", pageIndex);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
