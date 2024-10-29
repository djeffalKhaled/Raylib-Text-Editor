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

    Rectangle saveRect = {0, 0, 200, 50};
    Rectangle loadRect = {200, 0, 200, 50};
    Rectangle newRect = {400, 0, 200, 50};
    Rectangle emptyRect = {600, 0, 200, 50};
    Rectangle prevRect = {800, 0, 200, 50};
    Rectangle nextRect = {1000, 0, 200, 50};



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

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Raylib Text-Editor", screenWidth - 250, 10, 25, GRAY);

        Rectangle textEditorRect = {0, 50, screenWidth, (screenHeight - 50)};
        Rectangle textArea = {50, 100, screenWidth - 50, (screenHeight - 50) - 50};

        // Settings
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


        
        GuiDrawRectangle(textEditorRect, 10, GRAY, WHITE);
        

        // Text Details Section
        DrawText(intToStr(page), screenWidth - 50, screenHeight - 50, 40, LIGHTGRAY);
        DrawText(intToStr(sdslen(userText)), 50, screenHeight - 50, 40, LIGHTGRAY);

        // Attempt at a text cursor
        //int width = MeasureText(userText, 50);
        //Vector2 vect2 = {width, 50};
        //raylib_textCursor(vect2);

        
        // main writing script
        char charUserText = GetCharPressed();
        raylib_Debug(charToString(charUserText), screenWidth - 100);
        userText = sdscat(userText, charToString(charUserText));


        
        DrawTextBoxed(GetFontDefault(), userText, textArea, 40, 10, true, BLACK);
        //DrawTextBoxed(GetFontDefault(), cursor, textArea, 50, 20, true, RED);
        
        if (GetKeyPressed() == KEY_ENTER) {
            userText = sdscat(userText, "\n");
        }   

        if (IsKeyDown(KEY_RIGHT_SHIFT)) {
            delay(100);
            sdsrange(userText, 0, strlen(userText) - 2); // removes the last char using substring
            if (sdslen(userText) == 1) userText = sdsnew("");
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
