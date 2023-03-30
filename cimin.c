// Operating Systems (ITP30002-02, Spring 2023)
// Homework 2 - CIMIN (Crashing Input Minimizer)

// Authors: 22100600 이현서 <hslee@handong.ac.kr>
//          21800030 구현우 <21800030@handong.ac.kr>

#include <stdio.h>
#include <unistd.h>

void help();

int main(int argc, char* argv[]) {
    // Argument parsing
    if(argc < 2) {
        // If there is no argument, print help message.
        help();
        return 0;
    }
    else {
        // If there is an argument, parse it.
        char* testPath = NULL;      // -i
        char* crashStr = NULL;      // -m
        char* resultPath = NULL;    // -o
        char* execFile = NULL;      // executable file

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
                    break;
            }
        }
        execFile = argv[optind];
    }

    // TODO: Implement the main logic of the program.

    return 0;
}

void help() {
    printf("Usage: cimin [options] [executable file]\n");
    printf("Options:\n");
    printf("  -h\t\t\tPrint this help message\n");
    printf("  -i <file>\t\tfile path of the crashing input.\n");
    printf("  -m <string>\t\ta string whose appearance in standard error determines whether the expected crash occurs or not.\n");
    printf("  -o <file>\t\ta new file path to store the reduced crashing input.\n");
    printf("Executable file:\n");
    printf("  path of the executable file that causes the crash.\n");
}