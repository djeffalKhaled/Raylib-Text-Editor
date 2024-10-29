#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H
#include <stdio.h>
#include <stdlib.h>
#include "../raylib.h"
#include "raylib_print.h"
#include "sds.h"




void saveToText(sds strout) {
    // ONLY CALL ONCE!

    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        printf("FILE IO: Failure to create file!\n");
        fclose(file);
        return;
    }

    fputs(strout, file);
    
    fclose(file);
}


sds loadFromText() {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        printf("FILE IO: Failure to create file!\n");
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