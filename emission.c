#include "emission.h"
#include "stack.h"
#include "node.h"

#define BUFFERLEN 2048
#define DEBUG

void getVariableInitialization(struct stack *s, char* emission){
    // Start by printing lines to initialize variables and temporary variables
    // Both start with a tab symbol to follow C conventions
    char vars[BUFFERLEN] = "\tint";
    char tmpVars[BUFFERLEN] = "\tint";
    
    struct node* pointer = s->bottom; // start at bottom of stack and move your way upwards
    bool firstVar = true; // flag for comma syntax
    bool firstTmpVar = true; // flag for comma syntax

    strcat(emission, "\t// Variable and temporary variable initialization\n");
    while(pointer->next != NULL){
        // check each nodes name, if it is Tmp...
        char name[BUFFERLEN];
        if(strstr(pointer->nodeName, "Tmp") != NULL){
            if(firstTmpVar){
                sprintf(name, " %s", pointer->nodeName);
                firstTmpVar = false;
            }else{
                sprintf(name, ", %s", pointer->nodeName);
            }
            strcat(tmpVars, name);
        }else{
            if(firstVar){
                sprintf(name, " %s", pointer->nodeName);
                firstVar = false;
            }else{
                sprintf(name, ", %s", pointer->nodeName);
            }
            strcat(vars, name);
        }

        // progress to next node
        pointer = pointer->next;
    }
    // add terminating semicolon
    strcat(vars, ";\n");
    strcat(tmpVars, ";\n");
    
    strcat(emission, vars);
    strcat(emission, tmpVars);
}

void getVariableInput(struct stack* s, char* emission, char* inputVariables){
    // TODO: ADD SUPPORT FOR USER DEFINED VARIABLES
    strcat(emission, "\n\t//Using printf and scanf to get user input values\n");
    // struct node* pointer = s->bottom;
    // while(pointer->next != NULL){
    //     char varBuffer[BUFFERLEN]; // buffer for variable names
    //     char printBuffer[BUFFERLEN]; // buffer for printf line
    //     char scanBuffer[BUFFERLEN]; // buffer for scanf line
    //     // checks for user variable instead of temporary variable
    //     if(strstr(pointer->nodeName, "Tmp") == NULL){
    //         sprintf(varBuffer, "%s", pointer->nodeName);

    //         // build print statement
    //         sprintf(printBuffer, "\n\tprintf(\"%s=\");\n", varBuffer);
            
    //         // build scanf statement
    //         sprintf(scanBuffer, "\tscanf(\"%%d\", &%s);\n", varBuffer);
    //         strcat(printBuffer, scanBuffer); // combine the print and scan lines together
    //         strcat(emission, printBuffer); // add to emission
    //     }
    //     pointer = pointer->next;
    // }
    char *delimited = strtok(inputVariables, " "); // get a list of variable names needed
    while(delimited){
        char printBuffer[BUFFERLEN]; // buffer for printf line
        char scanBuffer[BUFFERLEN]; // buffer for scanf line
        // build print statement
        sprintf(printBuffer, "\n\tprintf(\"%s=\");\n", delimited);
        
        // build scanf statement
        sprintf(scanBuffer, "\tscanf(\"%%d\", &%s);\n", delimited);
        strcat(printBuffer, scanBuffer); // combine the print and scan lines together
        strcat(emission, printBuffer); // add to emission
        delimited = strtok(NULL, " "); // move to next variable
    }
}

void getEquations(struct stack* s, char* emission){
    strcat(emission, "\n\t//Three-Address Code Representation\n");
    struct node* pointer = s->bottom;

    // TODO: Why is it adding a random extra closing bracket???
    int labelCount = 1; // track number of statements
    while(pointer->next != NULL){
        char buffer[BUFFERLEN];
        if(strstr(pointer->equation, "if(") == NULL){
            // Case for regular expression 
            sprintf(buffer, "\tS%d:\t%s%s", labelCount, pointer->nodeName, pointer->equation);
            labelCount++;
            printf("buffer:%s\n", buffer);
            strcat(emission, buffer);
        }else{
            // Case for if/else expression
            char ifElseBuffer[BUFFERLEN];
            sprintf(ifElseBuffer, "%s", pointer->equation);
            char *delimited = strtok(ifElseBuffer, "\n"); // splits expression up into elements seperated by newline
            while(delimited != NULL){
                printf("delimited:%s\n", delimited);
                if(strcmp(delimited, "}") == 0){
                    sprintf(buffer, "\t\t%s\n", delimited);
                }else{
                    sprintf(buffer, "\tS%d:\t%s\n", labelCount, delimited);
                    labelCount++;
                }
                strcat(emission, buffer); // add to buffer
                delimited = strtok(NULL, "\n"); // advance to next element
            }
        }
        
        pointer = pointer->next;
    }
}

void getPrintStatements(struct stack* s, char* emission){
    strcat(emission, "\n\t//Print final value of variables\n");
    struct node* pointer = s->bottom;

    // Check each node in stack. If the name is not for a temporary variable, create a print statement for it
    while(pointer->next != NULL){
        char varBuffer[BUFFERLEN]; // buffer for variable name
        char lineBuffer[BUFFERLEN]; // buffer for single line

        if(strstr(pointer->nodeName, "Tmp") == NULL){
            sprintf(varBuffer, "%s", pointer->nodeName);
            // build print statement
            sprintf(lineBuffer, "\tprintf(\"%s=%%d\\n\", %s);\n", varBuffer, varBuffer);
            strcat(emission, lineBuffer);
        }
        pointer = pointer->next;
    }
}

void printEmission(struct stack* s, char* inputVariables){
    char emission[100000]; // total emission payload
    printf("\nProgram emission:\n");
    emission[0] = '\0';
    strcat(emission, "#include <stdio.h>\n");
    strcat(emission, "#include <stdlib.h>\n");
    strcat(emission, "\nint main(){\n");

    // Get variable initialization lines
    getVariableInitialization(s, emission);

    // Get user input lines for non-temporary variables
    getVariableInput(s, emission, inputVariables);

    // Get actual expression lines
    getEquations(s, emission);

    // Get final print statements
    getPrintStatements(s, emission);

    // Add finishing touches and print final emission
    strcat(emission, "\n\treturn 0;\n}\n");
    printf("%s", emission);

    // Save to file TODO: Fix label syntax so that program functions fully
    // FILE *out = fopen("program.c", "w");
    // fputs(emission, out);
    // fclose(out);
}