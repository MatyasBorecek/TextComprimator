#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // Only for getch() function.

char *compression(char *text);

char *deCompression(char *text);

unsigned int getCompressedLength(const char *text, size_t length);

unsigned int getDeCompressedLength(const char *text, size_t length);

char *binCompression(char *text);

char *binDeCompression(const int *text, int length);

int main(int argc, char *argv[]) {

    if (argc <= 1) {
        printf("You runs this program without arguments\n");
        printf("Try this: \n\t-i <input path>\n\t-o <output path>\n\t-b //binary files\n\t-d //decompression");
        getch();
        return 0;
    }
    int isBinary = 0, isCompressed = 0,
            inputPathIndex = 0, outputPathIndex = 0;

    for (int i = 1; i < argc; i++) {

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
        if (inputFile == NULL) {
            fprintf(stderr, "Can not open file '%s'", argv[inputPathIndex]);
            exit(1);
        }
        fseek(inputFile, 0, SEEK_END);
        size_t fileLength = ftell(inputFile);
        rewind(inputFile);

        char *fileData = (char *) malloc(sizeof(char) * fileLength + 1);
        fileData[fileLength] = '\0';
        fread(fileData, sizeof(fileData), fileLength, inputFile);
        fclose(inputFile);

        outputData = (isCompressed == 0 ? compression(fileData) : deCompression(fileData));

        free(fileData);

    } else {
        FILE *inputFile = NULL;
        if (isCompressed == 1) {
            inputFile = fopen((char *) argv[inputPathIndex], "rb");
            fseek(inputFile, 0, SEEK_END);
            size_t fileLength = ftell(inputFile);
            rewind(inputFile);

            char *fileData = (char *) malloc(sizeof(char) * fileLength + 1);
            fileData[fileLength] = '\0';
            fread(fileData, sizeof(fileData), fileLength, inputFile);
            fclose(inputFile);

            // This region of code is totally random, because I have problems with binary files.
            // So I came up with this solution because I don't know how better to do this.
            // I am ashamed because of it....
            int numberCounter = 1, multiplier = 1, idx = 0;
            for (int i = 0; i < fileLength; i++) {
                if (fileData[i] == ' ')
                    numberCounter++;
            }
            int *preData = (int *) calloc(numberCounter, sizeof(int));
            for (int i = 0; i < fileLength; i++) {
                if (fileData[i] != ' ') {
                    preData[idx] += (int) fileData[i];
                    if (multiplier > 1) {
                        preData[idx] += 6;
                    }
                    multiplier++;
                } else {
                    multiplier = 1;
                    idx++;
                }
            }

            for (int i = 0; i < numberCounter; i++) {
                preData[i] -= 48;
            }

            outputData = binDeCompression(preData, numberCounter);

        } else {
            inputFile = fopen((char *) argv[inputPathIndex], "r");
            fseek(inputFile, 0, SEEK_END);
            size_t fileLength = ftell(inputFile);
            fseek(inputFile, 0, SEEK_SET);

            char *fileData = (char *) malloc(sizeof(char) * fileLength + 1);
            fileData[fileLength] = '\0';
            fread(fileData, sizeof(fileData), fileLength, inputFile);

            fclose(inputFile);
        }

    }

    FILE *outputFile = fopen((char *) argv[outputPathIndex], "w+");
    fwrite(outputData, strlen(outputData) + 1, 1, outputFile);
    fclose(outputFile);

    getch();
    return 0;
}

char *binCompression(char *text) {
    unsigned length = strlen(text), deCompressedLength = 0;
    char prev = ' ';

    for (int i = 0; i < length; i++) {
        if (text[i] == ' ') {
            continue;
        }

        if (text[i] != prev) {
            prev = text[i];
            deCompressedLength += 2;
        }
    }


    return text;
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

char *binDeCompression(const int *text, int length) {
    unsigned int deCompressedLength = 0;

    for (int i = 0; i < length; i += 2) {
        deCompressedLength += text[i];
    }

    char *deCompressedText = (char *) malloc((deCompressedLength + 1) * sizeof(char));
    deCompressedText[deCompressedLength] = '\0';

    int compareMe = text[0], idx = 0;
    for (int i = 0; i < deCompressedLength; i++) {
        if (i < compareMe) {
            deCompressedText[i] = (char) text[idx + 1];
        } else {
            idx += 2;
            if ((i + 2) <= deCompressedLength) {
                compareMe += text[idx];
                deCompressedText[i] = (char) text[idx + 1];
            }
        }
    }
    return deCompressedText;
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