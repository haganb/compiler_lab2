#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "node.h"
#include "emission.h"


#define BUFFERLEN 2048

// handles the part of the emission where the temporary and non-temporary variables are instantiated on seperate lines
void getVariableInitialization(struct stack *s, char* emission){
    // Start by printing lines to initialize variables and temporary variables
    // Both start with a tab symbol to follow C conventions
    char vars[BUFFERLEN] = "\tint"; // always has to start with int
    char tmpVars[BUFFERLEN] = "\tint"; 
    
    struct node* pointer = s->bottom; // start at bottom of stack and move your way upwards
    bool firstVar = true;       // flag for comma syntax (you need a comment to seperate each variable, except last one)
    bool firstTmpVar = true;    // flag for comma syntax

    strcat(emission, "\t// Variable and temporary variable initialization\n");
    while(pointer->next != NULL){
        // check each nodes name, if it is Tmp...
        char name[BUFFERLEN];
        if(strstr(pointer->nodeName, "Tmp") != NULL){
            if(firstTmpVar){
                sprintf(name, " %s", pointer->nodeName); // needs no comma preceeding it 
                firstTmpVar = false;
            }else{
                sprintf(name, ", %s", pointer->nodeName); // needs comma preceeding it so we can instantiate all on one line
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

// handles the part of the emission where the user is prompted for variable values
void getVariableInput(char* emission, char* inputVariables){
    strcat(emission, "\n\t//Using printf and scanf to get user input values\n");
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

// handles the part of the emission where each instruction is displayed with a corresponding label
void getInstructions(struct stack* s, char* emission){
    strcat(emission, "\n\t//Three-Address Code Representation\n");
    int labelCount = 1; // track number of statements
    
    struct node* pointer = s->bottom;
    while(pointer->next != NULL){
        char buffer[BUFFERLEN];
        if(strstr(pointer->equation, "if(") == NULL){
            // Case for regular expression 
            sprintf(buffer, "\tS%d:\t%s%s", labelCount, pointer->nodeName, pointer->equation);
            labelCount++;
            strcat(emission, buffer);
        }else{
            // Case for if/else expression
            char ifElseBuffer[BUFFERLEN];
            sprintf(ifElseBuffer, "%s", pointer->equation);
            char *delimited = strtok(ifElseBuffer, "\n"); // splits expression up into elements seperated by newline
            while(delimited != NULL){
                if(strcmp(delimited, "}") == 0){
                    // bracket lines dont need a label!
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

// handles the part of the emission where each variables name and values are printed
void getPrintStatements(struct stack* s, char* emission){
    strcat(emission, "\n\t//Print final value of variables\n");

    // Check each node in stack. If the name is not for a temporary variable, create a print statement for it
    struct node* pointer = s->bottom;
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

// Main function to piece together the entire emission
void printEmission(struct stack* s, char* inputVariables){
    char emission[100000]; // total emission payload
    
    // Setup stuff
    emission[0] = '\0';
    strcat(emission, "#include <stdio.h>\n");
    strcat(emission, "#include <stdlib.h>\n");
    strcat(emission, "\nint main(){\n");

    // Get variable initialization lines
    getVariableInitialization(s, emission);

    // Get user input lines for non-temporary variables
    getVariableInput(emission, inputVariables);

    // Get actual expression lines
    getInstructions(s, emission);

    // Get final print statements
    getPrintStatements(s, emission);

    // Add finishing touches and print final emission
    strcat(emission, "\n\treturn 0;\n}\n");
    printf("%s", emission);

    // Save to file TODO: Fix label syntax so that program functions fully
    FILE *out = fopen("program.c", "w");
    fputs(emission, out);
    fclose(out);
}