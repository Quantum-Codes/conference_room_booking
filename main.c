#include <stdio.h>
#include <string.h>

/* 
assumptions:
   input is like : xxxx xxxxx xxx xxxxxx xxx 
   here we define MAX_COMMAND_PARTS as number of words in input. here its 6
   MAX_PART_LEN is the maximum length of each word
*/
#define MAX_COMMAND_PARTS 6
#define MAX_PART_LEN 50
#define MAX_ROOMS 50

// global vars
/*
rooms array format: {'AB1 301', 'Abheri 34', '<any name of max MAX_PART_LEN chars> <any number with max 3 digits'}
timeslots format: {'0 10', '<index of room in rooms array> <start time(hour)>'} 
we take slots of 1 hour and just store start time. then book multiple slots for a multi hour meeting

Convention we take: If 1st character of an element is \0, then it is deleted and we ignore it for all purposes
*/
char rooms[MAX_ROOMS][MAX_PART_LEN];
char timeslots[1000][MAX_PART_LEN];
int roomCount = 0, timeSlotCount = 0;

// prototypes
int tokeniser(char rawstring[], char tokenised[MAX_COMMAND_PARTS][MAX_PART_LEN]);
int processInstructions(char command[MAX_COMMAND_PARTS][MAX_PART_LEN], int len_command);
int addRoom(char buildingName[MAX_PART_LEN], char roomId[3]);
int removeRoom(char buildingName[MAX_PART_LEN], char roomId[3]);
int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[3], char time[2]);
int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[3], char time[2]);
void displayRooms(void);
void displayTimeSlots(void);


int main(void) {
    char rawcommand[MAX_COMMAND_PARTS * MAX_PART_LEN]; // store user input
    char command[MAX_COMMAND_PARTS][MAX_PART_LEN];     // store tokenised input
    int len_command;                                   // store length of the tokenised input
    
    while (1) {
        scanf(" %[^\n]s", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        len_command = tokeniser(rawcommand, command);
        if (len_command == 0) {
            return 1; // exit with error
        }

        for (int i = 0; i < len_command; i++) {
            printf("(%i %s) ", i, command[i]);
        }
        printf("\n");

        processInstructions(command, len_command);
        printf("\n");
    }
}

int tokeniser(char rawstring[], char tokenised[MAX_COMMAND_PARTS][MAX_PART_LEN]) {
    /*
        Tokenises the rawstring.
        rawstring = source
        tokenised = destination
        return val = length of tokenised array. output in tokenised parameter passed by reference
        return 0 if error (invalid input)
    */
    int word = 0, k = 0; // k = wordcounter
    int i = 0;
    for (; rawstring[i] != '\0'; i++) {
        if (rawstring[i] == ' ') {
            if (i-1 >= 0 && rawstring[i-1] != ' ') { // ignore consecutive spaces. also using short circuit evaluation to avoid undefined behaviour. if 1st confition is false then never evaluates 2nd in &&
                tokenised[word][k] = '\0';
                word++;
                k = 0;
            }
            continue;
        }
        tokenised[word][k] = rawstring[i];
        k++;
        if (k == MAX_PART_LEN) { // == as we catch error when it exceeds, not after.
            printf("error. max len of command part exceeded\n");
            return 0; // len cant be 0 in normal cases
        }
    }
    if (rawstring[i-1] != ' ') { // ignore contiguous spaces
        tokenised[word][k] = '\0';
        word++;
    }
    return word;
}

int processInstructions(char command[MAX_COMMAND_PARTS][MAX_PART_LEN], int len_command) {
    /*
        Runs the appropriate function based on the input command (command[0])
    */

    if (strcmp(command[0], "add") == 0) {
        if (len_command != 3){
            printf("Expected 2 arguments, got %d\n", len_command-1);
            return 1;
        } 
        addRoom(command[1], command[2]);
    }
    else if (strcmp(command[0], "remove") == 0) {
        if (len_command != 3){
            printf("Expected 2 arguments, got %d\n", len_command-1);
            return 1;
        } 
        removeRoom(command[1], command[2]);
    }
    else if (strcmp(command[0], "reserve") == 0) {
        if (len_command != 4){
            printf("Expected 3 arguments, got %d\n", len_command-1);
            return 1;
        } 
        reserveRoom(command[1], command[2], command[3]);
    }
    else if (strcmp(command[0], "cancel") == 0) {
        if (len_command != 4){
            printf("Expected 3 arguments, got %d\n", len_command-1);
            return 1;
        } 
        cancelRoom(command[1], command[2], command[3]);
    }
    else if (strcmp(command[0], "rooms") == 0) {
        if (len_command != 1){
            printf("Expected 0 arguments, got %d\n", len_command-1);
            return 1;
        } 
        displayRooms();
    }
    else if (strcmp(command[0], "timeslots") == 0) {
        if (len_command != 1){
            printf("Expected 0 arguments, got %d\n", len_command-1);
            return 1;
        } 
        displayTimeSlots();
    }
    else {
        for (int i = 0; i < len_command; i++) {
            printf("(%i %s) ", i, command[i]);
        }
        printf("\n");
    }
}

int addRoom(char buildingName[MAX_PART_LEN], char roomId[3]) {
    /*
        Return 0 if success
        If error then print an error message then return 1
    */
    printf("addroom params recieved: %s %s\n", buildingName, roomId);
    return 0;
}

int removeRoom(char buildingName[MAX_PART_LEN], char roomId[3]) {
    /*
        Return 0 if success
        If error then print an error message then return 1

        to delete, just mark as deleted (set 1st element of string as \0)
    */
    printf("removeroom params recieved: %s %s\n", buildingName, roomId);
    return 0;
}

int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[3], char time[2]) {
    /*
        Return 0 if success
        If error then print an error message then return 1
    */
    printf("reserveroom params recieved: %s %s %s\n", buildingName, roomId, time);
    return 0;
}

int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[3], char time[2]) {
    /*
        Return 0 if success
        If error then print an error message then return 1
        to delete, just mark as deleted (set string first char as \0)
    */
    printf("cancelroom params recieved: %s %s %s\n", buildingName, roomId, time);
    return 0;
}

void displayRooms(void) {
    /*
        Just print all rooms and buildings neatly. No need of returning anything
        ignore deleted entries
    */
    return;
}

void displayTimeSlots(void) {
    /*
        Just print all timeslots with rooms neatly. No need of returning anything
        ignore deleted entries
    */
    return;
}