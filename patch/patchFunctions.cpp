#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "patchFunctions.h"

int changeAimFile (aimFile_t* aimFile, const char* patchFileName) {
    assert(aimFile);
    assert(patchFileName);

    FILE* patchFile = fopen (patchFileName, "r");
    if (!patchFile) {
        fprintf(stderr, "Error of opening file \"%s\"", patchFileName);
        perror("");
        return 1;
    }

    int newByte = 0;
    int offset = 0;

    while (1) {
        int res = fscanf(patchFile, "%d", &offset);
        if (res == EOF) break;
        if (res != 1) {
            printf("Invalid offset format\n");
            fclose(patchFile);
            return 1;
        }

        res = fscanf(patchFile, "%d", &newByte);
        if (res == EOF) {
            printf("Unexpected end of file after offset %d\n", offset);
            fclose(patchFile);
            return 1;
        }
        if (res != 1) {
            printf("Invalid newByte format after offset %d\n", offset);
            fclose(patchFile);
            return 1;
        }


        aimFile->bufferCopy[offset] = (char)newByte;
    }

    rewriteAimFile (aimFile);

    if (fclose (patchFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", patchFileName);
        perror("");
        return 1;
    }

    return 0;
}

int rewriteAimFile (aimFile_t* aimFile) {
    assert(aimFile);

    FILE* hackedAimFile = fopen ("result.com", "w");
    if (!hackedAimFile) {
        fprintf(stderr, "Error of opening file \"%s\"", aimFile->name);
        perror("");
        return 1;
    }

    fwrite (aimFile->bufferCopy, sizeof (char), aimFile->size, hackedAimFile);

    if (fclose (hackedAimFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", aimFile->name);
        perror("");
        return 1;
    }

    return 0;
}
