#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // Only for getch() function.

char *compression(char *text);

char *deCompression(char *text);

unsigned int getCompressedLength(const char *text, size_t length);

unsigned int getDeCompressedLength(const char *text, size_t length);

int main(int argc, char *argv[]) {

    if (argc == 0) {
        printf("You runs this program without arguments\n");
        printf("Try this: \n\t-i <input path>\n\t-o <output path>\n\t-b //binary files\n\t-d //decompression");
        getch();
        return 0;
    }
    int isBinary = 0, isCompressed = 0,
            inputPathIndex = 0, outputPathIndex = 0;

    for (int i = 0; i < argc; i++) {

        /*
            I wanna use switch because I have 4 more types of arguments.
            But switch in C cannot be used with type char*(string).
         */
        if (strcmp(argv[i], "-b") == 0) {
            isBinary = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            inputPathIndex = ++i;
        } else if (strcmp(argv[i], "-o") == 0) {
            outputPathIndex = ++i;
        } else if (strcmp(argv[i], "-d") == 0) {
            isCompressed = 1;
        } else {
            printf("\nBad argument %s\nPlease try again better.", argv[i]);
            getch();
            return 1;
        }
    }

    if (inputPathIndex == 0 || outputPathIndex == 0) {
        printf("\nYou need to add input and output files!");
        getch();
        return 1;
    }

    char *outputData = NULL;

    if (isBinary == 0) {
        FILE *inputFile = fopen(argv[inputPathIndex], "r");
        fseek(inputFile, 0, SEEK_END);
        size_t fileLength = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        char *fileData = (char *) malloc(sizeof(char) * fileLength + 1);
        fileData[fileLength] = '\0';
        fread(fileData, sizeof(fileData), fileLength, inputFile);

        fclose(inputFile);

        outputData = (isCompressed == 0 ? compression(fileData) : deCompression(fileData));

        free(fileData);

    } else {
        /*
            Complete this else branch to finish project.
         */
        FILE *inputFile = NULL;
        if (isCompressed == 0) {
            inputFile = fopen(argv[inputPathIndex], "rb");
            fseek(inputFile, 0, SEEK_END);
            size_t fileLength = ftell(inputFile);
            fseek(inputFile, 0, SEEK_SET);

            char *fileData = (char *) malloc(sizeof(char) * fileLength + 1);
            fileData[fileLength] = '\0';
            fread(fileData, sizeof(fileData), fileLength, inputFile);

            fclose(inputFile);

        } else {
            inputFile = fopen(argv[inputPathIndex], "r");
            fseek(inputFile, 0, SEEK_END);
            size_t fileLength = ftell(inputFile);
            fseek(inputFile, 0, SEEK_SET);

            char *fileData = (char *) malloc(sizeof(char) * fileLength + 1);
            fileData[fileLength] = '\0';
            fread(fileData, sizeof(fileData), fileLength, inputFile);

            fclose(inputFile);
        }

    }

    FILE *outputFile = fopen(argv[outputPathIndex], "w+");
    fprintf(outputFile, "%s", outputData);
    fclose(outputFile);

    getch();
    return 0;
}

char *compression(char *text) {
    unsigned length = strlen(text),
            compressedLength = getCompressedLength(text, length);

    char *compressionText = (char *) malloc((compressedLength + 1) * sizeof(char));
    compressionText[compressedLength] = '\0';

    char prev = ' ';
    int counter = 0, idx = 0;
    for (int i = 0; i < length; i++) {
        if (i == length - 1) {
            counter++;
            char helper[3];
            sprintf(helper, "%d%c", counter, prev);
            for (int j = 0; j < 2; j++)
                compressionText[idx + j] = helper[j];

        } else if (text[i] == prev) {
            counter++;

            if (counter == 9) {
                char helper[3];
                sprintf(helper, "%d%c", counter, prev);
                for (int j = 0; j < 2; j++) {
                    compressionText[idx + j] = helper[j];
                }
                idx += 2;
                counter = 0;
            }

        } else {
            if (counter > 0) {
                char helper[3];
                sprintf(helper, "%d%c", counter, prev);
                for (int j = 0; j < 2; j++) {
                    compressionText[idx + j] = helper[j];
                }
                idx += 2;
                counter = 0;
            }
            counter++;
            prev = text[i];
        }
    }
    return compressionText;
}

char *deCompression(char *text) {
    unsigned int length = strlen(text),
            deCompressedLength = getDeCompressedLength(text, length);

    char *compressionText = (char *) malloc((deCompressedLength + 1) * sizeof(char));
    compressionText[deCompressedLength] = '\0';

    int idx = 0;
    for (int i = 0; i < length; i += 2) {
        int iterations = text[i] - '0';
        char character = text[i + 1];
        for (int j = 0; j < iterations; j++) {
            compressionText[idx] = character;
            idx++;
        }
    }
    return compressionText;
}

unsigned int getCompressedLength(const char *text, size_t length) {
    unsigned int compressedLength = 0, counter = 0;

    char prev = ' ';
    for (int i = 0; i < length; i++) {
        if (i == length - 1) {
            counter++;
            compressedLength += 2;

        } else if (text[i] == prev) {
            counter++;

            if (counter == 9) {
                compressedLength += 2;
                counter = 0;
            }

        } else {
            if (counter > 0) {
                compressedLength += 2;
                counter = 0;
            }
            counter++;
            prev = text[i];
        }
    }
    return compressedLength;
}

unsigned int getDeCompressedLength(const char *text, size_t length) {
    unsigned int deCompressedLength = 0;

    for (int i = 0; i < length; i += 2) {
        deCompressedLength += (text[i] - '0');
    }
    return deCompressedLength;
}