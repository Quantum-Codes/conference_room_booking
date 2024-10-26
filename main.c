// note: AFTER COMPLETE, DIVIDE INTO MULTIPLE FILES - core.c, helper.c, main.c

/*
Corner cases to take care of:
    1. If record not existing, then don't perform deletion/editing on it
    2. If record exists, then don't add again
    3. Ignore all "marked as deleted" elements for all operations, but dont decrease length of array.
    4. Check for validity of inputs
    5. Prevent undefined behaviours
*/

#include "globalVars.h"

/* 
assumptions:
   input is like : xxxx xxxxx xxx xxxxxx xxx 
   here we define MAX_COMMAND_PARTS as max number of words allowable in input. here its 6
   MAX_PART_LEN is the maximum length of each word
*/


// global vars in "globalVars.h"
/*
rooms array format: {'AB1 301', 'Abheri 34', '<any name of max MAX_PART_LEN chars> <any number with max 3 digits'}
timeslots format: {'0 10', '<index of room in rooms array> <start time(hour)>'} 
we take slots of 1 hour and just store start time. then book multiple slots for a multi hour meeting

Convention we take: If 1st character of an element is \0, then it is deleted and we ignore it for all purposes
*/
char rooms[MAX_ROOMS][MAX_PART_LEN];
char timeslots[1000][MAX_PART_LEN];
int roomCount = 0, timeSlotCount = 0;

int main(void) {
    char rawcommand[MAX_COMMAND_PARTS * MAX_PART_LEN]; // store user input
    char command[MAX_COMMAND_PARTS][MAX_PART_LEN];     // store tokenized input
    int len_command;                                   // store length of the tokenized input
    int status;                                        // To exit

    printf("\n\
**************************************************\n\
Conference Room Booking (Language: C) [Group 3]\n\
Type 'help' for a list of commands.\n\
Type 'help <commandname>' for usage of a command.\n\
Type 'credits' for team info. Type 'exit' to stop.\n\
**************************************************\n\n");
    while (1) {
        printf(">> ");
        scanf(" %[^\n]s", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        len_command = tokenizer(rawcommand, command);
        if (len_command == 0) {
            printf("\n");
            continue;
        }

        //for (int i = 0; i < len_command; i++) { // for printing tokenized output
        //    printf("(%i %s) ", i, command[i]);
        //}
        //printf("\n");

        status = processInstructions(command, len_command);
        if (status == 999) { // exit
            return 0;
        }
        printf("\n");
    }
}