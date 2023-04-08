// Operating Systems (ITP30002-02, Spring 2023)
// Homework 2 - CIMIN (Crashing Input Minimizer)

// Authors: 22100600 이현서 <hslee@handong.ac.kr>
//          21800030 구현우 <21800030@handong.ac.kr>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
    char* crashingInput;        // -i
    char* crashMessage;         // -m
    char* outputPath;           // -o
    char* minimizedOutput;      // Minimized output.
    char* program;              // executable file
    char* programOptions;       // options for the executable file
    char** execOptions;         // options for the executable file
    int isFirstRun;             // Is it the first run?
} Test;

Test test;

void help();
char* reader(char* path);
void writer(char* path);
void sigintHandler();
char* execute(char* input);
void minimize();
void reduce(char* t);

int main(int argc, char* argv[]) {

    #ifdef DEBUG
    printf("[CIMIN] DEBUG MODE\n");
    printf("argc: %d\n", argc);
    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    printf("\n");
    #endif

    signal(SIGINT, sigintHandler);

    int option;
    while((option = getopt(argc, argv, "hi:m:o:")) != -1) {
        switch(option) {
            case 'h':
                help();
                exit(0);
            case 'i':
                test.crashingInput = reader(optarg);
                break;
            case 'm':
                test.crashMessage = optarg;
                break;
            case 'o':
                test.outputPath = optarg;
                break;
            default:
                help();
                exit(1);
        }
    }

    if(argc == 8) {
        test.program = argv[optind];

        test.execOptions = (char**)malloc(sizeof(char*) * 2);
        test.execOptions[0] = test.program;
        test.execOptions[1] = NULL;
    } else if(argc >= 9) {
        test.program = argv[optind];
        test.programOptions = argv[optind + 1];

        test.execOptions = (char**)malloc(sizeof(char*) * 2);
        test.execOptions[0] = test.program;
        test.execOptions[1] = NULL;
        
        int i = 1;
        char* token = strtok(test.programOptions, " ");
        while(token != NULL) {
            test.execOptions = (char**)realloc(test.execOptions, sizeof(char*) * (i + 2));
            test.execOptions[i] = token;
            test.execOptions[i + 1] = NULL;
            token = strtok(NULL, " ");
            i++;
        }
        test.execOptions[i] = NULL;

        #ifdef DEBUG
        printf("[CIMIN] Parsed options for the executable file:\n");
        for(int i = 0; test.execOptions[i] != NULL; i++) {
            printf("execOptions[%d]: %s\n", i, test.execOptions[i]);
        }
        #endif
    } else {
        help();
        exit(1);
    }

    #ifdef DEBUG
    printf("[CIMIN] Parsed arguments:\n");
    printf("crashingInput: \n%s\n", test.crashingInput);
    printf("crashMessage: %s\n", test.crashMessage);
    printf("outputPath: %s\n", test.outputPath);
    printf("program: %s\n", test.program);
    for(int i = 0; test.execOptions[i] != NULL; i++) {
        printf("execOptions[%d]: %s\n", i, test.execOptions[i]);
    }
    printf("\n");
    #endif

    printf("[CIMIN] Minimizing the crashing input. It takes a while.\n");
    minimize();

    printf("[CIMIN] Minimization complete.\n");

    writer(test.outputPath);
    printf("[CIMIN] The reduced crashing input is stored in %s.\n", test.outputPath);

    return 0;
}

void help() {
    printf("Usage: ./cimin [Options] [Executable file \"Options\"]\n");
    printf("\nOptions:\n");
    printf("  -h\t\t\t  Print this help message\n");
    printf("  -i <file>\t\t* file path of the crashing input.\n");
    printf("  -m <\"string\">\t\t* a string whose appearance in standard error determines whether the expected crash occurs or not.\n");
    printf("  -o <file>\t\t* a new file path to store the reduced crashing input.\n");
    printf("\nExecutable file:\n");
    printf("  * path of the executable file that causes the crash.\n");
    printf("    options for the executable file.\n");
    printf("\n * = Required\n");
}

char* reader(char* path) {
    char* string;

    FILE* file = fopen(path, "r");
    if(file == NULL) {
        printf("[CIMIN] Error: Failed to open the crashing input. (Invalid option: -i %s)\n", path);
        exit(1);
    }

    // Read the file and store it in a string.
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    string = (char*)malloc(sizeof(char) * (size + 1));
    fread(string, 1, size, file);
    string[size] = '\0';
    fclose(file);

    return string;
}

void writer(char* path) {
    FILE* file = fopen(path, "w+");
    if(file == NULL) {
        printf("[CIMIN] Error: Failed to open/create the output file. (Invalid option: -o %s)\n", path);
        exit(1);
    }

    fwrite(test.minimizedOutput, 1, strlen(test.minimizedOutput), file);
    fclose(file);
}

void sigintHandler() {
    printf("\n[CIMIN] Interrupted by the user.\n");
    if(test.isFirstRun) {
        printf("[CIMIN] The test is not started yet.\n");
        printf("[CIMIN] No minimized input is created. Exiting...\n");
    }
    else {
        printf("[CIMIN] The current minimized input is stored in %s.\n", test.outputPath);
        writer(test.outputPath);
        printf("[CIMIN] Exiting...\n");
    }
    exit(0);
}

char* execute(char* input) {
    char* output = (char*)malloc(sizeof(char) * 1024);

    int childFD[2], parentFD[2];
    pipe(childFD);
    pipe(parentFD);

    int pid = fork();
    if(pid == 0) {
        // Child process

        dup2(parentFD[0], STDIN_FILENO);
        dup2(childFD[1], STDOUT_FILENO);

        execv(test.program, test.execOptions);
    } else {
        // Parent process
        close(parentFD[0]);
        close(childFD[1]);
        write(parentFD[1], input, strlen(input));

        int status = waitpid(pid, &status, 0);

        if(test.isFirstRun) {
            sleep(3);
            if(status == 0) {
                kill(pid, SIGKILL);
                printf("[CIMIN] Error: The program does not crash within 3 seconds.\n");
                printf("[CIMIN]        This happend when the program has a infinite loop.\n");
                printf("[CIMIN]        Please check the program and try again.\n");
                printf("[CIMIN] Exiting...\n");
                exit(1);
            }
            test.isFirstRun = 0;
        } else {
            wait(NULL);
        }

        read(childFD[0], output, 1024);
    }
    return output;
}

void minimize() {
    test.isFirstRun = 1;
    reduce(test.crashingInput);
}

void reduce(char* t) {
    char *Tm = (char*)malloc(sizeof(char) * ((int)strlen(t) + 1));
    strcpy(Tm, t);
    int s = (int)strlen(Tm) - 1;
    while(s > 0) {
        for(int i = 0; i <= (int)strlen(Tm) - s; i++) {
            char* head = NULL, *tail = NULL;
            head = (char*)malloc(sizeof(char) * (i + 1));
            strncpy(head, Tm, i);
            head[i] = '\0';

            tail = (char*)malloc(sizeof(char) * ((int)strlen(Tm) - i - s + 1));
            strcpy(tail, Tm + (i + s));
            tail[(int)strlen(Tm) - i - s] = '\0';

            char* output = execute(strcat(head, tail));
            if(strstr(output, test.crashMessage) != NULL) {
                reduce(strcat(head, tail));
            }
            else {
                break;
            }
        }
        for(int i = 0; i <= (int)strlen(Tm) - s; i++) {
            char* middle = (char*)malloc(sizeof(char) * (s + 1));
            strncpy(middle, Tm + i, s);
            middle[s] = '\0';

            #ifdef DEBUG
            printf("Input:\n%s\n", middle);
            #endif

            char* output = execute(middle);
            if(strstr(output, test.crashMessage) != NULL) {
                reduce(middle);
            }
            else {
                break;
            }
        }
        s--;
    }
    test.minimizedOutput = Tm;
}