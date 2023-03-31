// Operating Systems (ITP30002-02, Spring 2023)
// Homework 2 - CIMIN (Crashing Input Minimizer)

// Authors: 22100600 이현서 <hslee@handong.ac.kr>
//          21800030 구현우 <21800030@handong.ac.kr>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CHAR_ARR 1024

char* testPath      = NULL;     // -i
char* testFile      = NULL;     // Read from testPath and store the contents of the file.
short start = 0;                  // Start index of the test file.
short end = 0;                    // End index of the test file.

char* crashStr      = NULL;     // -m
char* resultPath    = NULL;     // -o
char* execFile      = NULL;     // executable file

void help();                    // Print the help message.

int main(int argc, char* argv[]) {
    // Debug mode
    #ifdef DEBUG
    printf("[CIMIN] DEBUG MODE\n");
    printf("argc: %d\n", argc);
    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    printf("\n");
    #endif
    
    // Argument parsing
    if(argc < 2) { // No arguments
        help();
        return 0;
    }
    
    // If there is an argument, parse it using getopt().
    int opt;
    while((opt = getopt(argc, argv, "hi:m:o:")) != -1) {
        switch(opt) {
            case 'h':
                help();
                break;
            case 'i':
                testPath = optarg;
                break;
            case 'm':
                crashStr = optarg;
                break;
            case 'o':
                resultPath = optarg;
                break;
            default:
                help();
                return 0;
        }
    }

    if(optind < argc) {
        execFile = argv[optind];
    }
    else {
        help();
        return 0;
    }

    #ifdef DEBUG
    printf("[CIMIN] Parsed arguments:\n");
    printf("testPath: %s\n", testPath);
    printf("crashStr: %s\n", crashStr);
    printf("resultPath: %s\n", resultPath);
    printf("execFile: %s\n", execFile);
    printf("\n");
    #endif

    // Check if the arguments are valid.
    if(testPath == NULL || crashStr == NULL || resultPath == NULL || execFile == NULL) {
        printf("[CIMIN] Error: Invalid arguments.\n");
        help();
        return 0;
    }

    // Get the whole contents of the test file.
    FILE* fp = fopen(testPath, "r");
    if(fp == NULL) {
        printf("[CIMIN] Error: Failed to open the test file.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    end = ftell(fp);        // Get the size of the test file.
    fseek(fp, 0, SEEK_SET);
    
    if(end > MAX_CHAR_ARR) { // If the size of the test file is larger than 4096 bytes:
        printf("[CIMIN] Error: The size of the test file is too large.\n");
        return 0;
    }
    testFile = malloc(end + 1);
    fread(testFile, end, 1, fp);
    fclose(fp);

    #ifdef DEBUG
    printf("[CIMIN] testFile: %s\nContents:\n%s\n", testPath, testFile);
    printf("start index: %d\n", start);
    printf("end index: %d\n", end);
    printf("\n");
    #endif

    // TODO: Implement the main logic of the program.

    return 0;
}

void help() {
    printf("Usage: cimin [options] [executable file]\n");
    printf("\nOptions:\n");
    printf("  -h\t\t\tPrint this help message\n");
    printf("  -i <file>\t\tfile path of the crashing input.\n");
    printf("  -m <string>\t\ta string whose appearance in standard error determines whether the expected crash occurs or not.\n");
    printf("  -o <file>\t\ta new file path to store the reduced crashing input.\n");
    printf("\nExecutable file:\n");
    printf("  path of the executable file that causes the crash.\n");
}