#include "txLib.h"
#include <stdio.h>

#include "patchFunctions.h"
#include "fileFunctions.h"

int main (int argc, char* argv[]) {

if (argc != 3) return printf("Usage: %s aim.com changes.txt\n", argv[0]), 1;

    txPlaySound("nokia_3310.wav", SND_ASYNC);
    createWindow();
    runGif();

    aimFile_t aimFile = {};
    aimFile.name = argv[1];
    const char* patchFileName = argv[2];

    copyFileContent(&aimFile);
    changeAimFile (&aimFile, patchFileName);


    txPlaySound(NULL);
    return 0;
}
