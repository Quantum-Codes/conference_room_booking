// note: AFTER COMPLETE, DIVIDE INTO MULTIPLE FILES - core.c, helper.c, main.c

/*
Corner cases to take care of:
    1. If record not existing, then don't perform deletion/editing on it
    2. If record exists, then don't add again
    3. Ignore all "marked as deleted" elements for all operations, but dont decrease length of array.
    4. Check for validity of inputs
    5. Prevent undefined behaviours
*/

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
int tokenizer(char rawstring[], char tokenized[MAX_COMMAND_PARTS][MAX_PART_LEN]);
int processInstructions(char command[MAX_COMMAND_PARTS][MAX_PART_LEN], int len_command);
int addRoom(char buildingName[MAX_PART_LEN], char roomId[3]);
int removeRoom(char buildingName[MAX_PART_LEN], char roomId[3]);
int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[3], char time[2]);
int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[3], char time[2]);
void displayRooms(void);
void displayTimeSlots(void);


int main(void) {
    char rawcommand[MAX_COMMAND_PARTS * MAX_PART_LEN]; // store user input
    char command[MAX_COMMAND_PARTS][MAX_PART_LEN];     // store tokenized input
    int len_command;                                   // store length of the tokenized input
    int status;

    while (1) {
        scanf(" %[^\n]s", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        len_command = tokenizer(rawcommand, command);
        if (len_command == 0) {
            printf("\n");
            continue;
        }

        for (int i = 0; i < len_command; i++) {
            printf("(%i %s) ", i, command[i]);
        }
        printf("\n");

        status = processInstructions(command, len_command);
        if (status == 999) {
            return 0;
        }
        printf("\n");
    }
}

int tokenizer(char rawstring[], char tokenized[MAX_COMMAND_PARTS][MAX_PART_LEN]) {
    /*
        tokenizes the rawstring.
        rawstring = source (also modified to lower case in this func)
        tokenized = destination
        return val = length of tokenized array. output in tokenized parameter passed by reference
        return 0 if error (invalid input)
    */
    int word = 0, k = 0; // k = wordcounter
    int i = 0;
    for (; rawstring[i] != '\0'; i++) {
        if (rawstring[i] >= 'A' && rawstring[i] <= 'Z') {
            rawstring[i] += 'a' - 'A';
        }
        else if (!((rawstring[i] >= 'a' && rawstring[i] <= 'z')||(rawstring[i] >= '0' && rawstring[i] <= '9')||(rawstring[i] == ' ')||(rawstring[i] == '\0'))) {
            printf("Invalid characters used.\n");
            return 0;
        }
        if (rawstring[i] == ' ') {
            if (i-1 >= 0 && rawstring[i-1] != ' ') { // ignore consecutive spaces. also using short circuit evaluation to avoid undefined behaviour. if 1st confition is false then never evaluates 2nd in &&
                tokenized[word][k] = '\0';
                word++;
                k = 0;
            }
            continue;
        }
        tokenized[word][k] = rawstring[i];
        k++;
        if (k == MAX_PART_LEN) { // == as we catch error when it exceeds, not after.
            printf("error. max len of command part exceeded\n");
            return 0; // len cant be 0 in normal cases
        }
    }
    if (rawstring[i-1] != ' ') { // ignore contiguous spaces
        tokenized[word][k] = '\0';
        word++;
    }
    return word;
}

int processInstructions(char command[MAX_COMMAND_PARTS][MAX_PART_LEN], int len_command) { // the getFields() func
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
    else if (strcmp(command[0], "exit") == 0) {
        printf("Exiting program...\n");
        return 999;
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
        building name length should be <= MAX_PART_LEN - 4 to make space for the 3 numbers and 1 space
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

int searchInArray(char array[][MAX_PART_LEN], char element[], int lengthOfArray) {
    /*
        Return index of position of element
        Return -1 if not found
    */
    for (int i = 0; i < lengthOfArray; i++) {
        if (strcmp(array[i], element) == 0) {
            return i;
        }
    }
    return -1;
}

void deleteFromArray(char array[][MAX_PART_LEN], int index) {
    array[index][0] = '\0';
}