#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#include "util.h"

#define MAX_READ 3000
#define MAX_INSTRUCTION 200 
#define MAX_THREAD 10

pthread_t threadIDs[MAX_THREAD];

struct code_block {
    char *code; // '\0' 結尾
    uint8_t *tape;
    uint8_t tapeShift;
};

void subRoutine(void *data)
{
    struct code_block code = *(struct code_block*)data;
    printf("tape shfit:%d",code.tapeShift);
    
    char current_char;
    uint8_t *ptr = (code.tape+code.tapeShift);
    for (int i = 0; (current_char = code.code[i]) != '\0'; ++i) {
//        printf("%c",current_char);
        
        switch (current_char) {
            case '>':
                ++ptr;
                break;

            case '<':
                --ptr;
                break;

            case '+':
                ++(*ptr);
                break;

            case '-':
                --(*ptr);

            case '.':
                putchar(*ptr);
                break;

            case ',':
                *ptr = getchar();
                break;

            case '[':
                if (!(*ptr)) {
                    int loop = 1;
                    while (loop > 0) {
                        current_char = code.code[++i];
                        if (current_char == ']')
                            --loop;
                        else if (current_char == '[')
                            ++loop;
                    }
                }
                break;
                    
                    
            case ']':
                if (*ptr) {
                    int loop = 1;
                    while (loop > 0) {
                        current_char = code.code[--i];
                        if (current_char == '[') 
                            --loop;
                        else if (current_char == ']')
                            ++loop;
                     }
                } 
        }

    } 



}

void kyinterpreter(const char *const input)
{

    uint8_t tape[MAX_READ] = {0};
    uint8_t *ptr = tape;
    char current_char;
   
    char *subRoutineCode = (char*)malloc(sizeof(char)*MAX_INSTRUCTION);
    uint8_t *subRoutineTape = (uint8_t*)malloc(sizeof(uint8_t)*MAX_READ);
    

    for (int i = 0 ; (current_char = input[i]) != '\0'; ++i) {
        switch(current_char) {
        
            case '+':
                ++(*ptr);
                break;

            case '-':
                --(*ptr);
                break;

            case '<':
                --ptr;
                break;

            case '>':
                ++ptr;
                break;

            case '[':
                
                if (!(*ptr)) {
                    int loop = 1;
                    while (loop>0) {
                        current_char = input[++i];
                        if (current_char == ']')
                            --loop;
                        else if (current_char == '[')
                            ++loop;
                    }
                }
                
                break;

            case ']':
                if (*ptr) {
                    int loop =1;
                    while (loop > 0) {
                        current_char = input[--i];
                        if (current_char == '[')
                            --loop;
                        else if (current_char == ']')
                            ++loop;
                    }
                }
                break;

            case '.':
               putchar(*ptr);
                break;
            
            case ',':
                *ptr = getchar();
                break;

            case '{':
                {
                    int codeSize = 0;
                    char tmpChar;
                    while ((tmpChar = input[i+(codeSize++)]) != '}') {
                    }              

                    memcpy(subRoutineCode,&input[i],codeSize);
                    subRoutineCode[codeSize] = '\0';
                    memcpy(subRoutineTape,tape,MAX_READ);
                
                    struct code_block *cbPtr = (struct code_block*)malloc(sizeof(struct code_block));                  
                    cbPtr->code = subRoutineCode;
                    cbPtr->tape = subRoutineTape;
                    cbPtr->tapeShift = (ptr-tape);
                    static int threadIndex = 0;
                    if (pthread_create(&threadIDs[threadIndex++],NULL,(void*)subRoutine,(void*)cbPtr)) {
                        perror("create thread fail");        
                    }


                }
                break;


            case '|':
                
                for (int i = 0; i < MAX_THREAD ; i++) {
                   pthread_t id = threadIDs[i];

                    if (id) {
                        pthread_join(id,NULL);
                    }
                }
        
                break;
            default:
                
                break;
        
        }
    }    

    printf("\n*** THE END ***\n");    
}


int main(int argc,char *argv[])
{

    if(argc < 2) {
        printf("Usage: ./interpreter <inputfile>\n");
        return 0;
    }


   
    char *file_contents = read_file(argv[1]);
    if (file_contents == NULL) err("couldn't open file");
    kyinterpreter(file_contents);

    free(file_contents);

    return 0;
}
