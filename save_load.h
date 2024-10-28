#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "raylib_print.h"
#include "include/sds.h"
void saveToText(sds);




void saveToText(sds strout) {
    // ONLY CALL ONCE!

    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        raylib_Debug("Failure to create file!", 100);
        fclose(file);
        return;
    }

    fputs(strout, file);
    
    fclose(file);
}


sds loadFromText() {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        raylib_Debug("Failure to create file!", 100);
    }

    sds strin = sdsnew("");

    sds strLine = sdsnew("strLine");

    while (fgets(strLine, sizeof(strLine), file) != NULL) {
        strin = sdscat(strin, strLine);
    }

    fclose(file);
    return strin;
}



#endif // SAVE_LOAD_H